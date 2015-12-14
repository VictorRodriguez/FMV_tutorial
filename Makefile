
normal:
	export CFLAGS=""
	gcc sort.c -o sort

normal_objdump:
	export CFLAGS=""
	gcc sort.c -g -c

vectorized:
	gcc -O3 sort.c -o sort

vectorized_objdump:
	gcc -O3 sort.c -g -c

avx2:
	gcc -O3 sort.c -o sort -mavx2 

avx2_objdump:
	gcc -O3 sort.c -g -c -mavx2

fmv:
	gcc -O3 sort_fmv.c -o sort

fmv_objdump:
	gcc -O3 sort_fmv.c -g -c

clean:
	rm -rf sort
	rm -rf *.o

