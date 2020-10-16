# Just translate the files.
# Result would be div.o main.o in the present working directory
PROG= compile

CC= g++
CLR= rm *.o

OBJS= interpreter.o variables.o

$(PROG) 		: $(OBJS)
				$(CC) -o $@ $(OBJS) && $(CLR)

interpreter.o 	: interpreter.cpp
				$(CC) -g -c $<

variables.o 	: variables.cpp
				$(CC) -g -c $<

clean			:
				rm *.o 