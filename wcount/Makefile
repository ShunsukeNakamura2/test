CC := gcc
CFLAGS := -O -Wuninitialized
OBJS := wcount.o
PROGRAM := wcount

all:		$(PROGRAM)

$(PROGRAM):	$(OBJS)
			$(CC) $(OBJS) -o $(PROGRAM)

clean:;		rm -f *.o *~ $(PROGRAM)
