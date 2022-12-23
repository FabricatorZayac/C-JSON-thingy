##
# JSON thingy
#
# @file
# @version 0.1

.PHONY: all clean run debug

run: all
	./json_thingy

debug: FLAGS := -O0 -ggdb
debug: all

all:
	gcc $(FLAGS) main.c -o json_thingy

clean:
	rm -f json_thingy
	rm -f *.json

# end
