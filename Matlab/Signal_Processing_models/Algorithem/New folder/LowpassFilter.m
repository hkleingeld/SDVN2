
    % ADDME  Lowpassfilter with Data, Samplerate and -3DB point
    % (first order IIR Butterworth)

    %this file was used to desing and test the filter used in the dark sense
    %device
    f = [1:1:1000];

    % All frequency values are in Hz.
    Fs = 82;  % Sampling Frequency

    N  = 2;   % Order
    Fc = 10;  % Cutoff Frequency

    % Construct an FDESIGN object and call its BUTTER method.
    h  = fdesign.lowpass('N,F3dB', N, Fc, Fs);
    Hd = design(h, 'butter');
    a = coeffs(Hd);
    SOS = a.SOSMatrix;
    G = a.ScaleValues;

    [b, a] = tf(Hd);

    x = f;
    Start = 1;
    [Dc Stop] = size(f);
    W = [0 0 0];
    Wn = 0;
    Yn = 0;
    Y = 0;

    for n = Start+3:Stop
        Wn = x(n) - a(2) * W(n-1);
        W = [W Wn];

        Yn = b(1) *(W(n) + W(n-1));
        Y = [Y Yn];
    end

 