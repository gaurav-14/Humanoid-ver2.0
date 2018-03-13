
file_name = 'log_4.txt';
data_length = 1000;

s = importdata(file_name);
y = s(1: data_length);
x = 1:1:data_length;
plot(x,y);
xlabel('time');
ylabel('imuData');
title('pidValues');