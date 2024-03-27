CC := gcc
CFLAGS := -O -Wuninitialized
OBJS := kadai18.o
PROGRAM := kadai18

all:		$(PROGRAM)

$(PROGRAM):	$(OBJS)
			$(CC) $(OBJS) -o $(PROGRAM)

clean:;		rm -f *.o *~ $(PROGRAM)
