function runSound(modul)
load handel;
y = y';
diffSqr = zeros(1,4);
if(modul == 2)
    cd BPSK;
    title = 'BPSK';
elseif(modul == 8) 
    cd PSK8;
    title = 'PSK8';
elseif(modul == 16) 
    cd QAM16;
    title = 'QAM16';
end;

load out1.txt
diffSqr(1) = process(out1,y);

load out2.txt;
diffSqr(2) = process(out2,y);

load out3.txt;
diffSqr(3) = process(out3,y);

load out4.txt;
diffSqr(4) = process(out4,y);
ber = [10^-1 10^-2 10^-3 10^-5];
cd ..;
drawfigure(ber,diffSqr,title,'BER','different square');


function y = process(x1,x2)        
   sound(x1);
   diff = x1-x2;
   y = diff*diff';
