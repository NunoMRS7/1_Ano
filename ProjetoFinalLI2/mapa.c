#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "som.h"
#include "monstros.h"

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

typedef struct jogo {
	jogador jogador;
    monstros monstro[MONSTROS_MAX]; // máximo de 30 monstros
    bala bala;
	
	int vertical;
	int horizontal;
    int nivel;
    int highScore;
} jogo;



void gerarMapa(jogo jg, pixeis px[jg.vertical][jg.horizontal]) {
    for (int i=0;i<jg.vertical-1;i++) { // percorre o mapa todo
        for (int j=0;j<jg.horizontal;j++) {
            if (i == 0 || i == jg.vertical - 2 || j == 0 || j == jg.horizontal - 1 || rand()%100<43) { // apenas há 43% de chance de uma casa passar a parede, caso esta se encontre dentro dos limites da janela
                px[i][j].mapa = '#';
            }
            else
                px[i][j].mapa = '.';
        }
    }
}



int tres_por_tres(int Y, int X, jogo jg, pixeis px[jg.vertical][jg.horizontal]) {
    int contador = 0;

    for(int i=Y-1;i<Y+2;i++){
        for(int j=X-1;j<X+2;j++){
            if(px[i][j].mapa == '#') 
                contador++; 
        }
    }
    return contador; // devolve o número de paredes num raio 3x3 duma casa
}



void conserta_mapa_tres_por_tres(jogo jg, pixeis px[jg.vertical][jg.horizontal]) {   
    char mapa_refresh[jg.vertical][jg.horizontal]; // este array auxiliar é necessário para não alterar o array que contem a informação de cada casa

    for (int k=0;k<4;k++) { // 4 passagens
        for (int i=1;i<jg.vertical-2;i++) {
            for (int j=1;j<jg.horizontal-1;j++) {
                if (tres_por_tres(i,j,jg,px) >= 5) // caso haja 5 ou mais paredes num raio de 3x3 da casa, a casa em si passa a parede
                    mapa_refresh[i][j] = '#';
                else
                    mapa_refresh[i][j] = '.';
            }
        }

        for(int i=1;i<jg.vertical-2;i++) { 
            for(int j=1;j<jg.horizontal-1;j++) {
                px[i][j].mapa = mapa_refresh[i][j]; // são copiadas todas as casas da matriz auxiliar para a matriz original
            }
        }
    }
}




void gera_portal(jogo jg, pixeis px[jg.vertical][jg.horizontal]) {
    int YPortal = jg.jogador.playerY-5 + rand()%11, XPortal = jg.jogador.playerX-5 + rand()%11; // atribuir coordenadas aleatórias ao portal, a um alcance de 11 por 11 do jogador
	
    while (px[YPortal][XPortal].mapa == '#' || check_if_jogador(jg,YPortal,XPortal) || check_if_monstro(jg,px,YPortal,XPortal) || YPortal <= 0 || YPortal >= jg.vertical - 2 || XPortal <= 0 || XPortal >= jg.horizontal - 1) { // repete a ação anterior caso esteje em cima de uma parete, esteje em cima de um monstro, esteje em cima do jogador ou esteje fora dos limites da janela
		YPortal = jg.jogador.playerY-5 + rand()%11;
		XPortal = jg.jogador.playerX-5 + rand()%11;
	}

    px[YPortal][XPortal].mapa = 'P';
    px[YPortal][XPortal].visao = 1; // há sempre visão do portal

    playSom("musica/Portal.wav"); // toca som do portal
}