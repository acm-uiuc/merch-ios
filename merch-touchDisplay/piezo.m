clear; close all
piezo_values = csvread('piezo.csv')';

[lpfB,lpfA] = butter(1,[.35,.75]);
piezo_values_filtered(1,:) = abs(filter(lpfB, lpfA, piezo_values(1,:)));
piezo_values_filtered(2,:) = abs(filter(lpfB, lpfA, piezo_values(2,:)));
piezo_values_filtered(3,:) = abs(filter(lpfB, lpfA, piezo_values(3,:)));

figure;

subplot(2,1,1);
hold;
plot(piezo_values(1,:));
plot(piezo_values(2,:));
plot(piezo_values(3,:));
hold;

subplot(2,1,2);
hold;
plot(piezo_values_filtered(1,:));
plot(piezo_values_filtered(2,:));
plot(piezo_values_filtered(3,:));
hold;
%%

s = serial('/dev/cu.usbserial-A9007Poz');
s.Baudrate = 115200;
s.Terminator = 44;
pollCount = 12000;
piezo0 = zeros(1, pollCount);
piezo1 = zeros(1, pollCount);
piezo2 = zeros(1, pollCount);


fopen(s);
for i = 3:(pollCount+1)*3
    idx = floor(i/3);
    if mod(i,3) == 0
        piezo0(idx) = fscanf(s)
    elseif mod(i,3) == 1
        piezo1(idx) = fscanf(s)
    elseif mod(i,3) == 2
        piezo2(idx) = fscanf(s)
    end
end
fclose(s);
