function [xi_t] = prob_bounded_controller(lambda,mu,gamma,xi,alpha,buffer)
% PROB_BOUNDED_CONTROLLER Calculates the probability vectors for the finite
% 							buffer controller
%
%   The input arguments are: 
%			- lambda: frame arrival rate (single variable)
%			- mu: service rate (vector: one per split configuration)
%			- gamma: change of split rate (single variable)
%			- xi: inverse of standby average duration (single variable)
%			- alpha: split configuration probability (vector: one per split configuration)
%			- buffer: buffer length (single variable)

dim_ = length(mu)+1;

F = lambda*eye(dim_);

B = diag([0 mu]);

L0 = -1*(F+diag([xi gamma*ones(1,dim_-1)]));
L0(2:end,1) = gamma;
L0(1,2:end) = xi*alpha;

Lk = -1*(B+diag([xi gamma*ones(1,dim_-1)]));
Lk(2:end,1) = gamma;
Lk(1,2:end) = xi*alpha;

L = L0-B;

[V,iter]=calculate_V(L,B,F,1e-8);
R = F*V;
G = V*B;

[Vhat,iter] = calculate_V(L,F,B,1e-8);
Rhat = B*Vhat;
Ghat = Vhat*F;

Aux1 = zeros(buffer*dim_,2*dim_);

Aux1(1:dim_,1:dim_) = eye(dim_);
Aux1(1:dim_,dim_+1:end) = Ghat^(buffer-1);
for i=1:buffer-2
    Aux1(i*dim_+1:(i+1)*dim_,1:dim_) = G^i;
    Aux1(i*dim_+1:(i+1)*dim_,dim_+1:end) = Ghat^(buffer-1-i);
end
i = buffer-1;
Aux1(i*dim_+1:(i+1)*dim_,1:dim_) = G^i;
Aux1(i*dim_+1:(i+1)*dim_,dim_+1:end) = eye(dim_);

Aux2 = [L+F*G -B*Ghat^buffer ; -F*G^buffer L+B*Ghat];

Ncol=-Aux1*Aux2^(-1);

Aux3 = [L+R*B -R^buffer*B ; -Rhat^buffer*F Rhat*F+L];
Aux4 = zeros(2*dim_,buffer*dim_);

Aux4(1:dim_,1:dim_) = eye(dim_);
Aux4(dim_+1:end,1:dim_) = Rhat^(buffer-1);
for i=1:buffer-2
    Aux4(1:dim_,i*dim_+1:(i+1)*dim_) = R^i;
    Aux4(dim_+1:end,i*dim_+1:(i+1)*dim_) = Rhat^(buffer-1-i);
end
i = buffer-1;
Aux4(1:dim_,i*dim_+1:(i+1)*dim_) = R^i;
Aux4(dim_+1:end,i*dim_+1:(i+1)*dim_) = eye(dim_);

Nrow=-Aux3^(-1)*Aux4;

N11 = Ncol(1:dim_,1:dim_);
N1m = Ncol(1:dim_,dim_+1:end);
Nm1 = Ncol(end-dim_+1:end,1:dim_);
Nmm = Ncol(end-dim_+1:end,dim_+1:end);

Aux_fin = [L0 + F*N11*B F*N1m*F ; B*Nm1*B Lk + B*Nmm*F];
    [V,D,W] = eig(Aux_fin);

    mineigvalue=min(abs(diag(D)));
    index_aux = find(abs(diag(D)) == mineigvalue);
    if(mineigvalue<1e-4)
        left_eigvector = W(:,index_aux)';
    end

pi_t = zeros(buffer+2,dim_);
pi_t(1,:)=left_eigvector(1:dim_);
pi_t(end,:)=left_eigvector(dim_+1:end);
for j=1:buffer
    N1j=Nrow(1:dim_,(j-1)*dim_+1:j*dim_);
    Nmj=Nrow(dim_+1:end,(j-1)*dim_+1:j*dim_);
    pi_t(j+1,:) = pi_t(1,:)*F*N1j+pi_t(end,:)*B*Nmj;
end
xi_t = pi_t/sum(pi_t,'all');