function plotdata(data)

hold on
plot(data(1,1), data(1,2), 'bo');
plot(data(2,1), data(2,2), 'go');
% plot(data(3:end,1), data(3:end,2), 'bs','MarkerSize',5);

for i=3:size(data,1)
    s = square(data(i,1), data(i,2));
    patch(s(1,:), s(2,:),'b');
end