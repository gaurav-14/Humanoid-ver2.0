P_0 = 0.1 ;
P_1 = 0.1;
I = 0;
D = 0;

file_path = 'C:\Users\HP\Desktop\Humanoid\SWAYAT-ver2.0\Humanoid_new\Project Task\Balancing on Platform\Data_logged\log_4\log_4';


fid=fopen(file_path);
s=textscan(fid,'%d %d %d','headerlines',20);
fclose(fid);
x=s{1};
y=s{2};
z=s{3};

for i = 1:1:400
    
  plot(i,x(i),'--r.'); %IMU
  hold on;
  %plot(i,y(i),'--g.');%Motor_left
  %plot(i,z(i),'--b.');%Motor_right
 
    
end
title('PID: 0.08/0.1  0  0');
xlabel('time');
ylabel('Imu&MotorData');