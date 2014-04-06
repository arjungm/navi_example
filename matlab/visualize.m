fname = '../DataSets/set1_vis.txt';
data = dlmread(fname);
subplot(2,3,1)
plotdata(data);

fname = '../DataSets/set2_vis.txt';
data = dlmread(fname);
subplot(2,3,2)
plotdata(data);

fname = '../DataSets/set3_vis.txt';
data = dlmread(fname);
subplot(2,3,3)
plotdata(data);

fname = '../DataSets/set4_vis.txt';
data = dlmread(fname);
subplot(2,3,4)
plotdata(data);

fname = '../DataSets/set5_vis.txt';
data = dlmread(fname);
subplot(2,3,5)
plotdata(data);