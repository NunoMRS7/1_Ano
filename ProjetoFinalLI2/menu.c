#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "mapa.h"
#include "visao.h"
#include "jogador.h"
#include "monstros.h"
#include "combate.h"
#include "som.h"
#include "poderes.h"

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

typedef struct estado{
    int highlight; // 0 equivale ao botão Jogar(Menu Principal)/Jogar novamente(Game Over), 1 equivale ao botão Ajuda(Menu Principal)/Desistir(Game Over), 2 equivale ao botão Sair(Menu Principal)
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




void update(jogo *jg, pixeis px[jg->vertical][jg->horizontal], double *tempoPartirParedes, double *tempoIluminarMonstros, double *tempoDanoArea) {
	switch(getch()) { // atualiza as coordenadas do jogador
		case KEY_BACKSPACE: endwin(); exit(0); break;
		case KEY_A1:
		case '7': moverJogador(jg, -1, -1, px); break;
		case KEY_UP:
		case '8': moverJogador(jg, -1, +0, px); jg->jogador.sentido='c'; break; // o sentido do jogador muda para cima
		case KEY_A3:
		case '9': moverJogador(jg, -1, +1, px); break;
		case KEY_LEFT:
		case '4': moverJogador(jg, +0, -1, px); jg->jogador.sentido='e'; break; // o sentido do jogador muda para a esquerda
		case KEY_RIGHT:
		case '6': moverJogador(jg, +0, +1, px); jg->jogador.sentido='d'; break; // o sentido do jogador muda para a direita
		case KEY_C1:
		case '1': moverJogador(jg, +1, -1, px); break;
		case KEY_DOWN:
		case '2': moverJogador(jg, +1, +0, px); jg->jogador.sentido='b'; break; // o sentido do jogador muda para baixo
		case KEY_C3:
		case '3': moverJogador(jg, +1, +1, px); break;
		case 'Q':
		case 'q': parte_paredes(jg,px,tempoPartirParedes); break; // invoca o poder
		case 'W':
		case 'w': ilumina_monstros(jg,px,tempoIluminarMonstros); break; // invoca o poder
		case 'E':
		case 'e': if (jg->nivel>=3) dano_em_area(jg,px,tempoDanoArea); break; // invoca o poder. Esta habilidade só funciona a partir do nível 3
		case 'D':
		case 'd': if (jg->nivel>=2) {  // a arma só está disponível a partir do nível 2
					if (jg->bala.ativa==0 && jg->bala.municao>0) { // o disparo só é realizado quando não estiver nenhuma bala em trajetória
						playSom("musica/Tiro.wav");
						jg->bala.municao--; // gasta-se uma bala
						jg->bala.ativa = 1; // a bala entra em trajetória
						jg->bala.balaY = jg->jogador.playerY; // a posição da bala assume a posição do jogador
						jg->bala.balaX = jg->jogador.playerX; // a posição da bala assume a posição do jogador
						jg->bala.sentido = jg->jogador.sentido; // o sentido da bala assume o sentido do jogador
					}
					else if (jg->bala.municao<=0)
						playSom("musica/Arma_Sem_Municao.wav");
				  }
				  break;
		default: break;
	}
}




void drawMenu(jogo *jg, estado *st) {
	Mix_VolumeMusic(24); // volume da música de fundo a 100%

    st->highlight = 0; // começa com a opção de jogar marcada
	jg->nivel = 1; // inicializa
	jg->jogador.vida = 100; // inicializa
	jg->jogador.dano = 25; // inicializa
	jg->jogador.killsTotais = 0; // inicializa
	jg->bala.municao = 0; // inicializa

	if (jg->highScore > 0) // só aparece o high score no ecrã, caso este seja maior do que 0
		mvprintw(jg->vertical/2-2, jg->horizontal/2-6, "High Score: %d     ",jg->highScore*10);

  	attron(COLOR_PAIR(COLOR_RED));
  	mvprintw(10, jg->horizontal/2-43, "#########      ###########     ##########     ##########    ##        ##   ############");
	mvprintw(11, jg->horizontal/2-43, "##########     ############   ############   ############   ##        ##   ############");
	mvprintw(12, jg->horizontal/2-43, "##       ##    ##        ##   ##        ##   ##             ##        ##   ##");
	mvprintw(13, jg->horizontal/2-43, "##       ##    ##       ##    ##        ##   ##             ##        ##   ##");
	mvprintw(14, jg->horizontal/2-43, "#########      #########      ##        ##   ##      ####   ##        ##   #######");
	mvprintw(15, jg->horizontal/2-43, "###########    ########       ##        ##   ##     #####   ##        ##   #######");
	mvprintw(16, jg->horizontal/2-43, "##       ###   ##     ##      ##        ##   ##        ##   ##        ##   ##");
	mvprintw(17, jg->horizontal/2-43, "##       ##    ##      ##     ##        ##   ##        ##   ###      ###   ##");
	mvprintw(18, jg->horizontal/2-43, "##########     ##       ##    ############   ############    ##########    ############");
	mvprintw(19, jg->horizontal/2-43, "#########      ##        ##    ##########     ##########       ######      ############");
  	attroff(COLOR_PAIR(COLOR_RED));

	mvprintw(jg->vertical-5,jg->horizontal/2-30,"Nuno Silva | Eduardo Faria | Gonçalo Cunha | André Ribeiro"); // membros do grupo

    while (st->ativo == -1) {

        for(int i=0;i<3;i++) { 
            if(i == st->highlight)
            	attron(A_REVERSE); // highlight da opção
            switch (i) {
                case 0: mvprintw(jg->vertical/2+2, jg->horizontal/2-2,"Jogar"); break;
                case 1: mvprintw(jg->vertical/2+4, jg->horizontal/2-2,"Ajuda"); break;
				case 2: mvprintw(jg->vertical/2+6, jg->horizontal/2-2,"Sair"); break;
            }
            attroff(A_REVERSE);
        }
        switch(getch()) { // por cada troca, muda o state que está highlighted e toca um som
            case KEY_UP:
                if (st->highlight == 1) {
                    st->highlight = 0;
					playSom("musica/Beep.wav");
				}
				else if (st->highlight == 2) {
                    st->highlight = 1;
					playSom("musica/Beep.wav");
				}
				else {
					st->highlight = 2;
					playSom("musica/Beep.wav");
				}
                break;
            case KEY_DOWN:
                if (st->highlight == 0) {
                    st->highlight = 1;
					playSom("musica/Beep.wav");
				}
				else if (st->highlight == 1) {
                    st->highlight = 2;
					playSom("musica/Beep.wav");
				}
				else {
					st->highlight = 0;
					playSom("musica/Beep.wav");
				}
                break;
            case 13:
                if (st->highlight == 0) {
					st->ativo = 0;
                }
				else if (st->highlight == 1) {
					st->ativo = 1;
				}
                else {
                    endwin();
                    exit(0);
				}
				break;
            default: break;
        }
    }
	clear();
	refresh();
}




void drawJogo(jogo *jg, pixeis px[jg->vertical][jg->horizontal], estado *st) {
	int r=0,g=0,b=0; // cores para mapa

	Mix_VolumeMusic(24); // volume da música de fundo a 100%
   
	gerarMapa(*jg,px); // inicializa
	conserta_mapa_tres_por_tres(*jg,px); // inicializa
	
	posicionarJogador(jg,px); // inicializa
	posicionarMonstros(jg,px); // inicializa

	inicializa_dano_vida_monstros(jg); // inicializa
    
	int portalAtivo = 0; // inicializa
	jg->bala.ativa = 0; // inicializa
	jg->jogador.kills = 0; // inicializa
	jg->jogador.sentido = 'c'; // inicializa
	jg->jogador.dano_area = 10; // inicializa
	jg->jogador.partir_paredes = 6; // inicializa
	jg->jogador.iluminar_monstros = 15; // inicializa
	double tempoRapido_dano_area = 0; // inicializa
	double tempoRapido_move_bala = 0; // inicializa
	double tempoRapido_vida_jogador = 0; // inicializa
	double tempoRapido_partir_paredes = 0; // inicializa
	double tempoRapido_iluminar_monstros = 0; // inicializa
   
	switch(jg->nivel%4) { // para cada nível é selecionada uma cor, entre 4, para o mapa
		case 0: r=400;g=400;b=400; break; // cinzento
		case 1: r=608;g=448;b=300; break; // castanho
		case 2: r=200;g=200;b=900; break; // azul
		case 3: r=0;g=600;b=600; break; // turquesa
	}
	init_color(25,r,g,b); // cor para mapa

	init_color(35,464,0,572); // cor do portal

    while(jg->jogador.vida > 0 && px[jg->jogador.playerY][jg->jogador.playerX].mapa != 'P') {  // o jogo atual só é mantido enquanto o jogador está vivo (vida superior a 0) e não se move para dentro do portal

		attron(COLOR_PAIR(COLOR_WHITE));
		mvprintw(jg->vertical-1, 1,"HP:");
		attroff(COLOR_PAIR(COLOR_WHITE));
		move(jg->vertical-1, 5);
		for (int i=0;i<100;i+=5) { // forma de representar a barra de vida do jogador
			if (i<=jg->jogador.vida) {
				attron(COLOR_PAIR('V'));
				printw(" "); // print de um "quadrado" verde
				attroff(COLOR_PAIR('V'));
			}
			else
				printw(" ");
		}
		
		if (jg->nivel<2) {
			attron(COLOR_PAIR(COLOR_WHITE));
			mvprintw(jg->vertical-1, 26,"|| Dano:%d || Kills:%d/10 || Nivel:%d || Q:%d/6  W:%d/15    ",jg->jogador.dano,jg->jogador.kills,jg->nivel,jg->jogador.partir_paredes,jg->jogador.iluminar_monstros);
			attroff(COLOR_PAIR(COLOR_WHITE));
		}
		else if (jg->nivel<3) { // a habilidade de disparar só fica (visualmente) disponível a partir do nível 2
			attron(COLOR_PAIR(COLOR_WHITE));
			mvprintw(jg->vertical-1, 26,"|| Balas:%d/%d || Dano:%d || Kills:%d/10 || Nivel:%d || Q:%d/6  W:%d/15    ",jg->bala.municao,MUNICAO_MAX,jg->jogador.dano,jg->jogador.kills,jg->nivel,jg->jogador.partir_paredes,jg->jogador.iluminar_monstros);
			attroff(COLOR_PAIR(COLOR_WHITE));
		}
		else { // a habilidade de dano em área só fica (visualmente) disponível a partir do nível 3
			attron(COLOR_PAIR(COLOR_WHITE));
			mvprintw(jg->vertical-1, 26,"|| Balas:%d/%d || Dano:%d || Kills:%d/10 || Nivel:%d || Q:%d/6  W:%d/15  E:%d/10     ",jg->bala.municao,MUNICAO_MAX,jg->jogador.dano,jg->jogador.kills,jg->nivel,jg->jogador.partir_paredes,jg->jogador.iluminar_monstros,jg->jogador.dano_area);
			attroff(COLOR_PAIR(COLOR_WHITE));
		}
		
		tempoRapido_move_bala += 0.005;
		if (tempoRapido_move_bala > 1) { // a bala atualiza a posição sempre que o contador rápido atinge o valor 1 (voltando a 0)
			move_bala(jg,px);
			tempoRapido_move_bala = 0;
		}

		if (jg->jogador.vida < 100) {  // jogador ganha vida passivamente
			tempoRapido_vida_jogador += 0.0001;
			if (tempoRapido_vida_jogador > 1) { // o jogador incrementa 1 de HP sempre que o contador rápido atinge o valor 1 (voltando a 0)
				jg->jogador.vida += tempoRapido_vida_jogador;
				tempoRapido_vida_jogador = 0;
			}
		}

		if (jg->jogador.partir_paredes < 6) { // tempo de recarga do poder
			tempoRapido_partir_paredes += 0.00023;
			jg->jogador.partir_paredes = tempoRapido_partir_paredes;
		}
		if (jg->jogador.iluminar_monstros < 15) { // tempo de recarga do poder
			tempoRapido_iluminar_monstros += 0.00023;
			jg->jogador.iluminar_monstros = tempoRapido_iluminar_monstros;
		}
		if (jg->jogador.dano_area < 10) { // tempo de recarga do poder
			tempoRapido_dano_area += 0.00023;
			jg->jogador.dano_area = tempoRapido_dano_area;
		}

		attron(COLOR_PAIR('C'));
		for (int i=0;i<jg->vertical-1;i++) { // Mapa sem luz, cor que varia com o nível
        	for (int j=0;j<jg->horizontal;j++) {
				if (px[i][j].mapa == '#')
					mvaddch(i,j,px[i][j].mapa); // Apaga todos os pontos (serve para apagar o caminho que ficou para trás)
				else
					mvaddch(i,j,' ');
			}
		}
		attroff(COLOR_PAIR('C'));
		
		if (jg->jogador.iluminar_monstros < 3) { // enquanto que o tempo de recarga do poder de iluminar monstros for inferior a 3, estes mantêm-se visiveis
			tempoRapido_iluminar_monstros += 0.00026; // o contador de tempo da habilidade em causa necessita de um maior incremento, nesta situação, devido ao aumento de iterações
			tempoRapido_partir_paredes += 0.00026; // análogo à linha anterior
			tempoRapido_dano_area += 0.00026; // análogo à linha anterior
			tempoRapido_move_bala += 0.0045; // análogo à linha anterior
			for (int i=0;i<jg->vertical-1;i++) {
				for (int j=0;j<jg->horizontal;j++) {
					if (!check_if_monstro(*jg,px,i,j))
						px[i][j].visao = 0; // desliga os píxeis que não são monstros nem portal
				}
			}
		}
		else { // todo o mapa é desligado (é feito um reset)
			for (int i=0;i<jg->vertical-1;i++) {
				for (int j=0;j<jg->horizontal;j++) {
					px[i][j].visao = 0; 
				}
			}
		}

		visao(*jg,px); // atualiza a visão de acordo com a posição do jogador

		for (int i=0;i<jg->vertical-1;i++) { // Liga os píxeis à volta do jogador (pixeis com número 1)
        	for (int j=0;j<jg->horizontal;j++) {
				if (px[i][j].visao == 1 && px[i][j].mapa != 'P' && px[i][j].mapa != '+' && px[i][j].mapa != '$' && px[i][j].mapa!= 'W') { // o portal, os powerups e a munição mantêm a sua cor
					attron(COLOR_PAIR(COLOR_YELLOW));
					mvaddch(i,j,px[i][j].mapa | A_BOLD); // a iluminação tem por base a cor amarela
					attroff(COLOR_PAIR(COLOR_YELLOW));
				}
				else if (px[i][j].mapa == 'P') {
					attron(COLOR_PAIR('R'));
					mvaddch(i,j,px[i][j].mapa | A_BOLD); // o portal tem a sua iluminação e cor única
					attroff(COLOR_PAIR('R'));
				}
				else if (px[i][j].mapa == '+') {
					attron(COLOR_PAIR(COLOR_GREEN));
					mvaddch(i,j,px[i][j].mapa | A_BOLD); // o power up de vida tem a sua iluminação e cor única
					attroff(COLOR_PAIR(COLOR_GREEN));
				}
				else if (px[i][j].mapa == '$') {
					attron(COLOR_PAIR(COLOR_BLUE));
					mvaddch(i,j,px[i][j].mapa | A_BOLD); // o power up de dano tem a sua iluminação e cor única
					attroff(COLOR_PAIR(COLOR_BLUE));
				}
				else if (px[i][j].mapa == 'W') {
					attron(COLOR_PAIR('B'));
					mvaddch(i,j,px[i][j].mapa | A_BOLD); // a munição tem a sua iluminação e cor única
					attroff(COLOR_PAIR('B'));
				}
			}
		}

		init_color(45,800,800,800);
		if (jg->bala.ativa==1) { // a bala só é imprimida caso esteja ativa, ou seja, se estiver em causa um disparo
			attron(COLOR_PAIR('B'));
			switch (jg->bala.sentido) { // dependendo do sentido para o qual o jogador está voltado, a bala é imprimida nesse sentido
            	case 'c': mvaddch(jg->bala.balaY,jg->bala.balaX, '^' | A_BOLD); break;
            	case 'b': mvaddch(jg->bala.balaY,jg->bala.balaX, 'v' | A_BOLD); break;
            	case 'e': mvaddch(jg->bala.balaY,jg->bala.balaX, '<' | A_BOLD); break;
            	case 'd': mvaddch(jg->bala.balaY,jg->bala.balaX, '>' | A_BOLD); break;
            	default: break;
        	}
			attroff(COLOR_PAIR('B'));
		}

		init_color(15,0,jg->jogador.vida*8+200,0);
		attron(COLOR_PAIR('G'));
		mvaddch(jg->jogador.playerY, jg->jogador.playerX, '@' | A_BOLD); // print do jogador
		attroff(COLOR_PAIR('G'));
		
		dano_monstro(jg,px); // atualiza vida do jogador de acordo com a função
		mover_monstros(jg,px); // atualiza a posição dos monstros

		attron(COLOR_PAIR(COLOR_RED));
		print_monstros(*jg,px); // print de todos os montros com a cor vermelha
		attroff(COLOR_PAIR(COLOR_RED));

		if (jg->jogador.kills >= 10 && portalAtivo == 0) { // o portal aparece quando o jogador acaba de matar o décimo monstro do nível
			gera_portal(*jg,px);
			portalAtivo = 1; // flag
		}

		update(jg,px,&tempoRapido_partir_paredes,&tempoRapido_iluminar_monstros,&tempoRapido_dano_area); // atualiza a posição do jogador atendendo à função update
	}

	if (jg->highScore <= jg->jogador.killsTotais) // quando o highscore foi ultrapassado
		jg->highScore = jg->jogador.killsTotais; 

	if (jg->jogador.vida <= 0) // apenas quando a vida do jogador for nula ou negativa (jogador morreu) aparece o ecrã de Game Over
		st->ativo = 2;
	else
		st->ativo = 3; // caso o jogador permaneça vivo aparece o ecrã de Próximo Nível

	clear();
	refresh();
}




void drawAjuda(jogo *jg, estado *st) {

	Mix_VolumeMusic(24); // volume da música de fundo a 100%

	attron(A_REVERSE);
    mvprintw(jg->vertical-2, jg->horizontal/2-3,"Voltar"); // botão voltar
    attroff(A_REVERSE);

	mvprintw(1,2,"#MOVIMENTO#");

	mvprintw(3,2, "[NUM_8] ou [KEY_UP]     ||  Mover para cima");
	mvprintw(4,2, "[NUM_2] ou [KEY_DOWN]   ||  Mover para baixo");
	mvprintw(5,2, "[NUM_4] ou [KEY_LEFT]   ||  Mover para a esquerda");
	mvprintw(6,2, "[NUM_6] ou [KEY_RIGHT]  ||  Mover para a direita");
	mvprintw(7,2, "[NUM_7]                 ||  Mover para cima e esquerda");
	mvprintw(8,2, "[NUM_9]                 ||  Mover para cima e direita");
	mvprintw(9,2, "[NUM_1]                 ||  Mover para baixo e esquerda");
	mvprintw(10,2,"[NUM_3]                 ||  Mover para baixo e direita");
	mvprintw(11,2,"[BACK_SPACE]            ||  Fechar o jogo");

	mvprintw(15,2,"#HABILIDADES#");

	mvprintw(17,2,"[Q]  ||  habilidade de quebrar paredes (1)");
	mvprintw(18,2,"[W]  ||  habilidade de visualizar monstros (2)");
	mvprintw(19,2,"[E]  ||  habilidade de dano em área (3)");
	mvprintw(20,2,"[D]  ||  habilidade de disparar (4)");

	mvprintw(24,2,"#DESCRICOES#");

	mvprintw(26,2,"Monstros: - Os monstros são entidades que estão espalhadas pelo mapa, de forma aleatória.");
	mvprintw(27,2,"          - Estes procuram aproximar-se do jogador, quando se encontram no seu campo de visão, para conseguirem matá-lo.");
	mvprintw(28,2,"          - Os seus ataques não são precisos, falhando com frequência.");
	mvprintw(29,2,"          - A precisão de ataque e velocidade dos monstros aumentam com os níveis.");

	mvprintw(31,2,"Power Ups/ Munição: - Existem dois tipos de Power Ups, um tipo fornece vida instantânea ao jogador ('+') e outro que fornece um aumento de dano permanente ('$').");
	mvprintw(32,2,"                    - A munição ('W') fornece, ao jogador, o máximo de munição possível, ou seja 30 balas.");
	mvprintw(33,2,"                    - Os Power Ups e a munição têm uma pequena chance de surgir com a morte de um monstro (no local exato onde este morreu).");
	mvprintw(34,2,"                    - O jogador consegue beneficiar dos mesmos, movendo-se para a posição destes.");
	mvprintw(35,2,"                    - Os Power Ups melhoram com os níveis.");

	mvprintw(37,2,"Combate: - O combate é realizado corpo a corpo ou à distância.");
	mvprintw(38,2,"         - Para o jogador causar dano a um único monstro, corpo a corpo, deve mover-se para a posição exata onde este se encontra.");
	mvprintw(39,2,"         - Para o jogador causar dano a um único monstro, à distância, deve voltar-se para o sentido desejado e realizar o disparo.");
	mvprintw(40,2,"         - Caso o jogador pretenda causar dano a diferentes monstros em simultâneo, corpo a corpo, pode recorrer à habilidade de dano em área.");

	mvprintw(42,2,"Objetivo: O objetivo do jogo é sobreviver durante o maior tempo possível, eliminando o máximo de monstros. Para passar para o nível seguinte, o jogador deve matar no mínimo 10 monstros.");
	mvprintw(43,2,"          Quando o jogador perde, é-lhe apresentado o seu score (quantos monstros matou, no total), o seu highscore (o maior score atingido pelo jogador) e o nível máximo atingido.");

	mvprintw(47,2,"(1) (tempo de recarga: 6 segundos)          - Ao usar esta habilidade, o jogador quebra todas as paredes, à sua volta, num alcance de 5 por 5 (exceto as paredes que delimitam o mapa)");
	mvprintw(48,2,"(2) (tempo de recarga: 15 segundos)         - Ao usar esta habilidade, o jogador visualiza todos os monstros ainda vivos, no mapa, por um curto período de tempo");
	mvprintw(49,2,"(3) (tempo de recarga: 10 segundos)         - Ao usar esta habilidade, o jogador golpeia e elimina todos os monstros que se encontram, à sua volta, num alcance de 3 por 3 (desbloqueia no nível 3)");
	mvprintw(50,2,"(4) (tempo de recarga: tempo de trajetória) - Ao usar esta habilidade, o jogador diapara uma bala, que dá dano ao primeiro monstro que atingir, na trajetória desejada (desbloqueia no nível 2)");

	while (st->ativo == 1) {

        switch(getch()) {
            case 13:
                st->ativo = -1; // regressa ao menu principal
				break;
            default: break;
        }
    }
	clear();
	refresh();
}




void drawPerdeu(jogo *jg, estado *st) {
	Mix_VolumeMusic(8); // volume da música de fundo a 25%

	playSom("musica/Game_Over.wav");

	mvprintw(jg->vertical/2,jg->horizontal/2 -20,"Score: %d      High Score: %d      Nível atingido: %d     ",jg->jogador.killsTotais*10,jg->highScore*10,jg->nivel);

	jg->nivel = 1; // reset do nível após o print
	jg->jogador.vida = 100; // reset da vida do jogador após o print
	jg->jogador.dano = 25; // reset do dano do jogador após o print
    st->highlight = 0; // começa com a opção de jogar marcada
	jg->jogador.killsTotais=0; // reset das kills totais após o print

	attron(COLOR_PAIR(COLOR_RED));
	mvprintw(10, jg->horizontal/2-58, " ##########       ######      ####    ####   ############       ##########    ##        ##   ############   ###########");
	mvprintw(11, jg->horizontal/2-58, "############     ########     ## ##  ## ##   ############      ############   ##        ##   ############   ############");
	mvprintw(12, jg->horizontal/2-58, "##              ###    ###    ##  ####  ##   ##                ##        ##   ##        ##   ##             ##        ##");
	mvprintw(13, jg->horizontal/2-58, "##             ##        ##   ##   ##   ##   ##                ##        ##   ##        ##   ##             ##       ##");
	mvprintw(14, jg->horizontal/2-58, "##      ####   ############   ##        ##   #######           ##        ##   ##        ##   #######        #########");
	mvprintw(15, jg->horizontal/2-58, "##     #####   ############   ##        ##   #######           ##        ##   ##        ##   #######        ########");
	mvprintw(16, jg->horizontal/2-58, "##        ##   ##        ##   ##        ##   ##                ##        ##   ###      ###   ##             ##     ##");
	mvprintw(17, jg->horizontal/2-58, "##        ##   ##        ##   ##        ##   ##                ##        ##    ###    ###    ##             ##      ##");
	mvprintw(18, jg->horizontal/2-58, "############   ##        ##   ##        ##   ############      ############     ########     ############   ##       ##");
	mvprintw(19, jg->horizontal/2-58, " ##########    ##        ##   ##        ##   ############       ##########       ######      ############   ##        ##");
	attroff(COLOR_PAIR(COLOR_RED));
    	
	while (st->ativo == 2) {

        for(int i=0;i<2;i++) { 
            if(i == st->highlight)
                attron(A_REVERSE); // highlight da opção
            switch (i) {
                case 0: mvprintw(jg->vertical/2+6, jg->horizontal/2-6,"Jogar novamente"); break;
                case 1: mvprintw(jg->vertical/2+8, jg->horizontal/2-3,"Desistir"); break;
            }
            attroff(A_REVERSE);
        }
        switch(getch()) { // por cada troca, muda o state que está highlighted e toca um som
            case KEY_UP:
                if (st->highlight == 1) {
                    st->highlight = 0;
					playSom("musica/Beep.wav");
				}
                break;
            case KEY_DOWN:
                if (st->highlight == 0) {
                    st->highlight = 1;
					playSom("musica/Beep.wav");
				}
                break;
            case 13:
                if (st->highlight == 0) {
					st->ativo = 0;
                } else {
					st->ativo = -1;
				}
				break;
            default: break;
        }
    }
	clear();
	refresh();
}




void drawNivel(jogo *jg, estado *st) {
	Mix_VolumeMusic(8); // volume da música de fundo a 25%

	playSom("musica/Proximo_Nivel.wav");

	jg->nivel++; // incrementa o nível

	if (jg->nivel==2) {
		attron(COLOR_PAIR(COLOR_WHITE));
		mvprintw(jg->vertical/2+5,jg->horizontal/2-21,"Desbloqueaste uma arma de longo alcance:");
		mvprintw(jg->vertical/2+7,jg->horizontal/2-21,"       Pressiona [D] para disparar");
		attroff(COLOR_PAIR(COLOR_WHITE));
	}

	if (jg->nivel==3) {
		attron(COLOR_PAIR(COLOR_WHITE));
		mvprintw(jg->vertical/2+5,jg->horizontal/2-18,"Desbloqueaste uma nova habilidade:");
		mvprintw(jg->vertical/2+6,jg->horizontal/2-18,"           DANO EM AREA");
		mvprintw(jg->vertical/2+8,jg->horizontal/2-18,"     Pressiona [E] para usar");
		attroff(COLOR_PAIR(COLOR_WHITE));
	}

	attron(COLOR_PAIR('E'));
	mvprintw(13, jg->horizontal/2+15, 										    "###");
	mvprintw(14, jg->horizontal/2+15, 											"######");
	mvprintw(15, jg->horizontal/2+15, 											"#########");
	mvprintw(16, jg->horizontal/2-26, "#####################################################");
	mvprintw(17, jg->horizontal/2-26, "########################################################");
	mvprintw(18, jg->horizontal/2-26, "##########################################################");
	mvprintw(19, jg->horizontal/2-26, "########################################################");
	mvprintw(20, jg->horizontal/2-26, "#####################################################");
	mvprintw(21, jg->horizontal/2+15, 											"#########");
	mvprintw(22, jg->horizontal/2+15, 											"######");
	mvprintw(23, jg->horizontal/2+15, 											"###");
	attroff(COLOR_PAIR('E'));

	double temporizador = 0; // inicializa

	while (st->ativo == 3 && temporizador < 600) {
		attron(COLOR_PAIR(COLOR_WHITE));
		mvprintw(jg->vertical/2,jg->horizontal/2-4,"Nível %d   ",jg->nivel); // print do nível
		attroff(COLOR_PAIR(COLOR_WHITE));
		getch(); // necessário para correr o loop
		temporizador += 0.001; // incrementa
	}

	st->ativo = 0; // volta sempre ao jogo

	clear();
	refresh();
}