#ifndef UTILS_H
#define UTILS_H

#include <math.h>
#include <iostream>
#include <random>

using namespace std;

enum type
{
  GENERATE_IN = 1,
  GENERATE_OUT,
  GENERATE_STOP,
  GENERATE_START
};

enum typeOp
{
  TIME_SYSTEM_IN = 1,
  TIME_SERVER_IN,
  TIME_SYSTEM_OUT
};

static std::random_device rd;
// static std::default_random_engine generator(rd());
static std::default_random_engine generator(0);
inline double
exp_neg(double average)
{
  std::exponential_distribution<double> distribution(average);
  return distribution(generator);
}

static std::random_device rdU;
static std::default_random_engine generatorU(rdU());
inline double
uniform_dist(double min, double max)
{
  std::uniform_real_distribution<> dis(min, max);
  return dis(generator);
}

// inline void
// readEvents(std::multiset<Event> setOfEvents)
// {
//   for (const auto &item : setOfEvents)
//   {
//     std::cout << "\nTipo del evento: " << item.getType() << ". Tiempo del evento: " << item.getTime() << std::endl;
//   }
// }

#endif /* UTILS_H */