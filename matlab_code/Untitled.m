% tester

[u_orbits, ia, ic] = unique(orbits,'rows');
u_orbits = u_orbits(2:end,:);
num_periods = u_orbits(1:end,1);
y = zeros(maxp,2);
for w = 1:maxp
    y(w,1) = w;
    y(w,2) = sum(num_periods == w);
end