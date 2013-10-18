

CFLAGS = -Wall
LDFLAGS := -lm

CC = gcc
RM = rm -f

SRC = point_queue.c event_queue.c arc.c seg_list.c voronoi.c adjustsegs.c seg_intersection.c  #find_nearestpointonline.c #find_nearestpointonline.h

SRC_TEST =  voronoi_test.c point_queue.c event_queue.c arc.c seg_list.c voronoi.c adjustsegs.c seg_intersection.c  #find_nearestpointonline.c #find_nearestpointonline.h
OBJ = $(SRC_TEST:.c=.o)
#OBJ = eventqueue.o voronoi.o pointqueue.o seglist.o arcpool.o segspool.o find_nearestpointonline.o

EXEC = voronoi

.PHONY: check-syntax 
check-syntax:
	$(CC) -Wall -Wextra -fsyntax-only $(CHK_SOURCES)
##$(CC) -Wall -Wextra -pedantic -fsyntax-only $(CHK_SOURCES)

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@  $^ $(LDFLAGS)

$(OBJ): voronoi.h 


#%.o: %.c
#	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ) $(EXEC)

run:
	./$(EXEC)

compile: $(SRC)
	$(CC) -c  $^ $(LDFLAGS)

