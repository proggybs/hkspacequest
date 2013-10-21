LDLIBS=-lSDLmain -lSDL -lGL -lSDL_image
OPTS=-std=c99 
IGNORES=-isystem /usr/include/gl
SRCS=main.c component.c entity.c system.c texture.c

game:
	gcc $(OPTS) $(IGNORES) -o hkspacequest $(SRCS) $(LDLIBS)
