#include <cstdlib>
#include <iostream>
#include <set>
#include <iterator>
#include <string>

#include <unistd.h>
#include <fstream>
#include <deque>
#include <vector>
#include <stdlib.h>
#include <map>

#include "Event.h"
#include "Utils.h"
#include "PKT_log.h"
#include "Operations_Buffer.h"
#include "Simulator_BufferEngine.h"
#include "Log.h"

LOG_REGISTER_MODULE("Simulator_BufferEngine");

using namespace std;

Simulator_BufferEngine::Simulator_BufferEngine(double timeIN, std::map<int, double> splits, std::map<int, double> splitsOUT, double timeSTOP, double timeSTART, int buffer, int nPkts, std::string muDist)
    : m_timeIN(timeIN), m_splits(splits), m_splitsOUT(splitsOUT), m_timeSTOP(timeSTOP), m_timeSTART(timeSTART), m_maxBufferSize(buffer), m_nPkts(nPkts), m_muDist(muDist)
{
  BEG;
  INFO("Inter arrival rate: ", m_timeIN);
  for (auto split : m_splits)
  {
    INFO("Split ", split.first);
    INFO("  Restart rate: ", split.second);
    INFO("  Service rate: ", splitsOUT[split.first]);
  }
  INFO("Stop rate: ", m_timeSTOP);
  INFO("Restart rate: ", m_timeSTART);
  INFO("Buffer size: ", m_maxBufferSize);
  INFO("Number of packets: ", m_nPkts);

  END;
}

void Simulator_BufferEngine::Run(void)
{
  BEG;
  // Ensure initialization of state variables
  m_splitStateTime = 0.0;
  m_currentSplit = 0;
  m_lastPktIndex = 0;
  m_controllerActive = true;
  // Create first arrival and stop events
  Event evI(GENERATE_IN, 0, ++m_lastPktIndex);
  m_setOfEvents.emplace(evI);
  auto time = exp_neg(m_timeSTOP);
  Event ev2(GENERATE_STOP, time);
  m_setOfEvents.emplace(ev2);

  std::multiset<Event>::iterator item = m_setOfEvents.begin();
  while (item != m_setOfEvents.end())
  {
    switch (item->getType())
    {
    case GENERATE_IN:
      INFO("=================================================================")
      INFO(int(item->getTime() * 100) / 100.0, "\t Arrival in split/state ", m_currentSplit, "/", GetState());
      ProcessArrivalEvent(item);
      INFO(int(item->getTime() * 100) / 100.0, "\t To split/state ", m_currentSplit, "/", GetState());
      break;
    case GENERATE_OUT:
      INFO("=================================================================")
      INFO(int(item->getTime() * 100) / 100.0, "\t Departure in split/state ", m_currentSplit, "/", GetState());
      ProcessDepartureEvent(item);
      INFO(int(item->getTime() * 100) / 100.0, "\t To split/state ", m_currentSplit, "/", GetState());
      break;
    case GENERATE_STOP:
      INFO("=================================================================")
      INFO(int(item->getTime() * 100) / 100.0, "\t Stop in split/state ", m_currentSplit, "/", GetState());
      ProcessStopEvent(item);
      INFO(int(item->getTime() * 100) / 100.0, "\t To split/state ", m_currentSplit, "/", GetState());
      break;
    case GENERATE_START:
      INFO("=================================================================")
      INFO(int(item->getTime() * 100) / 100.0, "\t Restart in split/state ", m_currentSplit, "/", GetState());
      ProcessRestartEvent(item);
      INFO(int(item->getTime() * 100) / 100.0, "\t To split/state ", m_currentSplit, "/", GetState());
      break;
    default:
      ERROR("\n ERROR");
      std::exit(-1);
    }
    m_setOfEvents.erase(item);
    item = m_setOfEvents.begin();
  }
  END;
}

