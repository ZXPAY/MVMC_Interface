clc, clear
R = 10e3;
% c1 = 0.528e-6;
% c2 = 1.09e-6;
% c3 = 0.048e-6;
% c4 = 1.51e-6;
% c5 = 0.0182e-6;

c1 = 0.48e-6;
c2 = 1.2e-6;
c3 = 0.048e-6;
c4 = 1.25e-6;
c5 = 0.0182e-6;


num = [c2]
den = [2*c1*c2*c4*c5*R^3 (3*c1*c2*c5*R^2+2*c1*c4*c5*R^2+3*c2*c4*c5*R^2) (c1*c2*R+2*c1*c5*R+4*c2*c5*R+3*c4*c5*R) (c2+3*c5)]

G = tf(num, den)

poles = roots(den)
figure();
rlocus(G);
figure();
bode(G);
figure();
step(G);






