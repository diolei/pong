CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic
LDFLAGS = -lSDL2 -lm

SRCS = pong.c
EXE = ray

all: $(EXE)

$(EXE): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(EXE) $(LDFLAGS)

clean:
	rm -f $(EXE)

