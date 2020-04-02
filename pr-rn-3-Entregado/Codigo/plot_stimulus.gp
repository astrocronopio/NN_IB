set term qt 1 size 1200,800 enhanced font 'Verdana,26'

set xlabel "Tiempo [ms]"
set ylabel "Intensidad [dB]"
plot "stimulus.dat" u 1:2  w l lw 2 tit "Estimulo"
