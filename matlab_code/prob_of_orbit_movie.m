%making a movie

clear
clc
close all

global tol

nFrames = 10;
figure('Renderer','zbuffer')
set(gca,'NextPlot','replaceChildren');
% cMovie(nFrames) = struct('cdata',[],'colormap',[]);
frameind = 1;
tol = 10e-6;
r = linspace(0+tol,4-tol,10);
kmax = 100; 

% for k = 1:length(r)
%     fname = strcat(strcat(strcat('prob_of_orbit_',num2str(kmax)),num2str(r(k))),'.csv')
%     M = csvread(fname);
%     
%     set(gcf,'toolbar','none')
%     plot(M(:,1),M(:,2),'--ks',...
%         'LineWidth',0.5,...
%         'MarkerSize',3,...
%         'MarkerEdgeColor','b',...
%         'MarkerFaceColor',[0.5,0.5,0.5])
%     drawnow
%     xlabel('Period Order')
%     ylabel('Probability')
%     title('Probability of Period Orders')    
%     axis([0 20 0 1])
%     cMovie(k) = getframe(gcf);    
% %     frameind = frameind+1;    
% end
% 
% myVideo = VideoWriter('myProbOfOrbit.avi');
% myVideo.FrameRate = 1;  % Default 30
% myVideo.Quality = 75;    % Default 75
% open(myVideo);
% writeVideo(myVideo, cMovie);
% close(myVideo);



axis([0 20 0 1]) 
axis manual
for k = 1:length(r)
    fname = strcat(strcat(strcat('prob_of_orbit_',num2str(kmax)),num2str(r(k))),'.csv');
    M = csvread(fname);
    
%     set(gcf,'toolbar','none')
    plot(M(:,1),M(:,2),'--ks',...
        'LineWidth',0.5,...
        'MarkerSize',3,...
        'MarkerEdgeColor','b',...
        'MarkerFaceColor',[0.5,0.5,0.5])
    xlabel('Period Order')
    ylabel('Probability')       
    title('Probability of Period Orders')    
    % Compose and display two multiline text
    % annotations as cell arrays
    str1(1) = {strcat('r = ',num2str(r(k)))};
    text(16,0.8,str1)
    F(k) = getframe; 
%     frameind = frameind+1;    
end

movie(F,20,1) 