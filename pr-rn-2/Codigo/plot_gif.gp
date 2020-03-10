set terminal gif animate delay 3 size 1000,900  enhanced font 'Verdana,26'
set output '../Graficos/current_15_in.gif'

neuron1='./ejercicio_2_1_current_15_in.txt'
neuron2='./ejercicio_2_2_current_15_in.txt'

set xlabel "Tiempo [ms]"

do for [i=1:100]{
	gsyn = 2.0*i/100.0

set key horizontal


		set multiplot layout 2,1
    		unset title
    		set ytics 10
    		set xtics format "%.f"
    		set xlabel "Tiempo [ms]"
    		set yrange [0:45]
    		set ylabel "Corriente [{/Symbol m}A]"
    		set origin 0,0.0
    		set size 1,0.45
    		plot neuron1 index (i-1)  u 1:6  w l dt 0 lw 2   lc rgb "red" tit "N-1", \
    			 neuron2 index (i-1)  u 1:($6+10)  w l dt 3 lw 2   lc rgb "blue" tit "N-2 + 10 {/Symbol m}A "

			set xtics format ''
			unset xlabel
			set ylabel "Voltaje [mV]"
			set title sprintf("Valor de g_{syn}= %2.2f [mS/cm^2]",gsyn)# at 20,85 # font "arialbd,18"
			set autoscale
			set origin 0,0.4
			set size 1.0,0.6
			    		set ytics 20
    		plot neuron1 index (i-1)  u 1:2  w l dt 0 lw 2   lc rgb "red" tit "N-1", \
    			 neuron2 index (i-1)  u 1:($2+25)  w l dt 3 lw 2   lc rgb "blue" tit "N-2 + 25 mV"

    
    	unset multiplot
}