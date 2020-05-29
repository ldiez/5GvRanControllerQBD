# 5G vRAN Controller model based on Quasi-Birth-Death (QBD) process

This repository contains a model implementation of the flexible functional split controller for 5G vRAN architectures.

The repository is split into an event driven simulator and an implementation of the theoretical model (to be added). 

*More detailed information will be added soon*

## Event-driven simulator

The simulator is implemented in `C++` and called from a `Python` script, although it can be directly executed from command line.

It has been only tested in `Ubuntu 20.04` with `C++17` standard enabled.

### Compilation and execution
```
make
```
```
./bin/main BUFFER_ENGINE_4 <NUM_PKTS> <SERV_DIST> <BUFF_SIZE> <SPLIT_PROBS> <SERV_TIME> <ARR_RATE> <GAMMA> <RES_HEAD> <RES_TAIL> <ITERS> <PKT_LOGS> <PROB_STATS> <PL_TT> <AVG_TIMES> <AVG_PL_TT>
```
#### Execution parameters description:
  + `NUM_PKTS`: number of packets
  + `SERV_DIST`: type of service time so far `CONST` (constant) or `EXP` (exponentially distributed)
  + `BUFF_SIZE`: buffer size in number of packets
  + `SPLIT_PROBS`: list with probabilities of the splits
  + `SERV_TIME`: list with mean service rate of each split (inverse of the service time)
  + `ARR_RATE`: mean value of the arrival rate (inverse of the inter-arrival-time)
  + `STOP_RATE`: mean value value of the stop rate (split shift rate)
  + `RES_HEAD `: string to add at the beginning of the results files 
  + `RES_TAIL`: string to add at the end of the results files 
  + `ITERS`: number of iterations
  + Parameters to handle results generation:
    + `PKT_LOGS`: `1` to generate packet log in each iteration, `0` otherwise
    + `PROB_STATS`: `1` to generate states probability in each iteration, `0` otherwise
    + `PL_TT`: : `1` to generate loss probability and total time in each iteration, `0` otherwise
    + `AVG_TIMES`: `1` to generate average packet times in each generation, `0` otherwise
    + `AVG_PL_TT`: `1` to generate average loss probability and total in each generation, `0` otherwise

### Results format:
  + `PKT_LOGS`: `1` file per iteration. 
    + File name (`curr_iter` is the current iteration index): `<RES_HEAD>pktsLog_<ARR_RATE>_<BUFF_SIZE>_<curr_iter><RES_TAIL>`
    + Row format:
      ```
      Total Time | Queue Time | Server Time 
      ```
  + `PROB_STATS`: `1` file per iteration. 
    + File name (`curr_iter` is the current iteration index): `<RES_HEAD>prob_states_<ARR_RATE>_<BUFF_SIZE>_<curr_iter><RES_TAIL>`
    + One row per packet with 3 columns:
      ```
      State (Pkts. in system) | Probability
      ```
  + `PL_TT`: : `1` file for all iterations. 
    + File name: `<RES_HEAD>ploss_vs_ttotal_<ARR_RATE>_<BUFF_SIZE><RES_TAIL>`
    + Row format:
      ```
      Avg. Total time | Loss Probability
      ```
  + `AVG_TIMES`: `1` file for all iterations. 
    + File name: `<RES_HEAD>simTimes_<BUFF_SIZE><RES_TAIL>`
    + Row format: 
      ```
      Avg. Total Time | Avg. Queue Time | Avg. Server Time | Avg. occupation ratio | Avg. loss Probability
      ```
  + `AVG_PL_TT`: `1` file for all iterations. 
    + File name: `<RES_HEAD>ploss_vs_ttotal_<ARR_RATE><RES_TAIL>`
    + Row format:
      ```
      Avg. Total Time | Avg. loss probability
      ```

## Model implementation
*Matlab theoretical model implementation coming soon!!*

## Contact
Luis Diez;  [email](mailto:ldiez@tlmat.unican.es). [University of Cantabria](https://web.unican.es/), [Communications Engineering Department](https://www.tlmat.unican.es/).

Ramón Agüero; [email](mailto:ramon@tlmat.unican.es). [University of Cantabria](https://web.unican.es/), [Communications Engineering Department](https://www.tlmat.unican.es/).