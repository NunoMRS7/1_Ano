/* Quando o poder se encontra passível de se usar, a função troca todas as
paredes por pontos, caso estas não sejam as paredes delimitadores da janela, e
reseta o cooldown do poder e toca um som. */
void parte_paredes();


/* Quando o poder se encontra passível de se usar, a função percorre todo o
array de monstros e altera a sua visão para 1. Reseta também o cooldown do poder
e toca um som. */
void ilumina_monstros();


/* Quando o poder se encontra passível de se usar, a função mata todos os monstros
que se encontrem num raio de 3x3 do jogador, assegurando-se de incrementar o número
de kills e kills totais, e de que há uma chance de o monstro dar "drop" de um powerup ou munição no
local em que morreu. Por último reseta o cooldown do poder e toca um som. */
void dano_em_area();