#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>
#include <math.h>


#define PI 3.14159265
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




void visao(jogo jg, pixeis px[jg.vertical][jg.horizontal]) {
	int X = jg.jogador.playerX;
	int Y = jg.jogador.playerY;

	for (double angle=0;angle<(2*PI);angle+=0.01) {
		int flag = 1;

		for (int r=1;r<30 && flag==1;r+=1) {
			int rcos = r*cos(angle);
			int rsin = 0.55*r*sin(angle); // multiplica-se por 0.55 para achatar o círculo
			
			px[Y + rsin][X + rcos].visao = 1; // liga o pixel em questão
			if (px[Y + rsin][X + rcos].mapa == '#'){ // se encontrar uma na linha em questão, as proximas casas dessa mesma não serão iluminadas
				flag=0;
			}
		}
	}
}