void Simulator_BufferEngine::ProcessArrivalEvent(EventIterator &item)
{
  BEG;
  int state = GetState();
  m_op.AddTimeSplitState(m_currentSplit, state, (item->getTime() - m_splitStateTime));
  INFO("++ Adding ", (item->getTime() - m_splitStateTime), " to split/state ", m_currentSplit, "/", state)
  m_splitStateTime = item->getTime();

  m_op.SetPktTime(TIME_SYSTEM_IN, item->GetPktIdx(), item->getTime());

  if (m_lastPktIndex < m_nPkts)
  {
    double time = item->getTime() + exp_neg(m_timeIN);
    Event ev(GENERATE_IN, time, ++m_lastPktIndex);
    m_setOfEvents.emplace(ev);
  }

  if (m_controllerActive == false || m_resource.HasPkt())
  {
    MSG_ASSERT(m_maxBufferSize > 0 ? m_queue.size() <= m_maxBufferSize : true, "Queue size exceeds limits!!");
    if (m_maxBufferSize <= 0 or m_queue.size() < m_maxBufferSize)
    {
      m_queue.push_back(item->GetPktIdx());
    }
    // else
    // {
    //   WARN("Packet dropped!!");
    // }
  }
  else
  {
    double time = (m_muDist == "EXP" ? item->getTime() + exp_neg(m_splitsOUT.find(m_currentSplit)->second) : item->getTime() + (1 / m_splitsOUT.find(m_currentSplit)->second));
    Event ev(GENERATE_OUT, time, item->GetPktIdx());
    m_setOfEvents.emplace(ev);
    m_op.SetPktTime(TIME_SERVER_IN, item->GetPktIdx(), item->getTime());

    m_op.AddResourceTime(item->getTime() - m_resource.GetChangeTime(), m_resource.IsBusy());
    m_resource.SetPkt(item->GetPktIdx(), item->getTime());
  }

  END;
}

void Simulator_BufferEngine::ProcessDepartureEvent(EventIterator &item)
{
  BEG;
  MSG_ASSERT(m_resource.IsBusy() == true, "Resource has to be busy when processing a departure");
  MSG_ASSERT(m_controllerActive == true, "Controller has to be active when processing a departure");
  MSG_ASSERT(m_currentSplit != int(m_splits.size()), "At restart current split is standby");

  int state = GetState();
  m_op.AddTimeSplitState(m_currentSplit, state, (item->getTime() - m_splitStateTime));
  INFO("++ Adding ", (item->getTime() - m_splitStateTime), " to split/state ", m_currentSplit, "/", state)
  m_splitStateTime = item->getTime();
  m_op.SetPktTime(TIME_SYSTEM_OUT, item->GetPktIdx(), item->getTime());

  if (m_queue.size() > 0)
  {
    ProcessFromQueue(item);
  }
  else
  {
    m_op.AddResourceTime(item->getTime() - m_resource.GetChangeTime(), m_resource.IsBusy());
    m_resource.Free(item->getTime());
  }
  if (m_lastPktIndex == m_nPkts && m_queue.size() == 0)
  {
    RemoveEvents(GENERATE_STOP);
  }
  END;
}

void Simulator_BufferEngine::ProcessStopEvent(EventIterator &item)
{
  BEG;
  MSG_ASSERT(m_controllerActive == true, "Controller has to be active when processing a stop");
  MSG_ASSERT(m_currentSplit != int(m_splits.size()), "At restart current split is standby");

  int state = GetState();
  m_op.AddTimeSplitState(m_currentSplit, state, (item->getTime() - m_splitStateTime));
  INFO("++ Adding ", (item->getTime() - m_splitStateTime), " to split/state ", m_currentSplit, "/", state)
  m_splitStateTime = item->getTime();

  m_currentSplit = m_splits.size();
  m_controllerActive = false;

  if (m_resource.IsBusy())
  {
    m_resource.CachePkt();
  }
  RemoveEvents(GENERATE_OUT);

  // Generate restart event
  double time = item->getTime() + exp_neg(m_timeSTART);
  Event ev1(GENERATE_START, time);
  m_setOfEvents.emplace(ev1);
  END;
}

