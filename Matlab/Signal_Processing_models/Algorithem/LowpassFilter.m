%this file was used to desing and test the filter used in the dark sense
%device
f = csvread('floor+roomlight.txt');
plot(f);

% All frequency values are in Hz.
Fs = 75;  % Sampling Frequency

N  = 2;   % Order
Fc = 10;  % Cutoff Frequency

% Construct an FDESIGN object and call its BUTTER method.
h  = fdesign.lowpass('N,F3dB', N, Fc, Fs);
Hd = design(h, 'butter');

    for i = 1:1:2
        i
    x = f(Start:Stop,i);
    W = [512.5 512.5 512.5];
    Wn = 0;
    Yn = 116;
    Y = 0;
        for n = Start+3:Stop
            Wn = x(n) - a(2) * W(n-1);
            W = [W Wn];

            Yn = b(1) *(W(n) + W(n-1));
            Y = [Y Yn];
        end

        plot(Y)
        hold on
    end
 