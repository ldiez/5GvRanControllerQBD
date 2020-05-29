#ifndef Operations_Buffer_H
#define Operations_Buffer_H

class Operations_Buffer
{
public:
  Operations_Buffer();
  double GetAverageSystemTime() const;
  double GetAverageQueueTime() const;
  double GetAverageServerTime() const;
  double GetAverageElementsQueue() const;
  double GetOccupationRatio() const;
  double GetLossProbability() const;
  double GetProbSplit(int split) const;
  void GetProbState(std::string file);
  void GetPktLog(std::string file);

  /**
   * Set logging time of a packet
   */
  void SetPktTime(enum typeOp timeType, int index, double time);
  /**
   * Add Time to a pair split/estate
  */
  void AddTimeSplitState(int split, int state, double time);
  /**
   * Add resource busy time
   * @Time
   * @Busy: true is busy, false if idle
   */
  void AddResourceTime(double time, bool busy);

private:
  std::map<int, PKT_log> m_pkts;
  std::map<int, std::map<int, double>> m_timeSplitState;
  std::map<int, double> m_timeState;
  double m_totalTime;
  double m_busyTime;
  double m_idleTime;
};

#endif /* Operations_Buffer_H */
