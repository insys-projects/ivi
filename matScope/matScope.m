%% Reading Waveforms from an oscilloscope using a Quick-Control Oscilloscope Object

% Check that the software is properly installed.
instrhwinfo('ivi')

myscope = oscilloscope()
drivers = getDrivers(myscope)
availableResources = getResources(myscope)
myscope.Resource = 'PXI2::0::INSTR'
myscope.Driver = 'itScope'
%myscope.Resource = 'TCPIP0::MDO4104B-3-05V1XY.local.::inst0::INSTR'
%myscope.Driver = 'tkdpo4k'
connect(myscope)

autoSetup(myscope)
VC = getVerticalCoupling (myscope, 'CH1')
VR = getVerticalRange (myscope, 'CH1')
AT = get(myscope, 'AcquisitionTime');
sample_time = double(myscope.AcquisitionTime)/double(myscope.WaveformLength);
X = double(zeros(1,myscope.WaveformLength));
for i = 1:1:myscope.WaveformLength
    X(i) = sample_time*double(i);
end

for i = 1:1:200
    waveformArray = getWaveform(myscope);
    subplot(2,1,1); 
    cla
    hold on
        plot(X(1:512),waveformArray(1:512));
        %xlabel('Samples');
        xlabel('Time');
        ylabel('Voltage');
        grid on;
    hold off
   
    % ----- calculate spectrum
    chan = 1;
    size = myscope.WaveformLength;
    Fs = 1.0/sample_time;
    Z = zeros(1,size);  % alloc array
    for i=1:1:chan
        Y = double(zeros(i,size));
        Pyy = double(zeros(i,size));
        w = blackman(size);%задание окна
        Y(i,:)=(w'.*waveformArray);%перемножение сигналов
        Y(i,:) = fft(Y(i,:));
        Pyy(i,:) = Y(i,:).*conj(Y(i,:))/double(size*size);
    end
    for i=1:size/2
        f(i) = double(i)*(Fs/double(size));
    end
    %f = 1:(size/2);
    subplot(2,1,2); 
    for i=1:1:chan
        b = max(Pyy(i,:));
        P = double(zeros(i,size/2));
        P(i,:) = 10.0*log10(Pyy(i,1:(size/2))/b);
    end
    %----- plot spectrum
    cla
    hold on
    for i=1:1:chan
        %plot(f,P(i,:));
        plot(f(1:1024),P(i,(1:1024)));
        %plot(P(i,:));
        xlabel('Freq');
        ylabel('dB');
    end
    hold off
    grid on;
    
    pause(0.1);
end

