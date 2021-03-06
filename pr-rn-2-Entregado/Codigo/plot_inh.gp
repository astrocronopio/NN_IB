set terminal qt size 1000,900  enhanced font 'Verdana,26'

I="15_in"


#set output "../Graficos/current_".I.".gif"
#set output "../Graficos/current_".I.".png"
neuron1="ejercicio_2_1_current_".I.".txt"
neuron2="./ejercicio_2_2_current_".I.".txt"

set xlabel "Tiempo [ms]"

set xrange [500:600]

i=10
    gsyn = 2.0*i/100.0

    set key horizontal
	set multiplot layout 2,1
    		unset title
    		set ytics 15
    		set xtics format "%.f"
    		set xlabel "Tiempo [ms]"
    		set yrange [0:58]
    		set ylabel "Corriente [{/Symbol m}A]"
    		set origin 0,0.0
    		set size 1,0.45
            set xrange [500:600]
    		plot neuron1 index (i-1)  u 1:6  w l dt 0 lw 2   lc rgb "red" tit "N-1", \
    			 neuron2 index (i-1)  u 1:($6-5)  w l dt 3 lw 2   lc rgb "blue" tit "N-2 - 5 {/Symbol m}A "

			set xtics format ''
			unset xlabel
			set ylabel "Voltaje [mV]"
			set title sprintf("g_{syn}= %2.2f [mS/cm^2] para I_0=15 [{/Symbol m}A]. Inhibitorio",gsyn)# at 20,85 # font "arialbd,18"
			set autoscale
            set xrange [500:600]
            set yrange [:50]
			set origin 0,0.4
			set size 1.0,0.6
			set ytics 20
    		plot neuron1 index (i-1)  u 1:2  w l dt 0 lw 2   lc rgb "red" tit "N-1", \
    			 neuron2 index (i-1)  u 1:($2+5)  w l dt 3 lw 2   lc rgb "blue" tit "N-2 + 5 mV"

     unset multiplot