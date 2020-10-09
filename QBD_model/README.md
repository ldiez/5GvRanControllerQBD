# QBD model

It consists of a set of Matlab scrips and functions. The code is mostly self-explanatory. Anyway, below we comment the functionality of each script (functions used by the scripts are not commented.)

## ploss_bounded_controller.m

Script to obtain the loss probability for the bounded model. 
It uses various split configurations (alpha and mu)
and it increases lambda from lmin to lmax. 
It also uses various gamma values and buffer lengths
*(It was used to obtain Fig. 5 of the paper)*

## ploss_ttotal_bounded_controller

Script to study the trade-off between total time at the controller and 
loss probability for the bounded model. 
It uses various split configurations (alpha and mu)
It also uses various lambda and gamma values 
It increases buffer length from buffmin to buffmax. 
*(It was used to obtain Fig. 6 of the paper)*

## prob_states_bounded_controller

Script to obtain the states probabilities for the bounded model. 
It uses various split configurations (alpha and mu)
and it calculates it for various lambda and gamma values
It also uses various buffer lengths
*(It was used to obtain Fig. 3 (c) and (d) of the paper)*


## prob_states_unbounded_model

Script to obtain the states probabilities for the unbounded model. 
It uses various split configurations (alpha and mu)
and it calculates it for various lambda and gamma values
*(It was used to obtain Fig. 3 (a) and (b) of the paper)*

## times_unbounded_model

Script to obtain the total time at the controller for the unbounded model. 
It uses various split configurations (alpha and mu)
and it increases lambda from lmin to various lmax. 
It also uses various and gamma values
*(It was used to obtain Fig. 4 (a) of the paper)*

## ttotal_bounded_controller

Script to obtain the total time at the controller for the bounded model. 
It uses various split configurations (alpha and mu)
and it increases lambda from lmin to various lmax. 
It also uses various gamma values and buffer lengths
*(It was used to obtain Fig. 4 (b) and (c) of the paper)*



# Contact
Luis Diez;  [email](mailto:ldiez@tlmat.unican.es). [University of Cantabria](https://web.unican.es/), [Communications Engineering Department](https://www.tlmat.unican.es/).

Ramón Agüero; [email](mailto:ramon@tlmat.unican.es). [University of Cantabria](https://web.unican.es/), [Communications Engineering Department](https://www.tlmat.unican.es/).