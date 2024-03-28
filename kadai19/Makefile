CC := gcc
CFLAGS := -O -Wuninitialized
OBJS := kadai19.o
PROGRAM := kadai19

all:		$(PROGRAM)

$(PROGRAM):	$(OBJS)
			$(CC) $(OBJS) -o $(PROGRAM)

clean:;		rm -f *.o *~ $(PROGRAM)
