#/bin/sh
echo '' > results
make generator
make 
for j in 500 300 200 100 50 # 2 3 4 5 6 7 8 9 10 #11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30
do
	bin/generator $j
	for i in 5 20 50 #2 3 4 5 6 7  8 9 10 11 12 13 14 15 16 17 18 19 20 #21 22 23 24 25 26 27 28 29 30
	do
		mpirun -np $i bin/GaussJacobiDiff data/dataJ$j >> results
	done
done
cat results | grep 'Metoda'

