% Script to obtain the total time at the controller for the unbounded model. 
% It uses various split configurations (alpha and mu)
% and it increases lambda from lmin to various lmax. 
% It also uses various and gamma values
%
% It was used to obtain Fig. 4 (a) of the paper

clc;
clear;

lmin=0.1;
lpoints=100;

xi = 1;

mu_cell = {[1 4],[1 1.5 3 5]};
alpha_cell = {[0.5 0.5],[0.75 0.25],[0.25 0.75];
              [0.25 0.25 0.25 0.25],[0.4 0.3 0.2 0.1],[0.1 0.2 0.3 0.4]};
gamma_vec = [0.2 0.1 0.05];
file_name_aux = ['a' 'b' 'c' 'd' 'e' 'f' 'g' 'h' 'i'];
lmax = [1.8 1.2 2];

mp = 800;
epsilon=1e-5;

for a=1:2
    mu = mu_cell{a};
    d = 1;
    for b=1:length(gamma_vec)
        gamma = gamma_vec(b);
        for c=1:3
            alpha = alpha_cell{a,c};
            lambda_vec=linspace(lmin,lmax(c),lpoints);
            times = zeros(3,lpoints);
            for j=1:lpoints
                [pi_t,max_col,R] = prob_unbounded_controller(lambda_vec(j),mu,gamma,xi,alpha,mp,epsilon);
                avg_n = norm(pi_t(:,1)'*R*(eye(length(mu)+1)-R)^(-2),1);
                times(1,j) = avg_n/lambda_vec(j);

                avg_w_aux = sum((0:max_col-2).*sum(pi_t(2:end,2:end),1)) + sum((1:max_col-1).*pi_t(1,2:end));
                times(2,j) = avg_w_aux/lambda_vec(j);

                avg_s = sum(pi_t(2:end,2:end),'all');
                times(3,j) = avg_s/lambda_vec(j);
            end
            file_name = strcat('results-unbounded-model/times_',int2str(a*2),file_name_aux(d),'.dat');
            dlmwrite(file_name,[lambda_vec' times'],' ');  
            d=d+1;
        end
    end
end