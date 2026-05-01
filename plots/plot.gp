# Script de gnuplot para los resultados de benchmarks.

set datafile separator ","
set terminal pdfcairo noenhanced font "Arial,16" size 16cm,9cm
set xlabel 'Cantidad de deportistas (n)'
set ylabel 'Tiempo promedio (s)'
set grid
set key outside
set border linewidth 2
set grid linewidth 1.5
set key font ",14"
set xlabel font ",16"
set ylabel font ",16"
set tics font ",13"

set style data lines
set key autotitle columnhead

searchFile = 'db/search_benchmark.csv'
sortFile   = 'db/sort_benchmark.csv'

searchOut      = 'plots/search_worst_benchmark.pdf'
sortBestOut    = 'plots/sort_best_benchmark.pdf'
sortAverageOut = 'plots/sort_average_benchmark.pdf'
sortWorstOut   = 'plots/sort_worst_benchmark.pdf'

hasSearch = int(system(sprintf("test -f '%s' && echo 1 || echo 0", searchFile)))
hasSort   = int(system(sprintf("test -f '%s' && echo 1 || echo 0", sortFile)))

if (!hasSearch) {
	print sprintf("ERROR: no existe %s", searchFile)
	exit 1
}
if (!hasSort) {
	print sprintf("ERROR: no existe %s", sortFile)
	exit 1
}

set autoscale
set xtics
set ytics
set key outside

# Search benchmark: peor caso
set output searchOut
set title 'Search benchmark - peor caso'
columnCount = int(system(sprintf("awk -F, 'NR==2{print NF; exit}' \"%s\"", searchFile)))
if (columnCount < 3) {
	print sprintf("ERROR: CSV invalido (pocas columnas): %s", searchFile)
	exit 1
} else {
	plot for [col=2:3] searchFile using 1:col lw 4 title columnhead(col)
}
unset output

# Sort benchmark: mejor caso
set output sortBestOut
set title 'Sort benchmark - mejor caso'
set logscale y
columnCount = int(system(sprintf("awk -F, 'NR==2{print NF; exit}' \"%s\"", sortFile)))
if (columnCount < 13) {
	print sprintf("ERROR: CSV invalido (pocas columnas): %s", sortFile)
	exit 1
} else {
	plot for [col=2:5] sortFile using 1:col lw 4 title columnhead(col)
}
unset output
unset logscale y

# Sort benchmark: caso promedio
set output sortAverageOut
set title 'Sort benchmark - caso promedio'
if (columnCount < 13) {
	print sprintf("ERROR: CSV invalido (pocas columnas): %s", sortFile)
	exit 1
} else {
	plot for [col=6:9] sortFile using 1:col lw 4 title columnhead(col)
}
unset output

# Sort benchmark: peor caso
set output sortWorstOut
set title 'Sort benchmark - peor caso'
if (columnCount < 13) {
	print sprintf("ERROR: CSV invalido (pocas columnas): %s", sortFile)
	exit 1
} else {
	plot for [col=10:13] sortFile using 1:col lw 4 title columnhead(col)
}
unset output
