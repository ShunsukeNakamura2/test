CC := gcc
CFLAGS := -O -Wuninitialized
OBJS := utltrandata.o
PROGRAM := utltrandata

all:		$(PROGRAM)

$(PROGRAM):	$(OBJS)
			$(CC) $(OBJS) -o $(PROGRAM)

clean:;		rm -f *.o *~ $(PROGRAM)
