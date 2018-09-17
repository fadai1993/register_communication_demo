CC=sw5cc

laplace2d: host.o slave.o
	${CC} -hybrid  host.o slave.o -o a.out

host.o: host.c
	${CC} -host host.c  -c  -lm  -OPT:IEEE_arith=2 

slave.o: slave.c
	${CC} -slave slave.c  -c -msimd -OPT:IEEE_arith=2 



clean:
	rm -Rf fd *.o
