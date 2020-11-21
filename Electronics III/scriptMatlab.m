%MATLA SCRIPT-SXEDIASI TELESTIKOU ENISXYTI
%NAPOLEON PAPOUTSAKIS
%AEM: 9170


%PRODIAGRAFES

% CL = 2.7 pF
% SR > 18.7 V/µs
% Vdd = 2.01 V
% vss = -2.01 V
% GB > 7.7 MHz
% A > 20.7 dB
% P < 50.7 mW


CL = 2.7 * 10^-12 
GBmin = 7.7 * 10^6 % Minimum GB
Vdd = 2.01
Vss = -2.01
minSR = 18.7 * 10^6
Pmax = 50.7 * 10^-3
Amin = 20.7

%1o BHMA
L = 1 * 10^-6 

%2o BHMA
Cc = 3 * 10^-12 % pf

%3o BHMA 
I5 = minSR * Cc

%4o BHMA
VINmax = 0.1
VINmin = -0.1
VToP = -0.9056
VToN = 0.786
kp = 60 * 10^-6
kn = 150 * 10^-6


s3 = I5/(kp * (Vdd - VINmax -abs(VToP) + VToN)^2)

% s3< 1 ara s3=1
s3 = 1
s4 = s3


%5o BHMA

Cox = 4.6 * 10^-3
ln = 0.04
lp = 0.05
I3 = I5/2
W3 = s3 * L
p3 = sqrt(2*kp*s3*I3)/(2*0.667*W3*L*Cox)
p3 > 10*GBmin * 2 *pi

%6o BHMA
gm1 = GBmin * 2*pi * Cc
gm2 = gm1
s1 = (gm1^2)/(kn*I5)
s2 = s1

%7o BHMA
b1 = kn * s1
Vds5 = VINmin - Vss - sqrt(I5/b1) - VToN
s5 = (2*I5)/(kn * (Vds5)^2)

%8o BHMA
I4 = I5/2
gm4 = sqrt(2*kp*s4*I4)
gm6 = 2.2 * gm2 * (CL/Cc)
s6 = s4 * (gm6/gm4)
I6 = (gm6^2)/(2*kp*s6)


%9o BHMA



%10o BHMA
s7 = (I6/I5)*s5

%11o BHMA
Av = (2*gm2*gm6)/(I5*(ln+lp)*I6*(ln+lp))
gain = 20*log10(Av)

gain > Amin

Pdiss = (I5+I6)*(Vdd + abs(Vss))

Pdiss < Pmax

W1 = s1 * L
W2 = s2 * L
W3 = s3 * L
W4 = s4 * L
W5 = s5 * L
W6 = s6 * L
W7 = s7 * L
W8 = W5




