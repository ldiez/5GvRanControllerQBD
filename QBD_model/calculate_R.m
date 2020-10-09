function [R,iter,timeElapsed] = calculate_R(L,B,F,epsilon)
% CALCULATE_R Calculates R using an iterative method
%
%   Based on transition matrices: L, B, F and the 
%	stopping criteria parameter, epsilon, it yields
%	the R matrix, for the unbounded model.
%
%	The function also gives the number of
%	iterations and time it requires to reach the solution

tic;
Raux_ = zeros(size(L));
dif = 1;
iter=0;
while(dif > epsilon)
    R = -1*(Raux_^2*B+F)*L^(-1);
    dif_aux = abs(R-Raux_);
    dif = max(dif_aux(:));
    Raux_ = R;
    iter=iter+1;
end
timeElapsed=toc;
