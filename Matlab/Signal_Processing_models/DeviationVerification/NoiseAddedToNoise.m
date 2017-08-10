% clear all
% f = csvread('rawdata_switcher.txt');
% f = f(:,1);
% 
% N = size(f);
% 
% 
% g = f(1:2:N)
% f = f(2:2:N)
% h = f-g;
x = 10;
y = 5;
a = normrnd(1,x,1,10000000);
b = normrnd(2,y,1,10000000);
c = a+b;
std(c) - sqrt(x*x+y*y)
mean(c)