set terminal qt 0 size 1000,750 enhanced font 'Arial,26'

set title "Ejercicio 4 - Hogdkin-Huxley"
set ylabel "Frecuencia [khz]"
set xlabel "Corriente [{/Symbol m}A]"

#set xrange [-200:]
set key left top
plot "freq_current_ej_4.txt" index 0 u 1:($2 == 0 ? 0 : 1/$2)   w l lc rgb 'red' lw 3  t "Simulación" 
replot "freq_current_ej_4.txt" index 1 u 1:($2 == 0 ? 0 : 1/$2)   w l lc rgb 'blue' lw 3  notit 

pause(-1)

set terminal qt 2 size 1000,750 enhanced font 'Arial,26'

set title "Ejercicio 4 - Hogdkin-Huxley"
set ylabel "Voltaje [mV]"
set xlabel "Tiempo [ms]"

set xrange [-200:]

plot "ejercicio_4.txt"  u 1:2  w l lw 3  t "Simulación" 



