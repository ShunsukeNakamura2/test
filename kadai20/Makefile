CC := gcc
CFLAGS := -O -Wuninitialized
OBJS := kadai20.o
PROGRAM := kadai20

all:		$(PROGRAM)

$(PROGRAM):	$(OBJS)
			$(CC) $(OBJS) -o $(PROGRAM)

clean:;		rm -f *.o *~ $(PROGRAM)
