
file_name = 'log_4.txt';
data_length = 1000;

p1 = 0.01;
d1 = 0.004;
i1 = 0.001;

p2 = 0.8;
i2 = 0.001;
d2 = 0.004;

s = importdata(file_name);
y = s(1: data_length);
x = 1:1:data_length;

plot(x,y);
xlabel('time');
ylabel('imuData');
title(['p1:' num2str(p1) 'd1:' num2str(d1) 'i1:' num2str(i1) 'p2:' num2str(p2) 'd2:' num2str(d2) 'i2:' num2str(i2)]);
savefig('file_name.fig');