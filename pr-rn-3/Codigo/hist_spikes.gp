set terminal qt 13 enhanced font 'Verdana,26' size 1000,750 

sigma=31.649

set auto
set xlabel "-{/Symbol t} [ms]"
set ylabel "D({/Symbol t})"

set xtics 10

plot 'filtro.dat' using ($1/100):($2/sigma) w l lc rgb "brown" lw 3 notit 

pause(-1)



binwidth_isi=10
bin(x,width)=width*floor(x/width)
N=7488


set terminal qt 10 enhanced font 'Verdana,26' size 1000,750 

set auto

set xrange [:300]
set yrange [:0.2]
set xtics 100
set xlabel "ISI [ms]"
set ylabel "P_{ISI}({/Symbol t})"
plot 'isi_dist.dat' using (bin($1,binwidth_isi)):(1.0/N) smooth freq with boxes lc rgb "black"  lw 2 notit




set terminal qt 2 enhanced font 'Verdana,26' size 1000,750 


set auto
set xtics 10
set yrange [:0.2]

set xlabel "N"
set ylabel "P(N)"

binwidth_N=6

plot 'N_dist.dat' using (bin($1,binwidth_N)):(1.0/128) smooth freq with boxes lc rgb  "red"  lw 2 notit


set terminal qt 3 enhanced font 'Verdana,26' size 1000,750 


set auto
set xlabel "t [ms]"
set ylabel "r(t)"

set xtics 2000

plot 'sum_spikes.dat' using 1:2 w lp lc rgb "blue" lw 3 notit 



