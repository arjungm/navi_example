fname = '../DataSets/set1.dat';
fid = fopen(fname);
raw = fread(fid,inf);
str = char(raw');
fclose(fid);

nstr = str(16:end-52);
xy = strread(nstr, '%d','delimiter','], [');
xs = 2:4:length(xy);
ys = 3:4:length(xy);

plot(x,y,'b.')