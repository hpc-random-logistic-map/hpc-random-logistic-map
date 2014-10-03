% plot the random cobweb
clear; clc; close all

L = 0.25;
global N
N = 10/0.1;
r = 3.5;
iter = 100;
x0 = linspace(0,1,iter);
[a,b] = myrand(L,N,r);
[xv,t,mymap]=cobweb(x0, iter, a, b,r);

% plot the map function
close all
plot(t,mymap)
% ,'--ks',...
%     'LineWidth',0.5,...
%     'MarkerSize',3,...
%     'MarkerEdgeColor','b',...
%     'MarkerFaceColor',[0.5,0.5,0.5])
hold on
axis('square'); axis([0 1 0 1]);
set(gca,'XTick',(0:0.1:1),'YTick',(0:0.1:1))
% grid on;

% % plot the line y = x
fplot('1*y',[0 1],'r');
%
% % plot cobweb
% line([x(1) x(1)],[0 x(2)],'Color','g')
% plot(x(1), x(1),'k*');
% inv_dens = zeros(iter-1,1);
% for ic = 1:iter-1
%     line([x(ic) x(ic+1)],[x(ic+1) x(ic+1)],'Color','g')
%     line([x(ic+1) x(ic+1)],[x(ic+1) x(ic+2)],'Color','g')
%     plot(x(ic+1), x(ic+1),'k*');
%     inv_dens(ic) = x(ic+1);
% end
% line([x(iter) x(iter+1)],[x(iter+1) x(iter+1)],'Color','g')
%
% % Add comments to plot
% % at = text(0.1,0.82,['R_0 = ',num2str(R0)]); set(at,'FontSize',12);
% % title(['R_0 = ',num2str(R0), ', x_0 = ',num2str(x0), ', N = ',num2str(N)])
xlabel('x_n')
ylabel('x_{n+1}')