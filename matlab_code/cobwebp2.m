function [ yv2 ] = cobwebp2( yv, xv, av, bv, iter )
Rp2 = zeros(iter,1);
for j = 1:length(Rp2)
    Rp2(j) = R(yv(j),av,bv);
end
mymap = Rp2'.*(yv.*(1-yv));
h = figure;
movegui(h, 'center')
plot(xv,mymap,'b')
hold on
axis('square'); axis([0 1 0 1]);
set(gca,'XTick',(0:0.1:1),'YTick',(0:0.1:1))
fplot('1*y',[0 1],'r');
xlabel('x')
ylabel('P(P(x))')
title('Period 2: P(P(x))')

yv2 = mymap;
end

