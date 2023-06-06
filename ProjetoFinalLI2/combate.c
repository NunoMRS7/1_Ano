#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>

#include "monstros.h"
#include "som.h"

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




void inicializa_dano_vida_monstros(jogo *jg) {
    for (int i=0;i<MONSTROS_MAX;i++) { // distribuir danos e vidas diferentes pelos monstros com o rand, tornando o jogo mais difícil com o incremento das vidas e danos pelo nível
        jg->monstro[i].vida = rand()%21 + 90 + 4*jg->nivel;
        jg->monstro[i].dano = rand()%5 + 16 + jg->nivel;
    }
}




void dano_monstro(jogo *jg, pixeis px) {
    for (int i=jg->jogador.playerY-1;i<jg->jogador.playerY+2;i++) {
        for (int j=jg->jogador.playerX-1;j<jg->jogador.playerX+2;j++) {
            if (check_if_monstro(*jg,px,i,j) && rand()%(100000-100*jg->nivel) <= 8) { // os monstros não têm 100% de precisão, mas esta aumenta com os níveis
                jg->jogador.vida -= jg->monstro[indice_monstro(*jg,i,j)].dano;
                playSom("musica/LevarDano.wav");
            }
        }
    }
}




void dano_jogador(jogo *jg, int i, pixeis px[jg->vertical][jg->horizontal]) {
    if (rand()%100 <= 80) { // o jogador não tem 100% de precisão
        jg->monstro[i].vida -= jg->jogador.dano;
        if (jg->monstro[i].vida > 0) // se o monstro permanecer vivo depois do ataque do jogador
            playSom("musica/Soco.wav");
        else {
            playSom("musica/Morte_Monstro.wav");
            jg->jogador.kills++;
            jg->jogador.killsTotais++;
            int chancePowerUp = rand()%100;
            if (chancePowerUp <= 10) // chance de surgir um power up de vida, de um monstro, após a sua morte
                px[jg->monstro[i].monstroY][jg->monstro[i].monstroX].mapa = '+';
            else if (chancePowerUp >= 90) // chance de surgir um power up de dano, de um monstro, após a sua morte
                px[jg->monstro[i].monstroY][jg->monstro[i].monstroX].mapa = '$';
            else if (chancePowerUp >= 40 && chancePowerUp <= 70 && jg->nivel >= 2) // chance de surgir munição, de um monstro, após a sua morte (a partir do nível 2)
                px[jg->monstro[i].monstroY][jg->monstro[i].monstroX].mapa = 'W';
        }
    }
}




void move_bala(jogo *jg, pixeis px[jg->vertical][jg->horizontal]) {
    if (jg->bala.ativa==1) { // está a ser disparada uma bala
        switch (jg->bala.sentido) { // dependendo do sentido para o qual o jogador está voltado, a bala segue nesse sentido
            case 'c': jg->bala.balaY--; break;
            case 'b': jg->bala.balaY++; break;
            case 'e': jg->bala.balaX--; break;
            case 'd': jg->bala.balaX++; break;
            default: break;
        }
        if (px[jg->bala.balaY][jg->bala.balaX].mapa=='#') // se for detetada uma parede a bala colide e desaparece
            jg->bala.ativa=0;
        if (check_if_monstro(*jg,px,jg->bala.balaY,jg->bala.balaX)) { // se for detetado um monstro a bala colide e desaparece
            if (jg->monstro[indice_monstro(*jg,jg->bala.balaY,jg->bala.balaX)].vida<=0.75*jg->jogador.dano) { // verifica se o monstro irá morrer nesta próxima iteração
                playSom("musica/Morte_Monstro.wav");
                jg->jogador.kills++;
                jg->jogador.killsTotais++;
                int chancePowerUp = rand()%100;
                if (chancePowerUp <= 10) // chance de surgir um power up de vida, de um monstro, após a sua morte
                    px[jg->monstro[indice_monstro(*jg,jg->bala.balaY,jg->bala.balaX)].monstroY][jg->monstro[indice_monstro(*jg,jg->bala.balaY,jg->bala.balaX)].monstroX].mapa = '+';
                else if (chancePowerUp >= 90) // chance de surgir um power up de dano, de um monstro, após a sua morte
                    px[jg->monstro[indice_monstro(*jg,jg->bala.balaY,jg->bala.balaX)].monstroY][jg->monstro[indice_monstro(*jg,jg->bala.balaY,jg->bala.balaX)].monstroX].mapa = '$';
                else if (chancePowerUp >= 40 && chancePowerUp <= 70) // chance de surgir munição, de um monstro, após a sua morte
                    px[jg->monstro[indice_monstro(*jg,jg->bala.balaY,jg->bala.balaX)].monstroY][jg->monstro[indice_monstro(*jg,jg->bala.balaY,jg->bala.balaX)].monstroX].mapa = 'W';
            }
            jg->monstro[indice_monstro(*jg,jg->bala.balaY,jg->bala.balaX)].vida-=0.75*jg->jogador.dano;
            jg->bala.ativa=0;
        }
    }
}