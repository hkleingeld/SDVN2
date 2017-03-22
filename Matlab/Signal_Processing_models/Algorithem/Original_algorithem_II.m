%this file shows the original algorithem performing in a specific situation

%      /|         115cm
%      /| <----------------->
%      /|
%      /|                     Photo diode
%      /|                   \
% Wall /|                   | Light source (110 us on/8900 us off)
%      /|                   /
%      /|
%      /|
%

f = fread(fopen( '115cm_11.txt'),'uint16');

d = 300

gs = std(f(1:d))
gm = mean(f(1:d))
g  = f(d+100)

for n = 1:1:1600
   gs = [gs std(f(n:n+d))];
   gm = [gm mean(f(n:n+d))];
   g  = [g f(n+d+100)];
end

plot(gm+3*gs)
hold on
plot(gm-3*gs)
plot(g)

hold on