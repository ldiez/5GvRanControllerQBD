% Script to obtain the states probabilities for the bounded model. 
% It uses various split configurations (alpha and mu)
% and it calculates it for various lambda and gamma values
% It also uses various buffer lengths
%
% It was used to obtain Fig. 3 (c) and (d) of the paper

clc;
clear;

lmin=0.1;
lmax=2;
lpoints=100;

buff_ = [2 4 8 16 32];
xi = 1;

mu_cell = {[1 4],[1 1.5 3 5]};
alpha_cell = {[0.5 0.5],[0.75 0.25],[0.25 0.75];
              [0.25 0.25 0.25 0.25],[0.4 0.3 0.2 0.1],[0.1 0.2 0.3 0.4]};
gamma_vec = [0.2 0.1 0.05];
lambda_vec = [0.5 1 1.5];
aux_lambda = {'050','100','150'};

file_name_aux = ['a' 'b' 'c' 'd' 'e' 'f' 'g' 'h' 'i'];

for a=1:2
    mu = mu_cell{a};
    d = 1;
    for b=1:length(gamma_vec)
        gamma = gamma_vec(b);
        for c=1:3
            alpha = alpha_cell{a,c};
            ttotal = zeros(length(buff_),lpoints);
            for i=1:length(buff_)
                for j=1:length(lambda_vec)
                    xi_ = prob_bounded_controller(lambda_vec(j),mu,gamma,xi,alpha,buff_(i));
                    
                    file_name = strcat('results-bounded-model/probs_',int2str(a*2),file_name_aux(d),'_lambda',aux_lambda{j},'_b',int2str(buff_(i)),'.dat');
                    
                    dlmwrite(file_name,[(0:buff_(i)+1)' xi_],' ');
                end
            end
            d=d+1;
        end
    end
end    