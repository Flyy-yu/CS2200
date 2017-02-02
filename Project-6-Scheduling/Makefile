# Makefile
# CS 2200 PRJ6

src=student.c os-sim.c process.c
obj=student.o os-sim.o process.o
inc=student.h os-sim.h process.h
misc=Makefile
target=os-sim
cflags=-Wall -g -O0 -Werror -pedantic -std=c99
lflags=-lpthread

all: $(target)

$(target) : $(obj) $(misc)
	gcc $(cflags) -o $(target) $(obj) $(lflags)

%.o : %.c $(misc) $(inc)
	gcc $(cflags) -c -o $@ $<

clean:
	rm -f $(obj) $(target)

submit: clean
	tar zcvf prj6-submit.tar.gz $(inc) $(src) $(misc) answers.txt
