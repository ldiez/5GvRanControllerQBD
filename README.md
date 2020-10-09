# 5G vRAN Controller model based on Quasi-Birth-Death (QBD) process

This repository contains a model implementation of the flexible functional split controller for 5G vRAN architectures.

The repository is split into an event driven simulator and an implementation of the theoretical model. 

*More detailed information will be added soon, once an ongoing paper is finished*

## [QBD model](./QBD_model)

The theoretical model is implemented in Matlab. The Matlab code solves the underlying Markov Chain (QBD process).

The Matlab code can be found in the folder [QBD_model](./QBD_model)


## [Event-driven simulator](./Ev_simulator)

The simulator is implemented in `C++` and called from a `Python` script, although it can be directly executed from command line.

It has been only tested in `Ubuntu 20.04` with `C++17` standard enabled.

The simulator source code and scripts can be found in the folder [./Ev_simulator](./Ev_simulator) 


# Contact
Luis Diez;  [email](mailto:ldiez@tlmat.unican.es). [University of Cantabria](https://web.unican.es/), [Communications Engineering Department](https://www.tlmat.unican.es/).

Ramón Agüero; [email](mailto:ramon@tlmat.unican.es). [University of Cantabria](https://web.unican.es/), [Communications Engineering Department](https://www.tlmat.unican.es/).