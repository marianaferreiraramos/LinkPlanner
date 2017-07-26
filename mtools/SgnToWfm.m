function [ data, symbolPeriod, samplingPeriod, type, numberOfSymbols,clockrate ] = SgnToWfm( fname_sgn,fname_wfm, nReadr )

[ data, symbolPeriod, samplingPeriod, type, numberOfSymbols ] = readSignal( fname_sgn, nReadr );
if (length(data)/4 ~= 0)
    data=data(1:length(data)-rem(length(data),4))
end
data=data/max(abs(data));
data=data';
sizeMark=size(data,1);
marker = false(sizeMark, 2);  % Initializing markers
SamplingRate=1/samplingPeriod;
clockrate=SamplingRate;

BuildTektronixAWG710xWFM(data,marker,clockrate,fname_wfm);



