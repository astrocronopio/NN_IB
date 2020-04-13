

set terminal qt 12 enhanced font 'Verdana,26' size 1200,750 

sigma=31.65

set auto
set xlabel "{/Symbol t} [ms]"
set ylabel "D({/Symbol t})"

set xtics 25


set xrange [-100:100]


plot 'filtro.dat' using (-$1/10):(0.06*($2-6100)/sigma) every 10 w lp lc rgb "violet" lw 3 notit 

pause(-1)

set terminal qt 3 enhanced font 'Verdana,26' size 1000,750 


set auto
set xlabel "t [ms]"
set ylabel "r(t) [Hz]"

set xtics 200


plot 'sum_spikes.dat' using ($1):($2/.01) w lp lc rgb "brown" lw 1 notit 

