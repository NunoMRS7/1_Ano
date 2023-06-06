/* Verifica se num dado X e Y, existe algum monstro, devolvendo 1 caso
tal se verifique ou 0 caso não haja nenhum mosntro. Para tal o monstro tem 
de ser visível e ter vida superior a 0. */
int check_if_monstro();


/* Esta função devolve o indice do monstro que se encontra na posição dada.
Caso não exista monstro, a função devolve -1. */
int indice_monstro();


/* Esta função apenas verifica se o jogador se encontra na posição dada. */
int check_if_jogador();


/* Esta função devolve 1 quando a posição dada se encontra num raio de 16x16. */
int proximidadeDoJogador();


/* Semelhantemente à posicionarJogador, esta função posiciona todos os montros numa casa
aleatória, atendendo aos limites do janela, à existênia de paredes e à proximidade inicial 
do jogador. */
void posicionarMonstros();


/* A função printa na janela todos os monstros que tenham visão 1 e vida maior que 0 com a
letra 'M'. */
void print_monstros();


/* A função devolve a distância entre dois pontos. */
double dist();


/* A função percorre todo o array de monstros, havendo uma pequena chance que incrementa
com o nível em que o jogador se encontra de os mover, caso estes tenham visão 1. Para tal,
é guardada a distância do monstro ao jogador assim como a sua posição inicial. De seguida,
são averiguadas todas as casas possíveis para o monstro se mover, verificando qual a casa que
devolve uma distância menor do jogador. Por último, a posição nova do monstro, caso haja uma
casa melhor, é atualizada. */
void mover_monstros();