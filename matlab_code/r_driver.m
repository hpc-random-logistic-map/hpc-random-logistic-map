%loop through values of r on (0,4) and check basin_size and orbit prob

clear 
clc
close all

global tol
tol = 10e-8;

% % prob of orbit
% % pick a value of r that has a long tail and do 10^6 iterates
% % 2 < r < 3
% L = linspace(0.2+tol,1-tol,20);
% r = linspace(1+tol,4-tol,20);
% r = 2.5;
% L = 0.1;
% % maxiters = 10e6;
% % for i = 1:maxiters  
% %     disp(i)
% %     disp(100*i/maxiters)
% 
% maxiter = 10e6 - 6519;
% for k = 4e5:6e5
%     disp(k)
%     disp(100*k/1e6)
%     prob_of_orbits(L,r,k)
% end
% % end

% % bifurcation diagram
r = linspace(3.75+tol,4-tol,500);
L = 0.1;

% Test = open('C:\Users\swamy\Documents\amy\thesis\code\figures\bifurcation_r500_x10_L0.1.fig');
Test = open('C:\Users\amy\Documents\code\bifurcation_r500_x10_L0.1.fig');
figure(Test)
hold on
axis([0 4 0 1])
axis manual

for j = 1:10
    disp(j)
    for i = 1:length(r)
        drawnow
        bifur(L,r(i))
    end
end
h = findall(gca,'marker','.');
set(h,'markersize',2)
savefig('bifurcation_2500_x10_L0.1.fig');
h2 = gcf;
print(h2, '-dpng', 'bifurcation_r500_x10_L0.1_v5.png')