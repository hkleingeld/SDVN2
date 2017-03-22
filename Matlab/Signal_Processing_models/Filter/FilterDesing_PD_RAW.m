%this file was used to desing and test the filter used in the dark sense
%device

clear all

f = fread(fopen( '100cm_transistor.txt'),'uint16');
%f = [f fread(fopen( 'paper_40cm.txt'),'uint16')];
%f = [f fread(fopen( 'paper_60cm.txt'),'uint16')];
%f = [f fread(fopen( 'paper_80cm.txt'),'uint16')];
%f = [f fread(fopen( 'paper_100cm.txt'),'uint16')];
%f = [f fread(fopen( 'paper_120cm.txt'),'uint16')];
%f = [f fread(fopen( 'paper_140cm.txt'),'uint16')];
f = [f fread(fopen( '100cm_resistor.txt'),'uint16')];

%plot(f(:,1));

f_max = find(f == hex2dec('FFFF'));
f(f_max) = 119;
Start = 1;
Stop = 64080;
hold on
%plot(g(Start:Stop));
%hold on
%plot(f(Start:Stop));
IRR = 1;
for Fc = 10000:10000:10000
    Fc
    if IRR == 1
        %%%%%%%%%%%%%%%%%%%%filter desing%%%%%%%%%%%%%%%
        % All frequency values are in Hz.
        Fs = 250000;  % Sampling Frequency
        N  = 1;     % Order
        %Fc = 2500;  % Cutoff Frequency

        % Construct an FDESIGN object and call its BUTTER method.
        h  = fdesign.lowpass('N,F3dB', N, Fc, Fs);
        Hd = design(h, 'butter');

        % Get the transfer function values.
        [b, a] = tf(Hd);
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    else
        %%%%%%%%%%%%%%%%%%%%filter desing%%%%%%%%%%%%%%%
        Fs = 250000;  % Sampling Frequency

        N     = 2;        % Order
        %Fc    = 5000;     % Cutoff Frequency
        flag  = 'scale';  % Sampling Flag
        Alpha = 2.5;      % Window Parameter

        % Create the window vector for the design algorithm.
        win = gausswin(N+1, Alpha);

        % Calculate the coefficients using the FIR1 function.
        b  = fir1(N, Fc/(Fs/2), 'low', win, flag);
        a = [0 0 0] %stub a
        Hd = dfilt.dffir(b);
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    end

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
end
    