void Simulator_BufferEngine::ProcessRestartEvent(EventIterator &item)
{
  BEG;
  MSG_ASSERT(m_resource.IsBusy() == false, "Resource cannot be busy when processing a restart");
  MSG_ASSERT(m_controllerActive == false, "Controller cannot be active when processing a restart");
  MSG_ASSERT(m_currentSplit == int(m_splits.size()), "At restart current split is standby");

  int state = GetState();
  m_op.AddTimeSplitState(m_currentSplit, state, (item->getTime() - m_splitStateTime));
  INFO("++ Adding ", (item->getTime() - m_splitStateTime), " to split/state ", m_currentSplit, "/", state)
  m_splitStateTime = item->getTime();

  // Calculate new split
  int random = rand() % 100 + 1;
  int number = 0;
  int number2 = 0;
  for (auto i = 0u; i < m_splits.size(); i++)
  {
    number += m_splits.find(i)->second * 100;
    if (random >= number2 && random <= number)
    {
      m_currentSplit = i;
      break;
    }
    number2 = number;
  }

  m_controllerActive = true;
  if (m_resource.HasCachedPkt())
  {
    double time = (m_muDist == "EXP" ? item->getTime() + exp_neg(m_splitsOUT.find(m_currentSplit)->second) : item->getTime() + (1 / m_splitsOUT.find(m_currentSplit)->second));
    Event ev(GENERATE_OUT, time, m_resource.GetCachedPkt());
    m_setOfEvents.emplace(ev);
    m_resource.ProcessFromCache();
  }
  else if (m_queue.size() > 0)
  {
    ProcessFromQueue(item);
  }

  double time = item->getTime() + exp_neg(m_timeSTOP);
  Event ev2(GENERATE_STOP, time);
  m_setOfEvents.emplace(ev2);
  END;
}

void Simulator_BufferEngine::ProcessFromQueue(EventIterator &item)
{
  BEG;
  double time = (m_muDist == "EXP" ? item->getTime() + exp_neg(m_splitsOUT.find(m_currentSplit)->second) : item->getTime() + (1 / m_splitsOUT.find(m_currentSplit)->second));

  auto pktIdx = m_queue.front();
  m_queue.pop_front();
  Event ev(GENERATE_OUT, time, pktIdx);
  m_setOfEvents.emplace(ev);

  m_op.SetPktTime(TIME_SERVER_IN, pktIdx, item->getTime());

  // Update busy time
  m_op.AddResourceTime(item->getTime() - m_resource.GetChangeTime(), m_resource.IsBusy());
  m_resource.SetPkt(pktIdx, item->getTime());
  END;
}

void Simulator_BufferEngine::RemoveEvents(enum type eventType)
{
  BEG;
  for (auto it = m_setOfEvents.begin(); it != m_setOfEvents.end(); it++)
  {
    if (it->getType() == eventType)
    {
      m_setOfEvents.erase(it);
    }
  }
  END;
}

int Simulator_BufferEngine::GetState(void) const
{
  return (m_resource.HasPkt() ? m_queue.size() + 1 : m_queue.size());
}

double Simulator_BufferEngine::GetAverageSystemTime() const
{
  BEG;
  return m_op.GetAverageSystemTime();
  END;
}

double Simulator_BufferEngine::GetAverageQueueTime() const
{
  BEG;
  return m_op.GetAverageQueueTime();
  END;
}
double Simulator_BufferEngine::GetAverageServerTime() const
{
  BEG;
  return m_op.GetAverageServerTime();
  END;
}

double Simulator_BufferEngine::GetAverageElementsQueue() const
{
  BEG;
  return m_op.GetAverageElementsQueue();
  END;
}

double Simulator_BufferEngine::GetOccupationRatio() const
{
  BEG;
  return m_op.GetOccupationRatio();
  END;
}

double Simulator_BufferEngine::GetLossProbability() const
{
  BEG;
  return m_op.GetLossProbability();
  END;
}

double Simulator_BufferEngine::GetProbSplit(int split) const
{
  BEG;
  return m_op.GetProbSplit(split);
  END;
}

void Simulator_BufferEngine::GetProbState(std::string file)
{
  BEG;
  return m_op.GetProbState(file);
  END;
}
void Simulator_BufferEngine::GetPktLog(std::string file)
{
  BEG;
  return m_op.GetPktLog(file);
  END;
}