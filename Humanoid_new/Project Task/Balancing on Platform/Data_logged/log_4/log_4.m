%%INPUT LOG_NUM AND DATA_LENGTH AND PID VALUES ONLY

% Log number
log_num = 4;
%length of data taken.It is number of rows of data.
data_length = 1000;

%%Enter PID values for left motor
p1 = 0.01;
d1 = 0.004;
i1 = 0.001;
%Enter PID values for right motor
p2 = 0.8;
i2 = 0.001;
d2 = 0.004;

file_name = ['log_' num2str(log_num) '.txt'];


%reading and storing data from file
s = importdata(file_name);
y = s(1: data_length);
x = 1:1:data_length;

%plotting data and saving it to a file.
plot(x,y);
xlabel('time');
ylabel('imuData');
title(['p1: ' num2str(p1) ' d1: ' num2str(d1) ' i1: ' num2str(i1) '     p2: ' num2str(p2) ' d2: ' num2str(d2) ' i2: ' num2str(i2)]);
savefig(['log_' num2str(log_num) '.fig']);