cc = gcc
cflags = -W -g
target = Starwars

objects = main.o Input.o Output.o

$(target) : $(objects)
	$(cc) $(cflags) -o $(target) $(objects)

%.o : %.c
	$(cc) $(cflags) -c -o $@ $<

main.o Input.o Output.o : Header.h

.PHONY : clean
clean :
	rm $(target) $(objects)
