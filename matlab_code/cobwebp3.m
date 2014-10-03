function [ oa ] = cobwebp3( yv2s, xv, av, bv, iter )
Rp3 = zeros(iter,1);
for j = 1:length(Rp3)
    Rp3(j) = R(yv2s(j),av,bv);
end
mymap = Rp3'.*(yv2s.*(1-yv2s));
h = figure;
movegui(h, 'southeast')
plot(xv,mymap,'c')
hold on
axis('square'); axis([0 1 0 1]);
set(gca,'XTick',(0:0.1:1),'YTick',(0:0.1:1))
fplot('1*y',[0 1],'r');
xlabel('x')
ylabel('P(P(P(x)))')
title('Period 3: P(P(P(x)))')
end

