num = [3.313e07]
den = [1 4.123e06 1.404e08]
Ts = .1
Gs = tf(num, den);

Gz = c2d(Gs, Ts, 'zoh')
disp(Gz);

isstable(Gs)
step(Gs)
disp(stepinfo(Gs))
SP = 1
[y,t] =step(SP*Gs);
sse = abs(SP - y(end));
disp(sse)

pid = pidtune(Gs, 'pid');
disp(pid);

pidTuner(Gz, 'pid')