objects = tcp.o main.o error.o

exe: $(objects)
	cc -o exe $(objects)

main.o: main.c
	cc -c main.c

tcp.o: tcp.h

error.o: ../utils/error.h ../utils/error.c
	cc -c ../utils/error.c

clean:
	rm exe $(objects)
