CC := gcc
CFLAGS := -O -Wuninitialized
OBJS := daemond.o
PROGRAM := daemond

all:		$(PROGRAM)

$(PROGRAM):	$(OBJS)
			$(CC) $(OBJS) -o $(PROGRAM)

clean:;		rm -f *.o *~ $(PROGRAM)
