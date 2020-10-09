% Script to obtain the loss probability for the bounded model. 
% It uses various split configurations (alpha and mu)
% and it increases lambda from lmin to lmax. 
% It also uses various gamma values and buffer lengths
%
% It was used to obtain Fig. 5 of the paper

clc;
clear;

lmin=0.1;
lmax=2;
lpoints=100;

buff_ = [2 4 8 16 32];
xi = 1;
lambda_vec = linspace(lmin,lmax,lpoints);

mu_cell = {[1 4],[1 1.5 3 5]};
alpha_cell = {[0.5 0.5],[0.75 0.25],[0.25 0.75];
              [0.25 0.25 0.25 0.25],[0.4 0.3 0.2 0.1],[0.1 0.2 0.3 0.4]};
gamma_vec = [0.2 0.1 0.05];
file_name_aux = ['a' 'b' 'c' 'd' 'e' 'f' 'g' 'h' 'i'];

for a=1:2
    mu = mu_cell{a};
    d = 1;
    for b=1:length(gamma_vec)
        gamma = gamma_vec(b);
        for c=1:3
            alpha = alpha_cell{a,c};
            ploss = zeros(length(buff_),lpoints);
            for i=1:length(buff_)
                for j=1:lpoints
                    xi_ = prob_bounded_controller(lambda_vec(j),mu,gamma,xi,alpha,buff_(i));
                    ploss(i,j) = norm(xi_(end,:),1);
                end
            end
            file_name = strcat('results-bounded-model/ploss_',int2str(a*2),file_name_aux(d),'.dat');
            dlmwrite(file_name,[lambda_vec' ploss'],' ');  
            d=d+1;
        end
    end
end    