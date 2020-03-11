set term qt 1 size 1200,800 enhanced font 'Verdana,26'

I="15_in"

out_file="../Graficos/current_".I.".png"
file_current="./gsyn_T_desfase_ej_2_current_".I.".txt"

###############################################################

set title "Frecuencia y Desfasaje en función de g_{syn}"

set ytics nomirror tc lt 1
set y2tics nomirror tc lt 2

set xlabel "g_{syn} [mS/cm^2]"
set ylabel " □ Frecuencia  [Hz]"  tc lt 1
set y2label "∇ Desfase [ms]" tc lt 2

plot file_current u 1:($2>0? 1000/$2 : 1/0) w lp pt 4 ps 1.5 notit ,\
	 file_current u 1:3         w lp pt 10 ps 2  notit axes x1y2


set term png 1 size 1200,800 enhanced font 'Verdana,26'
set output out_file
replot


set output
pause(2)