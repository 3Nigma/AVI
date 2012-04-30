CC := gcc
LDFLAGS := `pkg-config --libs --cflags gtk+-2.0`
CFLAGS := -Wall -g

PROJ := monavic
OBJS := 

./%.o : ./%.c
	$(CPP) $(CFLAGS) -c $< -o $@

.PHONY: all clean

all: $(OBJS)
	$(CC) $(CFLAGS) -o $(PROJ) $(OBJS) main.c $(LDFLAGS)

clean:
	rm -f $(PROJ) $(OBJS)
