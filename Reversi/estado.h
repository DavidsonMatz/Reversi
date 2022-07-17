#ifndef PROJ_ESTADO_H
#define PROJ_ESTADO_H


//              DEFINIÇÃO E REPRESENTAÇÃO MATRICIAL DO TABULEIRO



//      DEFINIÇÃO DE VALORES POSSÍVEIS NO TABULEIRO

typedef enum {VAZIA, VALOR_X, VALOR_O, VALIDO, HINT} VALOR;


//      ESTRUTURA QUE ARMAZENA O ESTADO DO JOGO

typedef struct estado {
    VALOR peca; // peça do jogador que vai jogar!
    VALOR pecabot; // peca do bot
    VALOR grelha[8][8]; // posições
    char modo; // modo em que se está a jogar! 0-> manual, 1-> contra computador
    char dif; // dificuldade do bot 1-3
} ESTADO;


//      ESTRUTURA QUE ARMAZENA AS JOGADAS

typedef struct armjog {
    int pos;
    ESTADO est[64];
}armJOG;

void printa(ESTADO,armJOG);
void pont(ESTADO);


#endif //PROJ_ESTADO_H