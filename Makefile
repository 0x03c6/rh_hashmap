all:
	gcc -o main main.c src/rh_hashmap.c -I include

clean:
	rm -f main

.PHONY: clean

