#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>


int playMusica(char *musica) {
	Mix_Music *sound = NULL;
	sound = Mix_LoadMUS(musica); // load da música dada
	if (sound == NULL)
		return 0; // caso não conseguir dar load, devolve 0
	Mix_PlayMusic(sound,5); // toca a música, servindo o parâmetro 5 para indicar que há 5 loops
    return 1;
}


int playSom(char *som) {
    Mix_Chunk *sound = NULL;
	sound = Mix_LoadWAV(som); // load do som dado
	if (sound == NULL)
		return 0; // caso não conseguir dar load, devolve 0
	Mix_PlayChannel(-1,sound,0); // toca a música, servindo o parâmetro -1 para indicar que é tocada em todos os canais e o parâmetro 0 para indicar que não há loops
    return 1;
}