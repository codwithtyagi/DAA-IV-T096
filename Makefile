CC=clang
CFLAGS=-Wall -Wextra -O2
PKG_CONFIG=/opt/homebrew/bin/pkg-config

# CLI build
SRCS=main.c utils.c graph.c routing.c schedule.c optimize.c search_sort.c hash.c file_io.c
OBJS=$(SRCS:.c=.o)
TARGET=smart_delivery

# GUI build
GTK_CFLAGS=$(shell $(PKG_CONFIG) --cflags gtk+-3.0)
GTK_LIBS=$(shell $(PKG_CONFIG) --libs gtk+-3.0)
LIB_SRCS=utils.c graph.c routing.c schedule.c optimize.c search_sort.c hash.c file_io.c
LIB_OBJS=$(LIB_SRCS:.c=.o)
GUI_SRC=gui/gui_main.c
GUI_TARGET=smart_delivery_gui

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) -lm

gui: $(LIB_OBJS) $(GUI_SRC)
	$(CC) $(CFLAGS) $(GTK_CFLAGS) -o $(GUI_TARGET) $(GUI_SRC) $(LIB_OBJS) -lm $(GTK_LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) $(GUI_TARGET)
	rm -rf *.dSYM

.PHONY: all clean gui
