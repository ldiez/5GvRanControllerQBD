#include <cstdlib>
#include <iostream>
#include <set>
#include <iterator>
#include <string>

#include <unistd.h>
#include <fstream>
#include <queue>
#include <map>
#include <iomanip>

#include "Event.h"
#include "Utils.h"
#include "PKT_log.h"
#include "Simulator_BufferMM1.h"
#include "Simulator_BufferEngine.h"
#include "Log.h"
using namespace std;
LOG_REGISTER_MODULE("MAIN");

enum scenarios
{
  BUFFER_ENGINE_2,
  BUFFER_ENGINE_4
};
static const std::map<std::string, int> scen_map{
    {"BUFFER_ENGINE_2", scenarios::BUFFER_ENGINE_2},
    {"BUFFER_ENGINE_4", scenarios::BUFFER_ENGINE_4},
};

int main(int argc, char *argv[])
{
  LOG_SET_LEVEL("Simulator_BufferEngine", LogLevel::WARNING);
  MSG_ASSERT(argc >= 4, "Too few arguments");
  std::string scenario = argv[1];
  int nPkts = atoi(argv[2]);
  std::string muDist = argv[3];

  int argOffset = 3;

  auto scenMap = scen_map.find(scenario);
  MSG_ASSERT(scenMap != scen_map.end(), "Incorrect scenario name");
  switch (scenMap->second)
  {
  case scenarios::BUFFER_ENGINE_4:
  {
    MSG_ASSERT((argc == 19 + argOffset + 1), "Incorrect number of parameters");
    double xi = 1;
    double buffer = atof(argv[1 + argOffset]);
    double alpha1 = atof(argv[2 + argOffset]);
    double alpha2 = atof(argv[3 + argOffset]);
    double alpha3 = atof(argv[4 + argOffset]);
    double alpha4 = atof(argv[5 + argOffset]);
    double mu1 = atof(argv[6 + argOffset]);
    double mu2 = atof(argv[7 + argOffset]);
    double mu3 = atof(argv[8 + argOffset]);
    double mu4 = atof(argv[9 + argOffset]);
    double lambda = atof(argv[10 + argOffset]);
    double gamma = atof(argv[11 + argOffset]);
    std::string fileHead = argv[12 + argOffset];
    std::string fileTail = argv[13 + argOffset];
    unsigned int iters = atoi(argv[14 + argOffset]);
    std::map<int, double> alphas{{0, alpha1}, {1, alpha2}, {2, alpha3}, {3, alpha4}};
    std::map<int, double> mus{{0, mu1}, {1, mu2}, {2, mu3}, {3, mu4}};

    unsigned int printPktLogs = atoi(argv[15 + argOffset]);
    unsigned int printStates = atoi(argv[16 + argOffset]);
    unsigned int printplTtAll = atoi(argv[17 + argOffset]);
    unsigned int printAvgs = atoi(argv[18 + argOffset]);
    unsigned int printAvgPlTt = atoi(argv[19 + argOffset]);

    auto avgSystemTime = 0.0;
    auto avgQueueTime = 0.0;
    auto avgServerTime = 0.0;
    auto avgQueueSize = 0.0;
    auto occRatio = 0.0;
    auto lossProb = 0.0;

    for (auto it = 0u; it < iters; ++it)
    {
      Simulator_BufferEngine simulatorEng1(lambda, alphas, mus, gamma, xi, buffer, nPkts, muDist);
      simulatorEng1.Run();
      std::stringstream ss;
      if (printPktLogs == 1)
      {
        ss.str("");
        ss << fileHead << "pktsLog_"
           << std::setw(2) << std::setfill('0') << int(lambda * 10)
           << "_" << std::setw(4) << std::setfill('0') << buffer
           << "_" << std::setw(4) << std::setfill('0') << it
           << fileTail;
        UINFO("Packets log to: ", ss.str())
        simulatorEng1.GetPktLog(ss.str());
      }
      if (printStates == 1)
      {
        ss.str("");
        ss << fileHead << "prob_states_"
           << std::setw(2) << std::setfill('0') << int(lambda * 10)
           << "_" << std::setw(4) << std::setfill('0') << buffer
           << "_" << std::setw(4) << std::setfill('0') << it
           << fileTail;
        simulatorEng1.GetProbState(ss.str());
        UINFO("States probabilities to: ", ss.str())
      }
      if (printplTtAll == 1)
      {
        ss.str("");
        ss << fileHead << "ploss_vs_ttotal_"
           << std::setw(2) << std::setfill('0') << int(lambda * 10)
           << "_" << std::setw(4) << std::setfill('0') << buffer
           << fileTail;
        ofstream fs1(ss.str(), std::ofstream::out | std::ofstream::app);
        fs1 << std::setprecision(10)
            << "\t" << simulatorEng1.GetAverageSystemTime()
            << "\t" << simulatorEng1.GetLossProbability()
            << std::endl;
        fs1.close();
      }
      avgSystemTime += simulatorEng1.GetAverageSystemTime();
      avgQueueTime += simulatorEng1.GetAverageQueueTime();
      avgServerTime += simulatorEng1.GetAverageServerTime();
      avgQueueSize += simulatorEng1.GetAverageElementsQueue();
      occRatio += simulatorEng1.GetOccupationRatio();
      lossProb += simulatorEng1.GetLossProbability();
    }

    std::stringstream ss;
    if (printAvgs == 1)
    {
      ss.str("");
      ss << fileHead << "simTimes"
         << "_" << std::setw(4) << std::setfill('0') << buffer
         << fileTail;

      ofstream fs1(ss.str(), std::ofstream::out | std::ofstream::app);
      fs1 << lambda
          << "\t" << std::setprecision(10)
          << (avgSystemTime / iters)
          << "\t" << (avgQueueTime / iters)
          << "\t" << (avgServerTime / iters)
          << "\t" << (avgQueueSize / iters)
          << "\t" << (occRatio / iters)
          << "\t" << (lossProb / iters)
          << std::endl;
      fs1.close();
    }
    if (printAvgPlTt == 1)
    {
      ss.str(""); // reset it
      ss << fileHead << "ploss_vs_ttotal_"
         << std::setw(2) << std::setfill('0') << int(lambda * 10)
         << fileTail;
      ofstream fs2(ss.str(), std::ofstream::out | std::ofstream::app);
      fs2 << buffer
          << std::setprecision(10)
          << "\t" << (avgSystemTime / iters)
          << "\t" << (lossProb / iters)
          << std::endl;
      fs2.close();
    }

    break;
  }
  default:
    UERROR("Unknown scenario: ", scenario);
    break;
  }
}