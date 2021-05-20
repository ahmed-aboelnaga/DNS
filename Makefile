all:dns

DNS.o:DNS.c head.h
	gcc -c DNS.c 

dns:DNS.o head.h
	gcc -o dns DNS.o head.h -g

test: all
	./dns www.facebook.com 4 @1.1.1.1;
	make clean

clean:
	rm dns *.o
