%plot basin_size.m output

clear
close all
clc

for k = 37:37
    fname = strcat(strcat(strcat('basin_size_',num2str(100)),num2str(k)),'.csv');
    M = csvread(fname);
    hold on
    plot(M(:,1),M(:,2),'rs','MarkerSize',10,...
        'MarkerEdgeColor','r',...
        'MarkerFaceColor',[1,0,1])
end
xlabel('period order')
ylabel('percentage of initial conditions drawn to orbit')
title('basin of attraction size')
axis([0 max(M(:,1)) 0 1])
set(gca,'XTick',[0:max(M(:,1))])