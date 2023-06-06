#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "mapa.h"
#include "visao.h"
#include "jogador.h"
#include "monstros.h"
#include "combate.h"
#include "som.h"
#include "menu.h"


#define MONSTROS_MAX 30

typedef struct jogador {
	int playerY;
	int playerX;
	int partir_paredes;
	int iluminar_monstros;
	int dano_area;
	int vida;
    int dano;
	int kills;
	int killsTotais;
	char sentido;
} jogador;

typedef struct monstros {
    int monstroY;
    int monstroX;
	int vida;
    int dano;
} monstros;

typedef struct bala {
        int balaY;
        int balaX;
        int dano;
        int municao;
        int ativa;
		char sentido;
} bala;

typedef struct pixeis {
    char mapa;
    int visao;
} pixeis;

typedef struct estado{
    int highlight; // 0 equivale ao botão Jogar/Jogar novamente, 1 equivale ao botão Ajuda, 2 equivale ao botão Sair/Desistir
    int ativo; // -1 equivale a Menu, 0 equivale a Jogo, 1 equivale a Ajuda, 2 equivale a Perdeu, 3 equivale a Passou de Nível
} estado;

typedef struct jogo {
	jogador jogador;
    monstros monstro[MONSTROS_MAX]; // máximo de 30 monstros
	bala bala;
	
	int vertical;
	int horizontal;
	int nivel;
	int highScore;
} jogo;




int main() {
	int success=1;
	if(Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048)<0) // Frequência, formato, nº de canais, chunk size
		success=0;
	if (playMusica("musica/Som_ProjetoLI2.wav")==0)
		success=0;

	jogo jg;
	estado st;

	WINDOW *wnd = initscr(); // receber o ecrã inicial
	getmaxyx(wnd,jg.vertical,jg.horizontal); // receber nas variáveis respetivas o número de linhas e colunas, a partir de wnd
	pixeis px[jg.vertical][jg.horizontal];

	srandom(time(NULL)); // gera um valor dependendo de time(NULL) em segundos
	start_color(); // dá start às cores
	curs_set(0); // desativa o cursor a piscar
	cbreak();
	noecho();
	nonl();
	intrflush(stdscr, false);
	keypad(stdscr, true); // ativa (true) o teclado
	nodelay(wnd,1); // ativado de modo a não haver espera de um caracter nas funções getchar()

	init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_BLACK); // Nome, foreground, background
    init_pair(COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK); // Nome, foreground, background
    init_pair(COLOR_BLUE, COLOR_BLUE, COLOR_BLACK); // Nome, foreground, background
	init_pair(COLOR_RED,COLOR_RED,COLOR_BLACK); // Nome, foreground, background
	init_pair(COLOR_GREEN,COLOR_GREEN,COLOR_BLACK); // Nome, foreground, background
	init_pair('G',15,COLOR_BLACK); // definida a cor que representa a vida do jogador a ser consumida ('G' é o nome da cor a ser chamada e 15 é o nome da cor a ser variada)
	init_pair('C',25,COLOR_BLACK); // definida a cor do mapa
	init_pair('R',35,35); // definida a cor do portal
	init_pair('V',COLOR_GREEN,COLOR_GREEN); // definida a cor da barra de vida do jogador
	init_pair('E',COLOR_RED,COLOR_RED); // definida a cor da seta de próximo nível
	init_pair('B',45,COLOR_BLACK); // definida a cor da bala

	st.ativo=-1; // o jogo começa no ecrã do Menu
	jg.highScore=0; // highscore começa a 0
	
	while (success) {
		if (st.ativo==-1) { // menu principal
			drawMenu(&jg,&st,wnd);
		}
		if (st.ativo==0) { // jogo
			drawJogo(&jg,px,&st);
		}
		if (st.ativo==1) { // ajuda
			drawAjuda(&jg,&st);
		}
		if (st.ativo==2) { // game over
			drawPerdeu(&jg,&st,wnd);
		}
		if (st.ativo==3) { // passou de nível
			drawNivel(&jg,&st);
		}
	}
	return 0;
}