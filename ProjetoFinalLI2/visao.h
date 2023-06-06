/* A função faz variar um valor raio (máximo 30) em função de um ângulo 
que varia entre 0 e 2PI (Radianos). Em cada iteração, a posição da casa em
questão é dada por r*cos(angulo) para o X e 0.55*r*sin(angulo) para o Y, servindo
o 0.55 para achatar a esfera, dando a aparência de uma visão mais circular no 
jogo. A cada casa é atribuida visão 1, e apenas depois é verificada a existência
de uma parede (isto foi feito de forma a iluminar também a parede). Quando tal
acontece, o raio para de aumentar, saindo do ciclo e recomeçando o processo 
para outro ângulo. */
void visao();