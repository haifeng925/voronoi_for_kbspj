CFLAGS = -Wall
LDFLAGS := -lm

CC = gcc
RM = rm -f

SRC =  eventqueue.c voronoi.c pointqueue.c seglist.c arcpool.c segspool.c adjustsegs.c seg_intersection.c #voronoi_test.c#find_nearestpointonline.c #find_nearestpointonline.h
OBJ = $(SRC:.c=.o)
#OBJ = eventqueue.o voronoi.o pointqueue.o seglist.o arcpool.o segspool.o find_nearestpointonline.o

EXEC = voronoi


all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@  $^ $(LDFLAGS)

$(OBJ): envdefs.h 


#%.o: %.c
#	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ) $(EXEC)

run:
	./$(EXEC)

compile: $(SRC)
	$(CC) -c  $^ $(LDFLAGS)

