#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>
#include <math.h>

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




void parte_paredes (jogo *jg, pixeis px [jg->vertical][jg->horizontal], double *t) {
    if (jg->jogador.partir_paredes == 6) { // esta habilidade só funciona após recarregar durante 6 seguntos (após o uso)
        for (int i=(jg->jogador.playerY-2);i<(jg->jogador.playerY+3);i++) {
            for (int j=(jg->jogador.playerX-2);j<(jg->jogador.playerX+3);j++) {
                if (px[i][j].mapa == '#' && i > 0 && i < jg->vertical - 2 && j > 0 && j < jg->horizontal - 1) { // estas condições não permitem ao jogador partir as paredes que limitam o ecrã
                    px[i][j].mapa = '.'; // troca uma parede por uma casa livre
                }
            }
        }
        jg->jogador.partir_paredes = 0; // reset do cooldown
        *t = 0; // reset do contador que incrementa

        playSom("musica/Quebrar_Paredes.wav");
    }
}




void ilumina_monstros(jogo *jg, pixeis px[jg->vertical][jg->horizontal], double *t) {
    if (jg->jogador.iluminar_monstros == 15) { // esta habilidade só funciona após recarregar durante 15 seguntos (após o uso)
        for (int i=0;i<MONSTROS_MAX;i++) {
            px[jg->monstro[i].monstroY][jg->monstro[i].monstroX].visao = 1; // todos os monstros ficam visíveis
        }
        jg->jogador.iluminar_monstros = 0; // reset do cooldown
        *t = 0; // reset do contador que incrementa

        playSom("musica/Beep.wav");
    }
}




void dano_em_area(jogo *jg, pixeis px[jg->vertical][jg->horizontal], double *t) {
    if (jg->jogador.dano_area == 10) { // esta habilidade só funciona após recarregar durante 10 seguntos (após o uso)
        for (int i=jg->jogador.playerY-1;i<jg->jogador.playerY+2;i++) {
            for (int j=jg->jogador.playerX-1;j<jg->jogador.playerX+2;j++) {
                if (check_if_monstro(*jg,px,i,j)) { // caso em que na posição averiguada se encontre um monstro
                    jg->monstro[indice_monstro(*jg,i,j)].vida = 0; // mata instantaneamente o mosntro na posição averiguada
                    jg->jogador.kills++;
                    jg->jogador.killsTotais++;
                    int chancePowerUp = rand()%100;
                    if (chancePowerUp <= 10) // chance de surgir um power up de vida, de um monstro, após a sua morte
                        px[i][j].mapa = '+';
                    else if (chancePowerUp >= 90) // chance de surgir um power up de dano, de um monstro, após a sua morte
                        px[i][j].mapa = '$';
                    else if (chancePowerUp >= 40 && chancePowerUp <= 70) // chance de surgir munição, de um monstro, após a sua morte
                        px[i][j].mapa = 'W';
                }
            }
        }
        jg->jogador.dano_area = 0; // reset do cooldown
        *t = 0; // reset do contador que incrementa
        
        playSom("musica/Som_Dano_Em_Area.wav");
    }
}