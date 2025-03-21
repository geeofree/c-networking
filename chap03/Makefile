objects = tcp_server.o tcp_client.o ../utils/error.o main.o

exe: $(objects)
	cc -o exe $(objects)

main.o: main.c
	cc -c main.c

tcp_server.o: tcp_server.h

tcp_client.o: tcp_client.h

../utils/error.o: ../utils/error.h

clean:
	rm exe $(objects)
