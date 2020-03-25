

set terminal qt 12 enhanced font 'Verdana,26' size 1200,750 

sigma=31.65

set auto
set xlabel "-{/Symbol t} [ms]"
set ylabel "D({/Symbol t})"

set xtics 25

set xrange [0:100]


plot 'filtro.dat' using ($1/10):($2/sigma) every 6 w lp lc rgb "violet" lw 3 tit "Cada 0.5 ms"
replot 'filtro.dat' using ($1/10):($2/sigma) every 12 w l lc rgb "brown" lw 3 tit "Cada 1 ms"
replot 'filtro.dat' using ($1/10):($2/sigma) every 12::6 w l lc rgb "brown" lw 3 notit 


#pause(-1)
binwidth_isi=10
bin(x,width)=width*floor(x/width)


N=14849



set terminal qt 10 enhanced font 'Verdana,26' size 1000,750 

set auto

set xrange [:30]
set yrange [:0.2]
set xtics 5
set xlabel "ISI [ms]"
set ylabel "P_{ISI}({/Symbol t})"
plot 'isi_dist.dat' using (bin($1,binwidth_isi)*0.1):(1.0/N) smooth freq with boxes lc rgb "brown"  lw 3 notit




set terminal qt 2 enhanced font 'Verdana,26' size 1000,750 


set auto
set xtics 10
set yrange [:0.2]

set xlabel "N"
set ylabel "P(N)"

binwidth_N=6

plot 'N_dist.dat' using (bin($1,binwidth_N)):(1.0/128) smooth freq with boxes lc rgb  "black"  lw 3 notit


set terminal qt 3 enhanced font 'Verdana,26' size 1000,750 


set auto
set xlabel "t [ms]"
set ylabel "r(t) [Hz]"

set xtics 200


plot 'sum_spikes.dat' using ($1):($2*100) w lp lc rgb "brown" lw 3 notit 




#
#set terminal qt 8 enhanced font 'Verdana,26' size 1500,500 
#
#
#set auto
#set xlabel "t [ms]"
#set ylabel ""
#unset ytics
#set x2tics nomirror
#unset x2tics
#
#set yrange [0:1.5]
#
#set xtics 200
#
#plot 'sum_spikes_por_celda.dat' using ($1/10):($2) w l lc rgb "blue" lw 3 notit 


pause(-1)

TMARGIN = "set tmargin at screen 0.97; set bmargin at screen 0.75"
BMARGIN = "set tmargin at screen 0.75; set bmargin at screen 0.20"
LMARGIN = "set lmargin at screen 0.15; set rmargin at screen 0.95"
RMARGIN = "set lmargin at screen 0.55; set rmargin at screen 0.95"

set terminal qt 7 enhanced font 'Verdana,26' size 1000,800

set auto


set multiplot layout 2,1 #enhanced font 'Verdana,26' size 1000,800

set xtics scale 0.0
set ytics scale 0.2

unset xtics

@TMARGIN; @LMARGIN

set ytics 10
set yrange [-18:]
set ylabel "S [dB]" offset 1.2
plot 'stimulus.dat' w l lw 1.2 notit 


unset ytics



set xrange [-10:]
set yrange [0:127]

set xtics 200
set ytics 16

set x2tics nomirror
unset x2tics
#unset xtics


set xlabel "t [ms]"
set ylabel "Prueba" offset 1.8

set xtics scale 0.2
set ytics scale 0.2

set bars small

 @BMARGIN; @LMARGIN

plot 'trans_spikes.dat' using ($0*0.1):($1>0? $1*0.5 +1 : 1/0):(0.25) w yerr  ps 0.01 pt 1 lc rgb "brown"  notit ,\
 for [i=2:128] 'trans_spikes.dat' using ($0*0.1):(column(i)>0? column(i)*0.5+i : 1/0):(0.25) w yerr  ps 0.01 pt 1  lc rgb "brown"  notit 



unset multiplot






set terminal qt 13 enhanced font 'Verdana,26' size 1200,750 

TMARGIN = "set tmargin at screen 0.97; set bmargin at screen 0.58"
BMARGIN = "set tmargin at screen 0.58; set bmargin at screen 0.20"
LMARGIN = "set lmargin at screen 0.15; set rmargin at screen 0.95"
RMARGIN = "set lmargin at screen 0.55; set rmargin at screen 0.95"

set ytics 1
set yrange [-0.8:4.3]
set xrange [0:100]
sigma=31.65

set multiplot layout 2,1 
set ylabel "D({/Symbol t})"
@TMARGIN; @LMARGIN

unset xtics
unset xlabel


plot 'filtro.dat' using ($1/10):($2/sigma)  every 1 w l lc rgb "black" lw 1 tit "Cada 0.1 ms",\



 @BMARGIN; @LMARGIN


set auto

set yrange [-0.6:4.3]
set xlabel "-{/Symbol t} [ms]"
set ylabel "D({/Symbol t})"

set xtics 20

set xrange [0:100]

set key horiz

plot 'filtro.dat' using ($1/10):($2/sigma)  every 2 w l lc rgb "gray" lw 3 tit "0.2 ms",\
	'filtro.dat' using ($1/10):($2/sigma) every 5 w lp lc rgb "pink" lw 3 tit "0.5 ms",\
 'filtro.dat' using ($1/10):($2/sigma) every 10 w l lc rgb "brown" lw 3 tit "1 ms",\
 'filtro.dat' using ($1/10):($2/sigma) every 10::5 w l lc rgb "brown" lw 3 notit 

unset multiplot