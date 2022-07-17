#include <stdio.h>
#include "estado.h"

//      IMPRIME O TABULEIRO

void printa(ESTADO e,armJOG a){
    char peca;
    if(e.peca==VALOR_X)peca='X';
    else peca='O';
    int x=1;
    char c = ' ';
    printf("  1 2 3 4 5 6 7 8\n");
    for (int i = 0; i < 8; i++) {
        printf("%d ",x);
        x++;
        for (int j = 0; j < 8; j++) {
            switch (e.grelha[i][j]) {
                case VALOR_O: {
                    c = 'O';
                    break;
                }
                case VALOR_X: {
                    c = 'X';
                    break;
                }
                case VAZIA: {
                    c = '-';
                    break;
                }
                case VALIDO: {
                    c = '.';
                    break;
                }
                case HINT: {
                    c = '?';
                    break;
                }
            }
            printf("%c ", c);
            if(a.pos>0) {
                if (i == 3 && j == 7)printf("        E a vez de :");
                if (i == 4 && j == 7)printf("             %c", peca);
            }
        }
        printf("\n");
    }
    pont(e);
}


//      INDICA AS PONTUAÇÕES ATUAIS DOS JOGADORES

void pont(ESTADO e){
    int l,c,x=0,o=0;
    for(l=0 ; l<8 ; l++){
        for(c=0 ; c<8 ; c++){
            if(e.grelha[l][c]==VALOR_X)x++;
            if(e.grelha[l][c]==VALOR_O)o++;
        }
    }
    printf("   X : %d   O : %d\n",x,o);
}