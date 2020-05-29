#ifndef PKT_log_H
#define PKT_log_H

class PKT_log
{
public:
  PKT_log(void);
  double getTimeSystIN() const;
  double getTimeServerIN() const;
  double getTimeSystOUT() const;
  void setTimeSystIN(double timeIN);
  void setTimeServerIN(double timeIN);
  void setTimeSystOUT(double timeOUT);
  bool IsLost(void) const;

private:
  double m_timeSystIN;
  double m_timeSystOUT;
  double m_timeServerIN;
};

#endif /* PKT_log_H */
