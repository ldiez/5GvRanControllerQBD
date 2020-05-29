#ifndef EVENT_H
#define EVENT_H

class Event
{
public:
  Event(int type, double time);
  Event(int type, double time, int pktIdx);
  int getType() const;
  double getTime() const;
  int GetPktIdx() const;
  bool operator<(const Event &ev) const;
  void in_event(double time, int queue);
  void out_event(double time, int queue);

private:
  const int m_type;
  const double m_time;
  const int m_pktIdx;
};

#endif /* EVENT_H */
