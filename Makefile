CC := gcc
CFLAGS := -Wall -Werror

csrc = $(wildcard src/*.c) \
	   $(wildcard src/ref_counting/*.c)

obj = $(csrc: .c = .o) 

main: $(obj)
	$(CC) $(CFLAGS) -o $@ $^

clean: 
	rm -rf *o main