% Script to obtain the states probabilities for the unbounded model. 
% It uses various split configurations (alpha and mu)
% and it calculates it for various lambda and gamma values
%
% It was used to obtain Fig. 3 (a) and (b) of the paper

clc;
clear;

xi = 1;

mu_cell = {[1 4],[1 1.5 3 5]};
alpha_cell = {[0.5 0.5],[0.75 0.25],[0.25 0.75];
              [0.25 0.25 0.25 0.25],[0.4 0.3 0.2 0.1],[0.1 0.2 0.3 0.4]};
gamma_vec = [0.2 0.1 0.05];
file_name_aux = ['a' 'b' 'c' 'd' 'e' 'f' 'g' 'h' 'i'];
lambda = [0.25 0.5 1 1.5];
aux_lambda = {'025','050','100','150'};

mp = 800;
epsilon=1e-6;

for a=1:2
    mu = mu_cell{a};
    d = 1;
    for b=1:length(gamma_vec)
        gamma = gamma_vec(b);
        for c=1:3
            alpha = alpha_cell{a,c};
            for j=1:length(lambda)
                [pi_t,max_col,R] = prob_unbounded_controller(lambda(j),mu,gamma,xi,alpha,mp,epsilon);
                file_name = strcat('results-unbounded-model/probs_',int2str(a*2),file_name_aux(d),'_lambda',aux_lambda{j},'.dat');
                dlmwrite(file_name,[(0:max_col-1)' pi_t'],' ');
            end
            d=d+1;
        end
    end
end