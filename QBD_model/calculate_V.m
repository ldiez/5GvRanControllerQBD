function [V,iter,timeElapsed] = calculate_V(L,B,F,epsilon)
% CALCULATE_R Calculates V using an iterative method
%
%   Based on transition matrices: L, B, F and the 
%	stopping criteria parameter, epsilon, it yields
%	the V matrix, for the bounded model.
%
%	The function also gives the number of
%	iterations and time it requires to reach the solution

tic;
Vaux_ = zeros(size(L));
dif = 1;
iter=0;
while(dif > epsilon)
    V = -1*Vaux_*F*Vaux_*B*L^(-1)-L^(-1);
    dif_aux = abs(V-Vaux_);
    dif = max(dif_aux(:));
    Vaux_ = V;
    iter = iter+1;
end
timeElapsed=toc;
