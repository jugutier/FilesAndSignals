COMPILE=compile

help:
	@echo "1) server"
	@echo "2) client"

$(COMPILE):
	@echo "compiling"
	
server:$(COMPILE)
	@gcc -o server.o server/main.c files.c
	@./server.o

client:$(COMPILE)
	@gcc -o client.o client/main.c files.c
	@./client.o