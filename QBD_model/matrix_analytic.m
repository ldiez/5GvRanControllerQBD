function [pi0,R] = matrix_analytic(L0,L,B,F,epsilon)
% MATRIX_ANALYTIC Calculates the probability vector for the 0th level for the infinite
% 							buffer controller
%
%   The input arguments are the transition matrices (L0, L, B, F)
%	and the stopping criteria (epsilon)
%	
% 	The function also returns the R matrix

R = calculate_R(L,B,F,epsilon);
Phi = L0+R*B;
Omega = (eye(size(L))-R)^(-1)*ones(size(L,1),1);
coef_aux = [Omega Phi(:,2:end)];
pi0 = [1 zeros(1,size(L,1)-1)]*coef_aux^-1;
end

