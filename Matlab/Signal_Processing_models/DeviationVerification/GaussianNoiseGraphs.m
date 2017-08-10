%plots in this file show that the meassured noise is indeedly gaussian
f = csvread('TwoLightLevels.txt');

high = f(:,1);
low = f(:,2);

hold on
histogram(high)
histogram(low)