LDLIBS=-lSDLmain -lSDL -lGL -lSDL_image
OPTS=-std=c99 -pedantic -Wall -Wextra 
SRCS=main.c component.c entity.c system.c texture.c

game:
	gcc $(OPTS) -o hkspacequest $(SRCS) $(LDLIBS)
