#ifndef RESOURCE_H
#define RESOURCE_H

class Resource
{
public:
  Resource(void);
  bool IsBusy(void) const;
  bool HasCachedPkt(void) const;
  bool HasPkt(void) const;
  int GetPkt(void) const;
  int GetCachedPkt(void) const;
  double GetChangeTime(void) const;
  void SetPkt(int pktIdx, double time);
  void ProcessFromCache();
  void CachePkt(void);
  void Free(double time);

private:
  int m_currentPktIdx;
  int m_cachedPktIdx;
  double m_changeTime;
};

#endif /* RESOURCE_H */