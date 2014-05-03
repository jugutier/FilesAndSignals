COMPILE=compile

help:
	@echo "1) compile --- "

$(COMPILE):
	@gcc -o filesAndSignals main.c
	@./filesAndSignals