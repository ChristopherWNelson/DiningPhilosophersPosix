COPTS = gcc -c -g -std=c99 -pedantic -Wall
ALL = philo

all: $(ALL)

clean:
	-rm -rf *.o *~ *.dSYM *philo

philo.o: philo.c
	${COPTS} philo.c -o philo.o

random_r.o: random_r.c random_r.h
	${COPTS} random_r.c -o random_r.o

philo: philo.o random_r.o
	gcc -g philo.o random_r.o -o philo -lm -lpthread