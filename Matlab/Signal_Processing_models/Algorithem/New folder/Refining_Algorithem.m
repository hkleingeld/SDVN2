function res = DeviationVSDelayedDeviation(data, SizeStd,Delay,SizeDev,Threshold)
f = data;

%f = j;
T = Threshold;
delay = Delay; %3 seconds
d = SizeStd; %samples in main std
e = SizeDev;  %samples in deleayed distribution

if e > 100
    e = 100
end

stdAtDetect = 0;

gs = std(f(1:d));
gm = mean(f(1:d))

hs = std(f(1:d))
hm = mean(f(1:d))
ga = std(f(1:d));
res = 0;
[dc N] = size(f)

for n = 2:1:N-(delay+d)
    ga = [ga std(f(n:n+d))];
    hm = [hm mean(f(n+d+delay-e+1:n+d+delay))];
    gs = [gs std(f(n:n+d))];
    gm = [gm mean(f(n:n+d))];
    
    if res(n-1) == 0
        stdAtDetect = gs(n);
        i = 0;
        %res = [res ((hm(n) < gm(n)- gm(n)*T) | (hm(n) > gm(n)+gm(n)*T))];
        res = [res ((hm(n) < gm(n)- gs(n)*T/sqrt(e)) | (hm(n) > gm(n)+gs(n)*T/sqrt(e)))];
    else
        i = i + 1;
        if i > d+delay
            if stdAtDetect > gs(n)
                res = [res 0];
            else
                res = [res 1];
                stdAtDetect = stdAtDetect + 0.01
            end
        else
            res = [res 1];
        end
    end
    
    %e = ceil((4*gs(n)/(T * gm(n)))^2+1);
    if e > 100
        e = 100
    end
end

%plot(gm+3*gs)
%hold on
%plot(gm+gs(n)*4/sqrt(e))
%plot(gm-3*gs)
%plot(gm-gs(n)*4/sqrt(e))
%plot(hm)
%plot(mean(f)+res*50)

end
