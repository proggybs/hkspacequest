LDLIBS=-lSDLmain -lSDL -lGL -lSDL_image
OPTS=-pedantic -Wall -Wextra
SRCS= main.c

game:
	gcc $(OPTS) -o hkspacequest $(SRCS) $(LDLIBS)
