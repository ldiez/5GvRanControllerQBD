#include "PKT_log.h"
#include "Log.h"

LOG_REGISTER_MODULE("PKT_log");

PKT_log::PKT_log() : m_timeSystIN(-1), m_timeSystOUT(-1), m_timeServerIN(-1)
{
  BEGEND;
}

double PKT_log::getTimeSystIN() const
{
  BEGEND;
  return m_timeSystIN;
}

double PKT_log::getTimeServerIN() const
{
  BEGEND;
  return m_timeServerIN;
}

double PKT_log::getTimeSystOUT() const
{
  BEGEND;
  return m_timeSystOUT;
}

void PKT_log::setTimeSystIN(double timeIN)
{
  BEG;
  m_timeSystIN = timeIN;
  END;
}

void PKT_log::setTimeServerIN(double timeIN)
{
  BEG;
  m_timeServerIN = timeIN;
  END;
}

void PKT_log::setTimeSystOUT(double timeOUT)
{
  BEG;
  m_timeSystOUT = timeOUT;
  END;
}

bool PKT_log::IsLost(void) const
{
  BEGEND;
  return (m_timeSystOUT <= 0);
}