function [av, bv] = myrand(L,N,r)

a = zeros(N,1);
b = zeros(N,1);

for j = 1:N
    Mn = sqrt(1.5*S(L,j,r));
    %     pd = makedist('Normal','mu',mu,'sigma',stddev);
    pd = makedist('Uniform','lower',-Mn,'upper',Mn);
    a(j) = random(pd);
    b(j) = random(pd);
end

av = a;
bv = b;

end