#include <unistd.h>
#include <fstream>

#include <iostream>
#include <map>
#include <cstdlib>
#include <string>

#include "PKT_log.h"
#include "Utils.h"
#include "Operations_Buffer.h"
#include "Log.h"

using namespace std;
LOG_REGISTER_MODULE("Operations_Buffer");

Operations_Buffer::Operations_Buffer()
    : m_totalTime(0.0), m_busyTime(0.0), m_idleTime(0.0)
{
  BEGEND;
}

void Operations_Buffer::SetPktTime(enum typeOp timeType, int index, double time)
{
  BEG;
  m_totalTime = time;
  switch (timeType)
  {
  case TIME_SYSTEM_IN:
    m_pkts[index].setTimeSystIN(time);
    break;
  case TIME_SERVER_IN:
    m_pkts[index].setTimeServerIN(time);
    break;
  case TIME_SYSTEM_OUT:
    m_pkts[index].setTimeSystOUT(time);
    break;
  default:
    MSG_ASSERT(false, "\n ERROR");
    break;
  }
  END;
}

void Operations_Buffer::AddTimeSplitState(int split, int state, double time)
{
  BEG;
  m_timeSplitState[split][state] += time;
  m_timeState[state] += time;
  END;
}

void Operations_Buffer::AddResourceTime(double time, bool busy)
{
  BEG;
  if (busy)
  {
    m_busyTime += time;
  }
  else
  {
    m_idleTime += time;
  }
  END;
}

double Operations_Buffer::GetAverageSystemTime() const
{
  BEG;
  double sumIN = 0;
  double sumOUT = 0;
  auto pktCtr = 0;
  for (auto &pkt : m_pkts)
  {
    if (pkt.second.IsLost())
    {
      continue;
    }
    ++pktCtr;
    sumIN += pkt.second.getTimeSystIN();
    sumOUT += pkt.second.getTimeSystOUT();
  }
  END;
  return (sumOUT - sumIN) / pktCtr;
}

double Operations_Buffer::GetAverageQueueTime() const
{
  BEG;
  double sumIN = 0;
  double sumOUT = 0;
  auto pktCtr = 0;
  for (auto &pkt : m_pkts)
  {
    if (pkt.second.IsLost())
    {
      continue;
    }
    ++pktCtr;
    sumIN += pkt.second.getTimeSystIN();
    sumOUT += pkt.second.getTimeServerIN();
  }
  END;
  return (sumOUT - sumIN) / pktCtr;
}
double Operations_Buffer::GetAverageServerTime() const
{
  BEG;
  double sumIN = 0;
  double sumOUT = 0;
  auto pktCtr = 0;
  for (auto &pkt : m_pkts)
  {
    if (pkt.second.IsLost())
    {
      continue;
    }
    ++pktCtr;
    sumIN += pkt.second.getTimeServerIN();
    sumOUT += pkt.second.getTimeSystOUT();
  }
  END;
  return (sumOUT - sumIN) / pktCtr;
}

double Operations_Buffer::GetAverageElementsQueue() const
{
  BEG;
  double result = 0;
  for (auto i = 0u; i < m_timeState.size(); i++)
  {
    result += i * (m_timeState.find(i)->second / m_totalTime);
  }
  END;
  return result;
}

double Operations_Buffer::GetOccupationRatio() const
{
  BEG;
  return (m_busyTime / m_totalTime);
  END;
}

double Operations_Buffer::GetLossProbability() const
{
  BEG;
  auto totPkts = m_pkts.size();
  auto mLost = 0;
  for (auto pkt : m_pkts)
  {
    if (pkt.second.IsLost())
    {
      ++mLost;
    }
  }
  END;
  return (double(mLost) / totPkts);
}

double Operations_Buffer::GetProbSplit(int split) const
{
  BEG;
  double timeSplit = 0;
  auto splitIter = m_timeSplitState.find(split);
  if (splitIter == m_timeSplitState.end())
  {
    return -1;
  }

  for (auto &state : splitIter->second)
  {
    timeSplit += state.second;
  }

  END;
  return (timeSplit / m_totalTime);
}

void Operations_Buffer::GetProbState(std::string file)
{
  BEG;

  auto maxStates = 0;
  for (auto &split : m_timeSplitState)
  {
    auto aux = split.second.rbegin()->first;
    maxStates = aux > maxStates ? aux : maxStates;
  }

  ofstream fs(file);
  for (auto state = 0; state < maxStates + 1; ++state)
  {
    fs << state << "\t";
    for (auto &split : m_timeSplitState)
    {
      if (split.second.find(state) != split.second.end())
      {
        fs << split.second.at(state) / m_totalTime;
      }
      else
      {
        fs << 0.0;
      }
      fs << "\t";
    }
    fs << "\n";
  }
  fs.close();
  END;
}

void Operations_Buffer::GetPktLog(std::string fn)
{
  BEG;
  ofstream fs(fn); //, std::ofstream::out | std::ofstream::app);
  for (auto pkt : m_pkts)
  {
    if (pkt.second.IsLost())
    {
      continue;
    }
    auto sysTime = pkt.second.getTimeSystOUT() - pkt.second.getTimeSystIN();
    auto queueTime = pkt.second.getTimeServerIN() - pkt.second.getTimeSystIN();
    auto serverTime = pkt.second.getTimeSystOUT() - pkt.second.getTimeServerIN();
    fs << sysTime << "\t" << queueTime << "\t" << serverTime << "\n";
  }
  fs.close();
  END;
}