set terminal qt 2 size 1000,750 enhanced font 'Arial,26'

set title "Ejercicio 5 - Hogdkin-Huxley"
set ylabel "Voltaje [mV]"
set xlabel "Tiempo [ms]"

set xrange [-200:]

plot "ejercicio_5.txt"  u 1:2  w l lw 3  t "Simulación" 

pause(-1)