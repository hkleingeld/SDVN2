clear all
low = csvread('coutsamplerate.txt');

%high = f(:,1);
%low = f(:,2);

hold on
%plot(high)
plot(low)

lowFilterd = conv(low,[0.25 0.25 0.25 0.25]);


b = [0.1 0.1 0.1 0.1 0.1 0.1 0.1 0.1 0.1 0.1]
hold off
llow = low(90000:100000) - mean(low(90000:100000))
plot(abs(fft(llow)))
