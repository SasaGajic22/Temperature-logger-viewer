set title"Temperature loger"
set xlabel"Time[s]"
set ylabel"Temperature[°C]"
set autoscale
plot 'log.txt' using 2 with lines
pause 1
reread
