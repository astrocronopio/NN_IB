set term png 1 size 1200,800 enhanced font 'Verdana,26'

out_file="../Graficos/current_15_in.png"
file_current="./gsyn_T_desfase_ej_2_current_15_in.txt"

###############################################################
set output out_file
set title "Frecuencia y Desfaseje en función de g_{syn}"

set ytics nomirror tc lt 1
set y2tics nomirror tc lt 2

set xlabel "g_{syn} [mS/cm^2]"
set ylabel " □ Frecuencia  [Hz]"  tc lt 1
set y2label "∇ Desfase [ms]" tc lt 2

plot file_current u 1:(1000/$2) w lp pt 4 ps 1.5 notit ,\
	 file_current u 1:3         w lp pt 10 ps 2  notit axes x1y2



#pause(-1)