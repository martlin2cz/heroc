# Martin Jašek
# KMI UP
# LS 2015
# mikroC intepreter

#standart překladu
STD=c99

#název binráky
BIN=uc



all:
	flex flex.l
#	bison bison.yy
	gcc -std=${STD} -o ${BIN} main.c

clean:
	rm flex.h flex.c bison.h bison.c uc
