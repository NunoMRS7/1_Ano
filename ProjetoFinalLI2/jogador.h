/* Esta função atualiza a posição do jogador atentando aos valores a
incrementar no X e Y do jogador. Caso a nova posição coincida com um monstro,
o jogador não muda de posição, e é aplicada a função dano_jogador no monstro
em questão. Caso contrário, o jogador pode mover-se, mas antes disso, é
verificada se a posição desejada está interdita por paredes. Depois do jogador
se mover, é verificado se na posição nova este se encontra em cima de algum
powerup, e caso tal se verifique, é aplicado o powerup correspondente (Os
powerups de vida e de dano são aplicados diretamente nos parâmetros do jogador,
aumentando estes de potência dependendo do nível em que o jogador se encontra). */
void moverJogador();


/* A função posiciona o jogador numa posição aleatória, atendendo aos limites da janela
e à presença de paredes. */
void posicionarJogador();
