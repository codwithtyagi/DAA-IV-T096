CC = gcc
CFLAGS = -std=c99 -Wall
LDFLAGS = -lm
SRC = main.c graph.c routing.c schedule.c optimize.c search_sort.c hash.c utils.c
OBJ = $(SRC:.c=.o)
TARGET = delivery

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJ) $(TARGET)
