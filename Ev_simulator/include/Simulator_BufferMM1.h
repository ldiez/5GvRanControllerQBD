#ifndef Simulator_BufferMM1_H
#define Simulator_BufferMM1_H

#include "Operations_Buffer.h"
#include "Resource.h"

using EventIterator = std::multiset<Event>::iterator;
class Simulator_BufferMM1
{
public:
  Simulator_BufferMM1(double timeIN, double timeOUT, int maxBufferSize, int nPkts, std::string muDist);
  void Run();

  double GetAverageSystemTime() const;
  double GetAverageQueueTime() const;
  double GetAverageServerTime() const;
  double GetAverageElementsQueue() const;
  double GetOccupationRatio() const;
  double GetLossProbability() const;
  void GetProbState(std::string file);
  void GetPktLog(std::string file);

private:
  // configuration
  const double m_timeIN;
  const double m_timeOUT;
  const int m_maxBufferSize;
  const int m_nPkts;
  const std::string m_muDist;
  // Resource
  Resource m_resource;
  // Buffer
  std::deque<int> m_queue;
  //
  double m_stateTime = 0.0;
  int m_lastPktIndex = 0;
  Operations_Buffer m_op;

  std::multiset<Event> m_setOfEvents;

  void ProcessArrivalEvent(EventIterator &item);
  void ProcessDepartureEvent(EventIterator &item);
};

#endif /* Simulator_BufferMM1_H */
