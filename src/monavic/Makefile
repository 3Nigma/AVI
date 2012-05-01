CC := gcc
LDFLAGS := `pkg-config --libs --cflags gtk+-2.0`
CFLAGS := -Wall -I. -I../../lib -g 

PROJ := monavic
OBJS := rs232.o commands.o console.o camwidget.o mainwindow.o

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(LDFLAGS)

.PHONY: all clean

all: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) main.c -o $(PROJ) $(LDFLAGS)

clean:
	rm -f $(PROJ) $(OBJS)
