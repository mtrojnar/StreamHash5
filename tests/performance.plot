# set terminal png truecolor notransparent linewidth 3 large size 1000,1000
set terminal svg lw 5 size 1000,500 font 'Helvetica,24'
set output "performance.svg"
set xlabel "input size [bytes]"
set ylabel "execution time [CPU cycles per byte]"
set logscale x 2
set logscale y 10
set grid
set yrange [.1:1000]
set format x "%.0f"

set style data line

plot "performance.dat" using 1:2 title "StreamHash5",\
     "performance.dat" using 1:3 title "SHA-256",\
     "performance.dat" using 1:4 title "SHA-512"
