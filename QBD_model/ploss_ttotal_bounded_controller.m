% Script to study the trade-off between total time at the controller and 
% loss probability for the bounded model. 
% It uses various split configurations (alpha and mu)
% It also uses various lambda and gamma values 
% It increases buffer length from buffmin to buffmax. 
%
% It was used to obtain Fig. 6 of the paper

clc;
clear;

buffmin = 1;
buffmax = 100;
buffinterval = 1;

buff_ = buffmin:buffinterval:buffmax;

xi = 1;

mu_cell = {[1 4],[1 1.5 3 5]};
alpha_cell = {[0.5 0.5],[0.75 0.25],[0.25 0.75];
              [0.25 0.25 0.25 0.25],[0.4 0.3 0.2 0.1],[0.1 0.2 0.3 0.4]};
gamma_vec = [0.2 0.1 0.05];
lambda_vec = [0.5 1 1.5 2];
aux_lambda = {'050','100','150','200'};

file_name_aux = ['a' 'b' 'c' 'd' 'e' 'f' 'g' 'h' 'i'];

for a=1:2
    mu = mu_cell{a};
    d = 1;
    for b=1:length(gamma_vec)
        gamma = gamma_vec(b);
        for c=1:3
            alpha = alpha_cell{a,c};
            
            for j=1:length(lambda_vec)
                res_ = zeros(2,length(buff_));
                for i=1:length(buff_)
                    xi_ = prob_bounded_controller(lambda_vec(j),mu,gamma,xi,alpha,buff_(i));
                    res_(2,i) = norm(xi_(end,:),1);
                    
                    lambda_avg = 0;
                    for t=1:size(xi_,1)-1
                        lambda_avg = lambda_avg + norm(xi_(t,:),1)*lambda_vec(j);
                    end 

                    nt_avg = 0;
                    for t=2:size(xi_,1)
                        nt_avg=nt_avg+(t-1)*norm(xi_(t,:),1);
                    end
                    
                    res_(1,i) = nt_avg/lambda_avg;

          
                end
                file_name = strcat('results-bounded-model/ploss_vs_ttotal_',int2str(a*2),file_name_aux(d),'_lambda',aux_lambda{j},'.dat');
                dlmwrite(file_name,res_',' ');
            end
            d=d+1;
        end
    end
end    