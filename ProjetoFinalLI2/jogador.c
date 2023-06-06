#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "monstros.h"
#include "combate.h"
#include "som.h"

#define MONSTROS_MAX 30
#define MUNICAO_MAX 30

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

typedef struct jogo {
	jogador jogador;
    monstros monstro[MONSTROS_MAX]; // máximo de 30 monstros
	bala bala;
	
	int vertical;
	int horizontal;
	int nivel;
	int highScore;
} jogo;




void moverJogador(jogo *jg, int dy, int dx, pixeis px[jg->vertical][jg->horizontal]) { // mover e dar dano aos monstros
	if (check_if_monstro(*jg,px,jg->jogador.playerY + dy,jg->jogador.playerX + dx)) { // caso o jogador se mova para uma casa em que se encontra um monstro, este não muda de posição 
		dano_jogador(jg,indice_monstro(*jg,jg->jogador.playerY + dy,jg->jogador.playerX + dx),px);
	} else { 
		if (px[jg->jogador.playerY + dy][jg->jogador.playerX].mapa != '#' && !check_if_monstro(*jg,px,jg->jogador.playerY + dy,jg->jogador.playerX)) { // caso em que o jogador se consiga mover primeiro pelo Y 
			jg->jogador.playerY += dy;
		
			if (px[jg->jogador.playerY][jg->jogador.playerX + dx].mapa != '#' && !check_if_monstro(*jg,px,jg->jogador.playerY,jg->jogador.playerX + dx)) {
				jg->jogador.playerX += dx;
			}
		}
		else {
			if (px[jg->jogador.playerY][jg->jogador.playerX + dx].mapa != '#' && !check_if_monstro(*jg,px,jg->jogador.playerY,jg->jogador.playerX + dx)) { // no caso em que o jogador não se consiga mover primeiro pelo Y, tenta mover-se primeiro, se conseguir, pelo X e depois tenta na nova posição mover-se pelo Y
				jg->jogador.playerX += dx;
			}
			if (px[jg->jogador.playerY + dy][jg->jogador.playerX].mapa != '#' && !check_if_monstro(*jg,px,jg->jogador.playerY + dy,jg->jogador.playerX)) {
				jg->jogador.playerY += dy;
			}
		}
	}

	if (px[jg->jogador.playerY][jg->jogador.playerX].mapa == '+') {
		if (jg->jogador.vida + 20+2*jg->nivel <= 100)
			jg->jogador.vida += 20+2*jg->nivel; // quando o jogador se encontra em cima de um power up de vida, a sua vida aumenta, tendo por base o nível atual
		else // caso o power up faça com que a vida do jogador ultrapasse o limite de 100, esta aumenta apenas até 100
			jg->jogador.vida = 100;
		px[jg->jogador.playerY][jg->jogador.playerX].mapa = '.'; // o caracter do power up é imediatamente substituido por '.' após ser usado
		playSom("musica/PowerUp.wav");
	}
	else if (px[jg->jogador.playerY][jg->jogador.playerX].mapa == '$') {
		jg->jogador.dano += 2 + jg->nivel; // quando o jogador se encontra em cima de um power up de dano, o seu dano aumenta, tendo por base o nível atual
		px[jg->jogador.playerY][jg->jogador.playerX].mapa = '.'; // o caracter do power up é imediatamente substituido por '.' após ser usado
		playSom("musica/PowerUp.wav");
	}
	if (px[jg->jogador.playerY][jg->jogador.playerX].mapa == 'W') {
		jg->bala.municao = MUNICAO_MAX;
		px[jg->jogador.playerY][jg->jogador.playerX].mapa = '.'; // o caracter da munição é imediatamente substituido por '.' após ser usado
		playSom("musica/PowerUp.wav");
	}
}




void posicionarJogador(jogo *jg, pixeis px[jg->vertical][jg->horizontal]) { 
    int newY = rand()%(jg->vertical-1), newX = rand()%jg->horizontal; // atribuir coordenadas aleatórias ao jogador
	
    while (px[newY][newX].mapa == '#') { // repete a ação anterior até a posição não ser uma parede 
		newY = rand()%(jg->vertical-1);
		newX = rand()%jg->horizontal;
	}

	jg->jogador.playerY = newY; // nova coordenada y jogador
	jg->jogador.playerX = newX; // nova coordenada x jogador
}