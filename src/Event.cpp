#include "Event.h"
#include "Utils.h"
#include "Log.h"

#include <iostream>
#include <set>

LOG_REGISTER_MODULE("Event");
std::set<Event> setOfEvents;

//Constructor de la clase Evento
Event::Event(int type, double time)
    : m_type(type), m_time(time), m_pktIdx(-1)
{
  BEG;
  INFO("Event ", m_type, ". Time: ", m_time);
  END;
}

Event::Event(int type, double time, int pktIdx)
    : m_type(type), m_time(time), m_pktIdx(pktIdx)
{
  BEGEND;
}

//Getter del tipo de evento
int Event::getType() const
{
  return m_type;
}

//Getter del tiempo en que se generó el evento
double Event::getTime() const
{
  BEGEND;
  return m_time;
}

int Event::GetPktIdx() const
{
  BEGEND;
  return m_pktIdx;
}

//Operación <
//Ordenar el set para que ordene eventos por tiempo: fifo --> primero que llegue (menor tiempo), antes está en la cola
bool Event::operator<(const Event &ev) const
{

  if (ev.m_time == m_time)
  {
    if (m_type == GENERATE_OUT)
    {
      return true;
    }
  }
  return ev.m_time > m_time;
}
