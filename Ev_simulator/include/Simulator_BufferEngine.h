#ifndef Simulator_BufferEngine_H
#define Simulator_BufferEngine_H

#include "Operations_Buffer.h"
#include "Resource.h"

using EventIterator = std::multiset<Event>::iterator;
class Simulator_BufferEngine
{
public:
  Simulator_BufferEngine(double timeIN, std::map<int, double> splits, std::map<int, double> splitsOUT, double timeSTOP, double timeSTART, int buffer, int nPkts, std::string muDist);
  void Run(void);
  double GetAverageSystemTime() const;
  double GetAverageQueueTime() const;
  double GetAverageServerTime() const;
  double GetAverageElementsQueue() const;
  double GetOccupationRatio() const;
  double GetLossProbability() const;
  double GetProbSplit(int split) const;
  void GetProbState(std::string file);
  void GetPktLog(std::string file);

private:
  // Configuration parameters
  const double m_timeIN;
  const std::map<int, double> m_splits;
  const std::map<int, double> m_splitsOUT;
  const double m_timeSTOP;
  const double m_timeSTART;
  const unsigned int m_maxBufferSize;
  const int m_nPkts;
  const std::string m_muDist;

  // Resource
  Resource m_resource;
  // Buffer
  std::deque<int> m_queue;
  // Split and queue control
  double m_splitStateTime = 0.0;
  int m_currentSplit = 0;
  //
  int m_lastPktIndex = 0;
  bool m_controllerActive = true;

  // Events container
  std::multiset<Event> m_setOfEvents;
  // Logging
  Operations_Buffer m_op;

  void ProcessArrivalEvent(EventIterator &item);
  void ProcessDepartureEvent(EventIterator &item);
  void ProcessStopEvent(EventIterator &item);
  void ProcessRestartEvent(EventIterator &item);
  void ProcessFromQueue(EventIterator &item);
  void RemoveEvents(enum type eventType);
  int GetState(void) const;
};

#endif /* Simulator_BufferEngine_H */
