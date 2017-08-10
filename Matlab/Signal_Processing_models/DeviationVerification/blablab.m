f = csvread('coutsamplerate.txt');

hold on

f = f-mean(f)

plot(abs(fft(f(1:5000))))