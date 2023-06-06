#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>
#include <math.h>

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




int check_if_monstro(jogo jg, pixeis px[jg.vertical][jg.horizontal], int PosY, int PosX) { // verifica se há um monstro numa dada posição (só regista se este tiver vida superior a 0 e visão 1)
	for (int i=0;i<MONSTROS_MAX;i++) {
		if (PosY == jg.monstro[i].monstroY && PosX == jg.monstro[i].monstroX && px[jg.monstro[i].monstroY][jg.monstro[i].monstroX].visao == 1 && jg.monstro[i].vida > 0) {
			return 1;
		}
	}
	return 0;
}




int indice_monstro(jogo jg, int PosY, int PosX) { // se existir um monstro numa dada posição, a função devolve o índice do respetivo monstro
	for (int i=0;i<MONSTROS_MAX;i++) {
		if (PosY == jg.monstro[i].monstroY && PosX == jg.monstro[i].monstroX && jg.monstro[i].vida > 0) {
			return i;
		}
	}
	return -1; // caso em que não encontra monstro
}




int check_if_jogador(jogo jg, int PosY, int PosX) { // verifica se o jogador se encontra numa dada posição
	if (PosY == jg.jogador.playerY && PosX == jg.jogador.playerX) {
		return 1;
	}
	return 0;
}




int proximidadeDoJogador(int Y, int X, jogo jg) {
    for(int i=Y-8;i<Y+9;i++){
        for(int j=X-8;j<X+9;j++){
            if(i == jg.jogador.playerY && j == jg.jogador.playerX) 
            return 1; // devolve 1 quando as coordenadas dadas estão num raio igual ou inferior a 16x16 do jogador
        }
    }
    return 0;
}




void posicionarMonstros(jogo *jg, pixeis px[jg->vertical][jg->horizontal]) { // usada para dar as posições iniciais dos monstros
    for (int i=0;i<MONSTROS_MAX;i++) {
        int newY = rand()%(jg->vertical-1), newX = rand()%jg->horizontal; // atribuir coordenadas aleatórias ao monstro
        
        while (px[newY][newX].mapa == '#' || proximidadeDoJogador(newY,newX,*jg)) { // garantir que o monstro não é clocado numa parede, nem próximo do jogador
            newY = rand()%(jg->vertical-1);
            newX = rand()%jg->horizontal;
        }
        jg->monstro[i].monstroY = newY; // coordenada y monstro
        jg->monstro[i].monstroX = newX; // coordenada x monstro

    }
}




void print_monstros(jogo jg, pixeis px[jg.vertical][jg.horizontal]) { // usada continuamente para dar print aos monstros     
    for (int i=0;i<MONSTROS_MAX;i++) {
        if (px[jg.monstro[i].monstroY][jg.monstro[i].monstroX].visao == 1 && jg.monstro[i].vida > 0) { // é necessário estar vivo para dar print
            mvaddch(jg.monstro[i].monstroY,jg.monstro[i].monstroX,'M' | A_BOLD);
        }
    }
}




double dist(int y1, int x1, int y2, int x2) { // usada para calcular a distância entre monstros e jogadores
    return sqrt(pow(x1-x2,2) + pow(y1-y2,2));
}



void mover_monstros(jogo *jg, pixeis px[jg->vertical][jg->horizontal]) {
    for (int n=0;n<MONSTROS_MAX;n++) { // para cada monstro
        if (rand()%(10000-100*jg->nivel) <= 1 && px[jg->monstro[n].monstroY][jg->monstro[n].monstroX].visao == 1) { // chance do monstro se mover (só quando este está no campo de visão do jogador), que incrementa com o nível
            double distInicial = dist(jg->monstro[n].monstroY,jg->monstro[n].monstroX,jg->jogador.playerY,jg->jogador.playerX); // distância inicial entre um dado monstro e o jogador
            int guardaY = jg->monstro[n].monstroY; // variável que irá guardar a ordenada mais satizfatória para o monstro se aproximar do jogador
            int guardaX = jg->monstro[n].monstroX; // variável que irá guardar a abcissa mais satizfatória para o monstro se aproximar do jogador

            for(int i=jg->monstro[n].monstroY-1;i<jg->monstro[n].monstroY+2;i++){
                for(int j=jg->monstro[n].monstroX-1;j<jg->monstro[n].monstroX+2;j++){
                    double distNova = dist(i,j,jg->jogador.playerY,jg->jogador.playerX);
                    if(distNova < distInicial && px[i][j].mapa != '#' && px[i][j].visao == 1 && !check_if_jogador(*jg,i,j) && !check_if_monstro(*jg,px,i,j)) { // as novas coordenadas são guardadas apenas se estas conseguirem aproximar o monstro do jogador, respeitando várias imposições
                        distInicial = distNova;
                        guardaY = i;
                        guardaX = j;
                    }
                }
            }
            jg->monstro[n].monstroY = guardaY;
            jg->monstro[n].monstroX = guardaX;
        }
    }
}