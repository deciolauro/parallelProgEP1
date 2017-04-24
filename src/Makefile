OUTPUT = mandelbrot

IMAGE = output.ppm

CC = gcc
CFLAGS = -Wall -Wpedantic -Werror -ggdb -pg
CC_OPT = -std=c11

CC_OMP = -fopenmp
CC_PTH = -pthread

.PHONY: all
all: $(OUTPUT)_omp $(OUTPUT)_pth $(OUTPUT)_seq

$(OUTPUT)_omp: $(OUTPUT)_omp.c
	$(CC) $(CFLAGS) -o $(OUTPUT)_omp $(CC_OPT) $(CC_OMP) $(OUTPUT)_omp.c

$(OUTPUT)_pth: $(OUTPUT)_pth.c
	$(CC) $(CFLAGS) -o $(OUTPUT)_pth $(CC_OPT) $(CC_PTH) $(OUTPUT)_pth.c

$(OUTPUT)_seq: $(OUTPUT)_seq.c
	$(CC) $(CFLAGS) -o $(OUTPUT)_seq $(CC_OPT) $(OUTPUT)_seq.c

.PHONY: clean

clean:
	rm -f $(OUTPUT)_omp $(OUTPUT)_pth $(OUTPUT)_seq $(IMAGE)
