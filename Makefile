HEADERS=
OBJS=src/main.o

all: cat

cat: $(OBJS)
	gcc -o $@ $^

%.o: %.c $(HEADERS)
	gcc -Wall -Werror -Wpedantic -o $@ -c $<

