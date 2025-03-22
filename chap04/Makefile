objects = udp_server.o udp_client.o ../utils/error.o main.o

exe: $(objects)
	cc -o exe $(objects)

main.o: main.c
	cc -c main.c

udp_server.o: udp_server.h

udp_client.o: udp_client.h

../utils/error.o: ../utils/error.h

clean:
	rm exe $(objects)
