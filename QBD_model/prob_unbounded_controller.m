function [xi_t,max_column,R] = prob_unbounded_controller(lambda,mu,gamma,xi,alpha,max_packets,epsilon)
% PROB_UNBOUNDED_CONTROLLER Calculates the probability vectors for the infinite
% 							buffer controller
%
%   The input arguments are: 
%			- lambda: frame arrival rate (single variable)
%			- mu: service rate (vector: one per split configuration)
%			- gamma: change of split rate (single variable)
%			- xi: inverse of standby average duration (single variable)
%			- alpha: split configuration probability (vector: one per split configuration)
%			- max_packets: limit of the maximum # of packets that are admissible (single variable)
%			- epsilon: to consider a probability as negligible

dim_ = length(mu)+1;

F = lambda*eye(dim_);
B = diag([0 mu]);

L0 = -1*(F+diag([xi gamma*ones(1,dim_-1)]));
L0(2:end,1) = gamma;
L0(1,2:end) = xi*alpha;

L = L0-B;

[pi0,R]=matrix_analytic(L0,L,B,F,1e-8);

pi_t = zeros(dim_,max_packets);
pi_t(:,1) = pi0;
    
for i=2:max_packets
    pi_t(:,i) = pi0*R^(i-1);
    if((1-sum(pi_t,'all')) < epsilon)
        break;
    end
end
xi_t = pi_t(:,1:i);
max_column = i;