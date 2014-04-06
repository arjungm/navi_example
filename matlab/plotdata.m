function plotdata(data)

hold on
plot(data(1,1), data(1,2), 'ro');
plot(data(2,1), data(2,2), 'go');
plot(data(3:end,1), data(3:end,2), 'b.');