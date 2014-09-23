function analysis(modul)

load handel_.txt;
if(modul==16)
load QAM16/out14.txt;
elseif(modul==8)
load PSK8/out14.txt;
elseif(modul==2)
load BPSK/out14.txt;
end
diff = handel_ - out14


