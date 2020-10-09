% Script to obtain the total time at the controller for the bounded model. 
% It uses various split configurations (alpha and mu)
% and it increases lambda from lmin to various lmax. 
% It also uses various gamma values and buffer lengths
%
% It was used to obtain Fig. 4 (b) and (c) of the paper

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
            ttotal = zeros(length(buff_),lpoints);
            for i=1:length(buff_)
                for j=1:lpoints
                    xi_ = prob_bounded_controller(lambda_vec(j),mu,gamma,xi,alpha,buff_(i));
                    
                    lambda_avg = 0;
                    for t=1:size(xi_,1)-1
                        lambda_avg = lambda_avg + norm(xi_(t,:),1)*lambda_vec(j);
                    end 

                    nt_avg = 0;
                    for t=2:size(xi_,1)
                        nt_avg=nt_avg+(t-1)*norm(xi_(t,:),1);
                    end
                    
                    ttotal(i,j) = nt_avg/lambda_avg;
                end
            end
            file_name = strcat('results-bounded-model/ttotal_',int2str(a*2),file_name_aux(d),'.dat');
            dlmwrite(file_name,[lambda_vec' ttotal'],' ');  
            d=d+1;
        end
    end
end    