#include "Resource.h"
#include "Log.h"

LOG_REGISTER_MODULE("Resource");

Resource::Resource(void)
    : m_currentPktIdx(-1), m_cachedPktIdx(-1), m_changeTime(0.0)
{
  BEGEND;
}
bool Resource::IsBusy(void) const
{
  BEGEND;
  return (m_currentPktIdx != -1);
}

bool Resource::HasCachedPkt(void) const
{
  BEGEND;
  return (m_cachedPktIdx != -1);
}

bool Resource::HasPkt(void) const
{
  BEGEND;
  return (m_cachedPktIdx != -1 || m_currentPktIdx != -1);
}

int Resource::GetPkt(void) const
{
  BEGEND;
  return m_currentPktIdx;
}

int Resource::GetCachedPkt(void) const
{
  BEGEND;
  return m_cachedPktIdx;
}

double Resource::GetChangeTime(void) const
{
  BEGEND;
  return m_changeTime;
}

void Resource::SetPkt(int pktIdx, double time)
{
  BEG;
  m_currentPktIdx = pktIdx;
  m_changeTime = time;
  END;
}

void Resource::ProcessFromCache()
{
  BEG;
  MSG_ASSERT(m_cachedPktIdx != -1, "Empty cache");
  m_currentPktIdx = m_cachedPktIdx;
  m_cachedPktIdx = -1;
  END;
}

void Resource::CachePkt(void)
{
  BEG;
  MSG_ASSERT(m_currentPktIdx != -1, "Empty packet");
  m_cachedPktIdx = m_currentPktIdx;
  m_currentPktIdx = -1;
  END;
}

void Resource::Free(double time)
{
  BEG;
  m_currentPktIdx = -1;
  m_cachedPktIdx = -1;
  m_changeTime = time;
  END;
}