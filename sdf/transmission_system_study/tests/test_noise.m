wl = 1.55e-6;
c = 299792458;
h = 6.626070040e-34;

bitPeriod = 1.0e-9;
samplesPerSymbol = 16;

n2 = 1e4;

P=c*h/(wl*bitPeriod/samplesPerSymbol);

fn_signal_name = 'signals\S9.sgn';


%yQ=read_signal_file(signal_filename(dirname,"S10")) / P /sqrt(n2)

full_signal = zeros(0);
N = 10;

for i=1:N
    fprintf('i = %i',i)
    % Nota: É preciso fazer um subdirectório 'signals' para criar os
    % ficheiros dos sinais.
    system('..\Debug\transmission_system_study.exe');
    yI=readSignal(fn_signal_name) / P /sqrt(n2);
    full_signal = [full_signal yI];
end

var(full_signal)