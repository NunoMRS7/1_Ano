/* A função prenche todos os caracteres de uma janela, atendendo ao seu tamanho.
Para tal, numa dada casa, há uma chance de 43% de a casa ser uma parede e 57%
de chance de ser uma casa vazia. Há que notar que existem cláusulas que obrigam
a presença de paredes a rodear os limites da janela. */
void gerarMapa();


/* Para uma certa casa, esta função devolve o número de paredes existentes num
raio de 3x3. */
void tres_por_tres();


/* Esta função percorre a janela toda e, caso numa casa da janela haja mais do
que 4 casas com parede, valor conhecido fruto da função tres_por_tres, essa
posição é definida como parede, sendo isto feito numa matriz auxiliar para manter a
informação da matriz inicial. No final a matriz original copia todos os elementos
da matriz auxiliar. Todo este processo é feito 4 vezes para obter um mapa visualmente
mais limpo. */
void conserta_mapa_tres_por_tres();


/* Atendo ao estado do mapa, isto é, à existência de paredes, à posição do 
jogador, à posição dos monstros e aos limites da janela, a função posiciona um 
portal numa casa aleatória, nas proximidades do jogador. */
void gera_portal();