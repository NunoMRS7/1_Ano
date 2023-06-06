CFLAGS=-Wall -Wextra -pedantic -O2
LIBS=-lm -lcurses -lSDL2main -lSDL2 -lSDL2_mixer

jogo: main.o mapa.o visao.o jogador.o monstros.o combate.o som.o menu.o poderes.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

clean:
	rm jogo *.o
