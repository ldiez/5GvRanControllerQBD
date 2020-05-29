#include <cstdlib>
#include <iostream>
#include <set>
#include <iterator>
#include <string>

#include <unistd.h>
#include <fstream>
#include <deque>
#include <map>
#include <vector>

#include "Event.h"
#include "Utils.h"
#include "PKT_log.h"
#include "Operations_Buffer.h"
#include "Simulator_BufferMM1.h"
#include "Log.h"

LOG_REGISTER_MODULE("Simulator_BufferMM1");

using namespace std;

Simulator_BufferMM1::Simulator_BufferMM1(double timeIN, double timeOUT, int maxBufferSize, int nPkts, std::string muDist)
    : m_timeIN(timeIN), m_timeOUT(timeOUT), m_maxBufferSize(maxBufferSize), m_nPkts(nPkts), m_muDist(muDist)
{
  BEG;
  INFO("Inter arrival rate: ", m_timeIN);
  INFO("Service rate: ", m_timeOUT);
  INFO("Buffer size: ", m_maxBufferSize);
  INFO("Number of packets: ", m_nPkts);
  END;
}

void Simulator_BufferMM1::Run()
{
  BEG;
  m_lastPktIndex = 0;
  m_stateTime = 0.0;

  Event ev(GENERATE_IN, 0, ++m_lastPktIndex);
  m_setOfEvents.emplace(ev);

  std::multiset<Event>::iterator item = m_setOfEvents.begin();
  while (item != m_setOfEvents.end())
  {
    switch (item->getType())
    {
    case GENERATE_IN:

      INFO("=================================================================")
      INFO(int(item->getTime() * 100) / 100.0, "\t Arrival in state ", m_queue.size());
      ProcessArrivalEvent(item);
      INFO(int(item->getTime() * 100) / 100.0, "\t To state ", m_queue.size());
      break;
    case GENERATE_OUT:
      INFO("=================================================================")
      INFO(int(item->getTime() * 100) / 100.0, "\t Departure in state ", m_queue.size());
      ProcessDepartureEvent(item);
      INFO(int(item->getTime() * 100) / 100.0, "\t To state ", m_queue.size());
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

void Simulator_BufferMM1::ProcessArrivalEvent(EventIterator &item)
{
  BEG;
  int state = m_queue.size();
  m_op.AddTimeSplitState(0, state, (item->getTime() - m_stateTime));
  INFO("++ Adding ", (item->getTime() - m_stateTime), " to state ", state);
  m_stateTime = item->getTime();

  m_op.SetPktTime(TIME_SYSTEM_IN, item->GetPktIdx(), item->getTime());

  if (m_lastPktIndex < m_nPkts)
  {
    double time = item->getTime() + exp_neg(m_timeIN);
    Event ev(GENERATE_IN, time, ++m_lastPktIndex);
    m_setOfEvents.emplace(ev);
  }

  if (m_resource.IsBusy())
  {
    MSG_ASSERT(m_maxBufferSize > 0 ? int(m_queue.size()) <= m_maxBufferSize : true, "Queue size exceeds limits!!");
    if (m_maxBufferSize <= 0 or int(m_queue.size()) < m_maxBufferSize)
    {
      m_queue.push_back(item->GetPktIdx());
    }
  }
  else
  {
    double time = (m_muDist == "EXP" ? item->getTime() + exp_neg(m_timeOUT) : item->getTime() + (1 / m_timeOUT));
    Event ev(GENERATE_OUT, time, item->GetPktIdx());
    m_setOfEvents.emplace(ev);
    m_op.SetPktTime(TIME_SERVER_IN, item->GetPktIdx(), item->getTime());

    m_op.AddResourceTime(item->getTime() - m_resource.GetChangeTime(), m_resource.IsBusy());
    m_resource.SetPkt(item->GetPktIdx(), item->getTime());
  }
  END;
}

void Simulator_BufferMM1::ProcessDepartureEvent(EventIterator &item)
{
  BEG;
  MSG_ASSERT(m_resource.IsBusy() == true, "Resource has to be busy when processing a departure");

  int state = m_queue.size();
  m_op.AddTimeSplitState(0, state, (item->getTime() - m_stateTime));
  INFO("++ Adding ", (item->getTime() - m_stateTime), " to state ", state)
  m_stateTime = item->getTime();
  m_op.SetPktTime(TIME_SYSTEM_OUT, item->GetPktIdx(), item->getTime());

  if (m_queue.size() > 0)
  {
    double time = (m_muDist == "EXP" ? item->getTime() + exp_neg(m_timeOUT) : item->getTime() + (1 / m_timeOUT));

    auto pktIdx = m_queue.front();
    m_queue.pop_front();
    Event ev(GENERATE_OUT, time, pktIdx);
    m_setOfEvents.emplace(ev);

    m_op.SetPktTime(TIME_SERVER_IN, pktIdx, item->getTime());

    // Update busy time
    m_op.AddResourceTime(item->getTime() - m_resource.GetChangeTime(), m_resource.IsBusy());
    m_resource.SetPkt(pktIdx, item->getTime());
  }
  else
  {
    m_op.AddResourceTime(item->getTime() - m_resource.GetChangeTime(), m_resource.IsBusy());
    m_resource.Free(item->getTime());
  }

  END;
}

double Simulator_BufferMM1::GetAverageSystemTime() const
{
  BEG;
  return m_op.GetAverageSystemTime();
  END;
}

double Simulator_BufferMM1::GetAverageQueueTime() const
{
  BEG;
  return m_op.GetAverageQueueTime();
  END;
}
double Simulator_BufferMM1::GetAverageServerTime() const
{
  BEG;
  return m_op.GetAverageServerTime();
  END;
}

double Simulator_BufferMM1::GetAverageElementsQueue() const
{
  BEG;
  return m_op.GetAverageElementsQueue();
  END;
}

double Simulator_BufferMM1::GetOccupationRatio() const
{
  BEG;
  return m_op.GetOccupationRatio();
  END;
}

double Simulator_BufferMM1::GetLossProbability() const
{
  BEG;
  return m_op.GetLossProbability();
  END;
}

void Simulator_BufferMM1::GetProbState(std::string file)
{
  BEG;
  return m_op.GetProbState(file);
  END;
}
void Simulator_BufferMM1::GetPktLog(std::string file)
{
  BEG;
  return m_op.GetPktLog(file);
  END;
}