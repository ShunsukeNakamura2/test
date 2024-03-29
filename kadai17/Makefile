CC := gcc
CFLAGS := -O -Wuninitialized
OBJS := kadai17.o
PROGRAM := kadai17

all:		$(PROGRAM)

$(PROGRAM):	$(OBJS)
			$(CC) $(OBJS) -o $(PROGRAM)

clean:;		rm -f *.o *~ $(PROGRAM)