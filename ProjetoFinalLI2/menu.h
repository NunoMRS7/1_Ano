/* A função realiza uma ação atendendo ao input do jogador. Caso este seja a
tecla BACKSPACE, a janela fecha e o jogador sai do jogo. Caso seja um número,
chama a função moverJogador para, se puder, mover o jogador para a posição
desejada. Por último, caso seja uma tecla pré-definida com uma habilidade, é
chamada a função que a representa. */
void update();


/* A função começa por inicializar alguns parâmetros, como a vida e dano do 
jogador. De seguida procede ao print do logotipo do jogo e dos elementos do grupo 
responsável. Depois a função entra num loop perpétuo, onde desenha os botões do menu.
O input pelas setas do teclado altera o botão selecionado, sendo tal input responsável
pela ação de tocar um som. Quando o jogador carrega no ENTER, o estado geral do jogo
atualiza para o estado em que o jogador pretende ir, o que faz a sair do loop
em que se encontrava. */
void drawMenu();


/* A função começa por gerar o mapa, posicionar o jogador e mobs, definir o dano 
e vida de tais monstros e inicializar alguns valores, desde parâmetros do jogador a
cores a serem utilizadas. Após isso, a função entra num loop cuja a condição de saída
é feita pela morte do jogador ou pela entrada do mesmo no portal (Apenas aparece
quando o jogador mata 10 mobs). Durante esse loop, a função atualiza constantemente o
mapa atendendo a fatores como a posição do jogador e dos mobs e da visão que o jogador
emite. A função escreve no final da janela informações como o número de kills
e a vida do jogador, e chama outras funções indespensáveis à fluidez do jogo. Quando
finalmente a função sai do loop, o parâmetro de highscore é atualizado, e o state do
jogo é alterado ou para a janela de próximo nível ou para a janela de Game Over. */
void drawJogo();


/* A função limita-se a dar print no ecrâ de tópicos que o grupo acho pretinente 
o jogador saber, havendo o botão sair, que quando acionado, retorna o jogador ao menu. */
void drawAjuda();


/* Esta função é semelhante à função drawMenu, mas ao invés de apresentar o logotipo do jogo,
é printado "GAME OVER" na janela, e existem os botões "Jogar novamente" e "Desistir", ao invés 
dos botões "Jogar", "Ajuda" e "Sair". É também printado o score e highscore do jogador. */
void drawPerdeu();


/* Esta função toca um som e printa na janela um seta assim como o nível para o qual o jogador
está a ir. A função automaticamente altera o state para ser chamada a função drawJogo ao fim 
de um curto período de tempo. */
void drawNivel();