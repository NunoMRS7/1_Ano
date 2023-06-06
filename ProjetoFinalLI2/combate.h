/* Esta função atribui os valores de vida e de dano a todos os monstros,
sendo estes valores aleatórios, incrementando com o nível em que o jogador
se encontra. */
void inicializa_dano_vida_monstros();


/* A função aplica dano dado pelo monstro ao jogador, caso um monstro se encontre
num raio 3x3 do jogador. Este dano não é garantido, havendo, para tal, uma
percentagem que incrementa com o nível em que o jogador se encontra. Caso o 
monstro dê dano, é tocado um som. */
void dano_monstro();


/* Esta função é chamada quando um jogador se move para uma casa onde se encontra
um monstro, sendo por isso que a função recebe um inteiro que representa o indice
do monstro a levar dano. Para tal, há uma chance fixa de 80% de o jogador dar dano
ao monstro, onde toca um som. Caso o monstro morra com o ataque, o som tocado é
diferente e, para além disso, são incrementados os parâmetros kills e killsTotais do
jogador. Há também uma chance de 20% de o monstro dar "drop" de um powerup e 30% de dar
"drop" de munição, que é colocado na posição onde o monstro morreu. */
void dano_jogador();


/* Esta função é chamada quando uma bala se encontra ativa, ou seja, em trajetória.
Esta permite que a bala se mova tendo em conta o último sentido registado pelo jogador,
antes do disparo. Caso seja detetada uma parede ou monstro, a bala colide e desaparece
(fica inativa), sendo que, no caso do monstro, esta provoca-lhe também algum dano, ou até
mesmo a sua morte, onde toca um som. Caso o monstro morra com o ataque são incrementados
os parâmetros kills e killsTotais do jogador. Há também uma chance de 20% de o monstro dar
"drop" de um powerup e 30% de dar "drop" de munição, que é colocado na posição onde o monstro
morreu. */
void move_bala();