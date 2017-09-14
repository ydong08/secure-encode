OBJECT=main.o rawsocket.o rawsocsniffer.o
CC=g++

main:$(OBJECT)
	$(CC) -o $@ $^
%.o: %.c
	$(CC) -c $<
clean:
	rm $(OBJECT)
    	
