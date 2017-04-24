#!/bin/bash

set -o xtrace

MEASUREMENTS=10
ITERATIONS=10
INITIAL_SIZE=16
NUM_THREADS=1
MAX_THREADS=33
MESSAGE="Starting stats with: "

SIZE=$INITIAL_SIZE

NAMES=('mandelbrot_seq' 'mandelbrot_pth' 'mandelbrot_omp')

make
mkdir results

for NAME in ${NAMES[@]}; do
    mkdir results/$NAME

	for((j=1; j<=$ITERATIONS; j++)); do
		for((i=1; i<=$MAX_THREADS; i++)); do
			if [ "$NAME" == "mandelbrot_seq" ] && [ $i -gt 1 ];
			then
				echo "Skipping threads test for mandelbrot_seq\n"
			else
				echo -e "\n$MESSAGE THREADS=$i, MEASUREMENTS=$MEASUREMENTS, IMGSIZE=$SIZE\n" >> full.log 2>&1
				echo -e "\n$MESSAGE THREADS=$i, MEASUREMENTS=$MEASUREMENTS, IMGSIZE=$SIZE\n" >> seahorse.log 2>&1
				echo -e "\n$MESSAGE THREADS=$i, MEASUREMENTS=$MEASUREMENTS, IMGSIZE=$SIZE\n" >> elephant.log 2>&1
				echo -e "\n$MESSAGE THREADS=$i, MEASUREMENTS=$MEASUREMENTS, IMGSIZE=$SIZE\n" >> triple_spiral.log 2>&1
				perf stat -r $MEASUREMENTS ./$NAME -2.5 1.5 -2.0 2.0 $SIZE $i >> full.log 2>&1
				perf stat -r $MEASUREMENTS ./$NAME -0.8 -0.7 0.05 0.15 $SIZE $i >> seahorse.log 2>&1
				perf stat -r $MEASUREMENTS ./$NAME 0.175 0.375 -0.1 0.1 $SIZE $i >> elephant.log 2>&1
				perf stat -r $MEASUREMENTS ./$NAME -0.188 -0.012 0.554 0.754 $SIZE $i >> triple_spiral.log 2>&1
			fi
		done
		SIZE=$(($SIZE * 2))
    done

    SIZE=$INITIAL_SIZE

    mv *.log results/$NAME
    rm output.ppm
done
