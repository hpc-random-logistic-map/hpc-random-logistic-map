%plotting prob of orbits
clear
close all
clc
global tol
tol = 10e-6;
r = linspace(0+tol,4-tol,10);
kmax = 100; 
for k = 1:length(r)
    fname = strcat(strcat(strcat('prob_of_orbit_',num2str(kmax)),num2str(r(k))),'.csv')
    M = csvread(fname);
    figure
    plot(M(:,1),M(:,2),'--ks',...
        'LineWidth',0.5,...
        'MarkerSize',3,...
        'MarkerEdgeColor','b',...
        'MarkerFaceColor',[0.5,0.5,0.5])
    xlabel('Period Order')
    ylabel('Probability')
    title('Probability of Period Orders')
end
