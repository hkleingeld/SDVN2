f = csvread('Paper_nothing_with_noise.txt');
plot(f(1:1500));

histogram(f(1:1500))

title('Histogram noise','FontSize',16)
ylabel('Nr of occurances','FontSize',16)
xlabel('RSS','FontSize',16)

mean(f(1:1500))
std(f(1:1500))