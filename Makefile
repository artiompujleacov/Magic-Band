build:
	gcc -std=c11 tema1.c -o tema1
run:
	./tema1
valgrind:
	valgrind --leak-check=full --track-origins=yes ./tema1
clean:
	rm -f tema1 *.o
archive:
	zip -r tema1.zip \
	Makefile README \
	*.c 