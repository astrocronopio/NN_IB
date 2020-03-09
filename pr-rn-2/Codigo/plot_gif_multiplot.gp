set terminal gif animate delay 100
set output "multiplot_animated.gif"

n = 50
dphi = 2*pi/n

do for [i=0:(n-1)] {
   phi = i*dphi
   set multiplot layout 2,1
      plot sin(x+phi)
      plot cos(x+phi)
   unset multiplot
}