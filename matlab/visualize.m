
fname = '../DataSets/set1_vis.txt';
sname = '../DataSets/set1_sol.txt';
iname = '../DataSets/set1_sol.png';

for i=[1 2 3 5]

f = figure('visible','off');

fname(16) = num2str(i);
env = dlmread(fname);
plotdata(env);

sname(16) = num2str(i);
sol = dlmread(sname);
plot(sol(:,1), sol(:,2), 'r');

iname(16) = num2str(i);
saveas(f, iname);

disp(i)
close all
end