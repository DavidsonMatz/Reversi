#include <stdio.h>
#include "estado.h"
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#define MAX_BUF 100

//      DECLARAÇÃO DAS FUNÇÕES

FILE *reversi;
ESTADO lerficheiroL();
void escreverficheiroE(ESTADO *e);
ESTADO jogarJ(ESTADO *e, int lin, int col, armJOG *a);
ESTADO grelha_inicial(ESTADO *e, armJOG a);
ESTADO interpretador(ESTADO *e,armJOG a);
void menu();
ESTADO jogPos(ESTADO *e,armJOG a);
void hint_bot(ESTADO *e, armJOG *a);
void incARM(ESTADO *e, armJOG *a);
void undo(ESTADO *e, armJOG *a);
void mudapeca(ESTADO *e);


//      INICIALIZAÇÃO DO ARMAZENAMENTO DOS ESTADOS

void iniciaArm(ESTADO *e,armJOG *a){
    armJOG f;
    f.pos=0;
    f.est[f.pos]=*e;
    *a=f;
}


//      GERADOR DE NÚMEROS PSEUDO-ALEATÓRIOS

int random() {
    int x=rand() % 1000;
    return x;
}


//      INDICA QUAIS SÃO AS JOGADAS VALIDAS

int valido(ESTADO *e){
    int linha,coluna,l,c,r = 0;
    VALOR a;
    VALOR b;
    if (e->peca == VALOR_X) {
        a = VALOR_X;
        b = VALOR_O;
    }
    else {
        a = VALOR_O;
        b = VALOR_X;
    }
    for(linha=0 ; linha<8 ; linha++){
        for(coluna=0 ; coluna<8 ; coluna++){
            if (e->grelha[linha][coluna] == VAZIA){
                if(e->grelha[linha-1][coluna-1] == b) {
                    l = linha - 1;
                    c = coluna - 1;
                    while (0 <= l && l < 8 && 0 <= c && c < 8 && e->grelha[l][c]!=VAZIA){
                        if (e->grelha[l][c] == a){
                            e->grelha[linha][coluna]=VALIDO;
                            r=1;
                            break;
                        }
                        l--;
                        c--;
                    }
                }
                if(e->grelha[linha-1][coluna] == b){
                    l = linha - 1;
                    c = coluna;
                    while (0 <= l && l < 8 && 0 <= c && c < 8 && e->grelha[l][c]!=VAZIA){
                        if (e->grelha[l][c] == a){
                            e->grelha[linha][coluna]=VALIDO;
                            r=1;
                            break;
                        }
                        l--;
                    }
                }
                if(e->grelha[linha-1][coluna+1]== b){
                    l = linha - 1;
                    c = coluna + 1;
                    while (0 <= l && l < 8 && 0 <= c && c < 8 && e->grelha[l][c]!=VAZIA){
                        if (e->grelha[l][c] == a){
                            e->grelha[linha][coluna]=VALIDO;
                            r=1;
                            break;
                        }
                        l--;
                        c++;
                    }
                }
                if(e->grelha[linha][coluna-1]== b){
                    l = linha;
                    c = coluna-1;
                    while (0 <= l && l < 8 && 0 <= c && c < 8 && e->grelha[l][c]!=VAZIA){
                        if (e->grelha[l][c] == a){
                            e->grelha[linha][coluna]=VALIDO;
                            r=1;
                            break;
                        }
                        c--;
                    }
                }
                if(e->grelha[linha][coluna+1]== b){
                    l = linha;
                    c = coluna + 1;
                    while (0 <= l && l < 8 && 0 <= c && c < 8 && e->grelha[l][c]!=VAZIA){
                        if (e->grelha[l][c] == a){
                            e->grelha[linha][coluna]=VALIDO;
                            r=1;
                            break;
                        }
                        c++;
                    }
                }
                if(e->grelha[linha+1][coluna-1]== b){
                    l = linha + 1;
                    c = coluna - 1;
                    while (0 <= l && l < 8 && 0 <= c && c < 8 && e->grelha[l][c]!=VAZIA){
                        if (e->grelha[l][c] == a){
                            e->grelha[linha][coluna]=VALIDO;
                            r=1;
                            break;
                        }
                        l++;
                        c--;
                    }
                }
                if(e->grelha[linha+1][coluna]== b){
                    l = linha + 1;
                    c = coluna;
                    while (0 <= l && l < 8 && 0 <= c && c < 8 && e->grelha[l][c]!=VAZIA){
                        if (e->grelha[l][c] == a){
                            e->grelha[linha][coluna]=VALIDO;
                            r=1;
                            break;
                        }
                        l++;
                    }
                }
                if(e->grelha[linha+1][coluna+1]== b){
                    l = linha + 1;
                    c = coluna + 1;
                    while (0 <= l && l < 8 && 0 <= c && c < 8 && e->grelha[l][c]!=VAZIA){
                        if (e->grelha[l][c] == a){
                            e->grelha[linha][coluna]=VALIDO;
                            r=1;
                            break;
                        }
                        l++;
                        c++;
                    }
                }
            }
        }
    }
    return r;
}


//      TESTA SE O JOGO CHEGOU AO FIM (SE UM JOGADOR FICOU SEM PEÇAS EM CAMPO OU SE TODAS AS POSIÇÕES ESTIVEREM OCUPADAS)

int fimdojogo(ESTADO *e) {
    int l, c, x = 0, y = 0, z = 0, r = 0;
    for (l = 0 ; l < 8 ; l++){
        for (c = 0 ; c < 8 ; c++){
            if(e->grelha[l][c] == VALOR_X)x++;
            if(e->grelha[l][c] == VALOR_O)y++;
            if(e->grelha[l][c]==VAZIA)z++;
        }
    }
    if(x==0){
        printf("'O' ganhou\n");
        r=1;
    }
    if(y==0){
        printf("'X' ganhou\n");
        r=1;
    }
    if(z==0 && x!=0 && y!=0){
        r=1;
        if(x>y)printf("'X' ganhou\n");
        if(x<y)printf("'O' ganhou\n");
        if(x==y)printf("empate\n");
    }
    return r;
}


//      EFETUA AS JOGADAS

int jogada(ESTADO *e, int lin, int col){
    VALOR a;
    VALOR b;
    if (e->peca == VALOR_X) {
        a = VALOR_X;
        b = VALOR_O;
    }
    else {
        a = VALOR_O;
        b = VALOR_X;
    }
    int l,c,x,y,r;
    r=0;
    //void trocapeca(int linha, int coluna){
    //int x,y;
    //x=linha;
    //c=coluna;
    //while (0 <= linha && linha < 8 && 0 <= coluna && coluna < 8 && e->grelha[l][c]!=VAZIA){
    //if(e->grelha[linha][coluna] == a){
    //while(x != l || y != c){
    //                        e->grelha[x][y]=a;
    //                        x--;
    //                        y--;
    // }
    //                    r = 1;
    //                    break;
    //                }
    // l--;
    //c--;
    //}
    if ((e->grelha[lin][col] == VAZIA) && 0 <= lin && lin < 8 && 0 <= col && col < 8){
        if(e->grelha[lin-1][col-1] == b){
            l = lin - 1;
            c = col - 1;
            x = l;
            y = c;
            while (0 <= l && l < 8 && 0 <= c && c < 8 && e->grelha[l][c]!=VAZIA){
                if (e->grelha[l][c] == a){
                    while(x != l || y != c){
                        e->grelha[x][y]=a;
                        x--;
                        y--;
                    }
                    r = 1;
                    break;
                }
                l--;
                c--;
            }
        }
        if(e->grelha[lin-1][col] == b){
            l = lin - 1;
            c = col;
            x = l;
            y = c;
            while (0 <= l && l < 8 && 0 <= c && c < 8 && e->grelha[l][c]!=VAZIA){
                if (e->grelha[l][c] == a){
                    while(x != l || y != c){
                        e->grelha[x][y]=a;
                        x--;
                    }
                    r = 1;
                    break;
                }
                l--;
            }
        }
        if(e->grelha[lin-1][col+1]== b){
            l = lin - 1;
            c = col + 1;
            x = l;
            y = c;
            while (0 <= l && l < 8 && 0 <= c && c < 8 && e->grelha[l][c]!=VAZIA){
                if (e->grelha[l][c] == a){
                    while(x != l || y != c){
                        e->grelha[x][y]=a;
                        x--;
                        y++;
                    }
                    r = 1;
                    break;
                }
                l--;
                c++;
            }
        }
        if(e->grelha[lin][col-1]== b){
            l = lin;
            c = col-1;
            x = l;
            y = c;
            while (0 <= l && l < 8 && 0 <= c && c < 8 && e->grelha[l][c]!=VAZIA){
                if (e->grelha[l][c] == a){
                    while(x != l || y != c){
                        e->grelha[x][y]=a;
                        y--;
                    }
                    r = 1;
                    break;
                }
                c--;
            }
        }
        if(e->grelha[lin][col+1]== b){
            l = lin;
            c = col + 1;
            x = l;
            y = c;
            while (0 <= l && l < 8 && 0 <= c && c < 8 && e->grelha[l][c]!=VAZIA){
                if (e->grelha[l][c] == a){
                    while(x != l || y != c){
                        e->grelha[x][y]=a;
                        y++;
                    }
                    r = 1;
                    break;
                }
                c++;
            }
        }
        if(e->grelha[lin+1][col-1]== b){
            l = lin + 1;
            c = col - 1;
            x = l;
            y = c;
            while (0 <= l && l < 8 && 0 <= c && c < 8 && e->grelha[l][c]!=VAZIA){
                if (e->grelha[l][c] == a){
                    while(x != l || y != c){
                        e->grelha[x][y]=a;
                        x++;
                        y--;
                    }
                    r = 1;
                    break;
                }
                l++;
                c--;
            }
        }
        if(e->grelha[lin+1][col]== b){
            l = lin + 1;
            c = col;
            x = l;
            y = c;
            while (0 <= l && l < 8 && 0 <= c && c < 8 && e->grelha[l][c]!=VAZIA){
                if (e->grelha[l][c] == a){
                    while(x != l || y != c){
                        e->grelha[x][y]=a;
                        x++;
                    }
                    r = 1;
                    break;
                }
                l++;
            }
        }
        if(e->grelha[lin+1][col+1]== b){
            l = lin + 1;
            c = col + 1;
            x = l;
            y = c;
            while (0 <= l && l < 8 && 0 <= c && c < 8 && e->grelha[l][c]!=VAZIA){
                if (e->grelha[l][c] == a){
                    while(x != l || y != c){
                        e->grelha[x][y]=a;
                        x++;
                        y++;
                    }
                    r = 1;
                    break;
                }
                l++;
                c++;
            }
        }
    }
    if(r==1)e->grelha[lin][col]=e->peca;
    return r;
}


//      CRIA UM TABULEIRO NOVO
ESTADO grelha_inicial(ESTADO *e,armJOG a){
    ESTADO z ={0};
    z.grelha[3][4] = VALOR_X;
    z.grelha[4][3] = VALOR_X;
    z.grelha[3][3] = VALOR_O;
    z.grelha[4][4] = VALOR_O;
    *e=z;
    printf("\n");
    printa(*e,a);
    printf("\n");
    return(*e);
}


//      INDICA AS OPÇÕES DO JOGADOR

void menu(){
    printf("Menu:\n");
    printf("N <peca> -> novo jogo em que o primeiro a jogar e o jogador com peca <peca>;\n");
    printf("L <ficheiro> -> ler o jogo de um ficheiro\n");
    printf("E <ficheiro> -> gravar o jogo num ficheiro\n");
    printf("J <L> <C> -> jogar peca atual na posicao (L,C)\n");
    printf("S -> jogadas validas\n");
    printf("H -> sugestao de jogada.\n");
    printf("U -> desfazer a ultima jogada (Undo)\n");
    printf("A <peca> <nivel> -> novo jogo contra 'bot', o computador joga com a peca <peca> com dificuldade <nivel>\n");
    printf("Q -> sair\n");
}


//      LE UM FICHEIRO DE TEXTO E CARREGA O JOGO

ESTADO lerficheiroL(){
    ESTADO e ={0};
    armJOG a;
    char modo[1],peca[1],dif[1],grelha[1];
    reversi = fopen("reversi.txt", "r");
    fscanf(reversi,"%c %c",modo,peca);
    if(modo[0]=='A')e.modo = 1;
    else e.modo = 0;
    if(peca[0]=='X')e.peca=VALOR_X;
    else e.peca=VALOR_O;
    if(e.modo==1)fscanf(reversi, " %c\n" ,dif);
    else fscanf(reversi,"\n");
    if(dif[0]=='1')e.dif=1;
    if(dif[0]=='2')e.dif=2;
    if(dif[0]=='3')e.dif=3;
    for(int l=0; l<8 ; l++){
        for(int c=0; c<8 ; c++){
            fscanf(reversi, "%c " ,grelha);
            if(grelha[0]=='X')e.grelha[l][c]=VALOR_X;
            if(grelha[0]=='O')e.grelha[l][c]=VALOR_O;
            if(grelha[0]=='-')e.grelha[l][c]=VAZIA;
        }
        fscanf(reversi,"\n");
    }
    fclose(reversi);
    iniciaArm(&e,&a);
    printa(e,a);
    menu();
    interpretador(&e,a);
    return e;
}


//      GRAVA O JOGO NUM FICHEIRO DE TEXTO
void escreverficheiroE(ESTADO *e){
    char x=0;
    char modo = e->modo;
    char peca;
    char dif=e->dif;
    if(modo==1)modo='A';
    else modo ='M';
    if (e->peca==VALOR_X)peca='X';
    else peca='O';
    reversi = fopen("reversi.txt", "w");
    fprintf(reversi,"%c %c",modo , peca);
    if(e->modo==1)fprintf(reversi," %d\n",dif);
    else fprintf(reversi,"\n");
    for(int l=0; l<8 ; l++){
        for(int c=0; c<8 ; c++){
            if(e->grelha[l][c]==VALOR_X)x='X';
            if(e->grelha[l][c]==VALOR_O)x='O';
            if(e->grelha[l][c]==VAZIA)x='-';
            fprintf(reversi,"%c ",x);
        }
        fprintf(reversi,"\n");
    }
    fclose(reversi);
}


//      VALIDA AS JOGADAS

ESTADO jogarJ(ESTADO *e, int lin, int col, armJOG *a){
    if(jogada(e,lin,col)==1){
        mudapeca(e);
        incARM(e,a);
        if(fimdojogo(e)==1){
            printa(*e,*a);
            printf("FIM DO JOGO\n");
            menu();
            return *e;
        }
        if(valido(e)==0 && fimdojogo(e)==0){
            incARM(e,a);
            mudapeca(e);
            printf("\nsem jogadas possiveis, a passar a vez...\n\n");
        }
        for(int l=0 ; l<8 ; l++){
            for(int c=0 ; c<8 ; c++){
                if (e->grelha[l][c]==VALIDO)e->grelha[l][c]=VAZIA;
            }
        }
        printa(*e,*a);
        if(e->peca!=e->pecabot)menu();
    }
    else{
        printf("jogada invalida\n");
        printa(*e,*a);
        menu();
    }
    if(e->modo==1 && e->peca==e->pecabot)hint_bot(e,a);
    return(*e);
}


//      TESTA SE HÁ JOGADAS VÁLIDAS

ESTADO jogPos(ESTADO *e,armJOG a){
    ESTADO *z=e;
    if(valido(z)==1){
        printa(*z,a);
        menu();
    }
    else{
        printf("nao ha jogadas validas\n");
        printa(*z,a);
        menu();
    }
    for(int l=0 ; l<8 ; l++){
        for(int c=0 ; c<8 ; c++){
            if (e->grelha[l][c]==VALIDO)e->grelha[l][c]=VAZIA;
        }
    }
    return(*e);
}


//      ATRASA A JOGADA DO BOT

void delay(int segundos){
    int milissegundos = 1000 * segundos;
    clock_t start_time = clock();
    while (clock() < start_time + milissegundos)
        ;
}

//      SUGERE JOGADAS E INDICA A JOGADA DO BOT

void hint_bot(ESTADO *e, armJOG *arm){
    int x,y,l,c,lin,col,linhag=0,colunag=0,linhap=0,colunap=0,numAgr,numg=0,nump=64;
    VALOR a;
    VALOR b;
    if (e->peca == VALOR_X) {
        a = VALOR_X;
        b = VALOR_O;
    }
    else {
        a = VALOR_O;
        b = VALOR_X;
    }
    for(lin = 0 ; lin < 8 ; lin++){
        for(col = 0 ; col < 8 ; col++){
            numAgr=0;
            if (e->grelha[lin][col] == VAZIA){
                if(e->grelha[lin-1][col-1] == b) {
                    l = lin - 1;
                    c = col - 1;
                    x = l;
                    y = c;
                    while (0 <= l && l < 8 && 0 <= c && c < 8 && e->grelha[l][c]!=VAZIA){
                        if (e->grelha[l][c] == a){
                            while(x != l || y != c){
                                x--;
                                y--;
                                numAgr++;
                            }
                            break;
                        }
                        l--;
                        c--;
                    }
                }
                if(e->grelha[lin-1][col] == b){
                    l = lin - 1;
                    c = col;
                    x = l;
                    y = c;
                    while (0 <= l && l < 8 && 0 <= c && c < 8 && e->grelha[l][c]!=VAZIA){
                        if (e->grelha[l][c] == a){
                            while(x != l || y != c){
                                x--;
                                numAgr++;
                            }
                            break;
                        }
                        l--;
                    }
                }
                if(e->grelha[lin-1][col+1]== b){
                    l = lin - 1;
                    c = col + 1;
                    x = l;
                    y = c;
                    while (0 <= l && l < 8 && 0 <= c && c < 8 && e->grelha[l][c]!=VAZIA){
                        if (e->grelha[l][c] == a){
                            while(x != l || y != c){
                                x--;
                                y++;
                                numAgr++;
                            }
                            break;
                        }
                        l--;
                        c++;
                    }
                }
                if(e->grelha[lin][col-1]== b){
                    l = lin;
                    c = col-1;
                    x = l;
                    y = c;
                    while (0 <= l && l < 8 && 0 <= c && c < 8 && e->grelha[l][c]!=VAZIA){
                        if (e->grelha[l][c] == a){
                            while(x != l || y != c){
                                y--;
                                numAgr++;
                            }
                            break;
                        }
                        c--;
                    }
                }
                if(e->grelha[lin][col+1]== b){
                    l = lin;
                    c = col + 1;
                    x = l;
                    y = c;
                    while (0 <= l && l < 8 && 0 <= c && c < 8 && e->grelha[l][c]!=VAZIA){
                        if (e->grelha[l][c] == a){
                            while(x != l || y != c){
                                y++;
                                numAgr++;
                            }
                            break;
                        }
                        c++;
                    }
                }
                if(e->grelha[lin+1][col-1]== b){
                    l = lin + 1;
                    c = col - 1;
                    x = l;
                    y = c;
                    while (0 <= l && l < 8 && 0 <= c && c < 8 && e->grelha[l][c]!=VAZIA){
                        if (e->grelha[l][c] == a){
                            while(x != l || y != c){
                                x++;
                                y--;
                                numAgr++;
                            }
                            break;
                        }
                        l++;
                        c--;
                    }
                }
                if(e->grelha[lin+1][col]== b){
                    l = lin + 1;
                    c = col;
                    x = l;
                    y = c;
                    while (0 <= l && l < 8 && 0 <= c && c < 8 && e->grelha[l][c]!=VAZIA){
                        if (e->grelha[l][c] == a){
                            while(x != l || y != c){
                                x++;
                                numAgr++;
                            }
                            break;
                        }
                        l++;
                    }
                }
                if(e->grelha[lin+1][col+1]== b){
                    l = lin + 1;
                    c = col + 1;
                    x = l;
                    y = c;
                    while (0 <= l && l < 8 && 0 <= c && c < 8 && e->grelha[l][c]!=VAZIA){
                        if (e->grelha[l][c] == a){
                            while(x != l || y != c){
                                x++;
                                y++;
                                numAgr++;
                            }
                            break;
                        }
                        l++;
                        c++;
                    }
                }
            }
            if(numAgr>numg){
                numg=numAgr;
                linhag=lin;
                colunag=col;
            }
            if(numAgr<nump && numAgr>0){
                nump=numAgr;
                linhap=lin;
                colunap=col;
            }
        }
    }
    if(e->modo==0 || (e->modo==1 && e->peca!=e->pecabot))e->grelha[linhag][colunag]=HINT;
    if(e->modo==1 && e->pecabot==e->peca && e->dif==1){
        delay(2);
        jogarJ(e,linhap,colunap,arm);
        return;
    }
    if(e->modo==1 && e->pecabot==e->peca && e->dif==2) {
        if (rand() < 500) {
            delay(2);
            jogarJ(e, linhag, colunag, arm);
    }
        else {
            delay(2);
            jogarJ(e, linhap, colunap, arm);
        }
        return;
    }
    if(e->modo==1 && e->pecabot==e->peca && e->dif==3){
        delay(2);
        jogarJ(e,linhag,colunag,arm);
        return;
    }
    printa(*e,*arm);
    for(lin=0 ; lin<8 ; lin++){
        for(col=0 ; col<8 ; col++){
            if(e->grelha[lin][col]==HINT)e->grelha[lin][col]=VAZIA;
        }
    }
    menu();
}


//      RETROCEDE UMA JOGADA

void undo(ESTADO *e,armJOG *a){
    if((e->modo==0 && a->pos<=0) || (e->modo==1 && a->pos<=1))printf("Nao e possivel recuar mais\n");
    if(a->pos>0 && e->modo==0){
        if(e->peca==VALOR_X)a->est[a->pos-1].peca=VALOR_O;
        if(e->peca==VALOR_O)a->est[a->pos-1].peca=VALOR_X;
        *e = a->est[a->pos - 1];
        a->pos--;
    }
    if(a->pos>1 && e->modo==1){
        *e=a->est[a->pos-2];
        a->pos=a->pos-2;
    }
    printa(*e,*a);
    menu();
}


//      GRAVA O ESTADO DO JOGO NUMA LISTA

void incARM(ESTADO *e, armJOG *a){
    a->pos++;
    a->est[a->pos]=*e;
}


//      TROCA DE TURNOS

void mudapeca(ESTADO *e){
    if(e->peca==VALOR_X) e->peca=VALOR_O;
    else e->peca=VALOR_X;
}


//      ACEITA E INTERPRETA O INPUT DO UTILIZADOR

ESTADO interpretador(ESTADO *e, armJOG a){
    char cmd[MAX_BUF];
    char dif[MAX_BUF];
    char peca[MAX_BUF];
    char linha[MAX_BUF];
    int lin[MAX_BUF];
    int col[MAX_BUF];
    fgets(linha,sizeof(linha),stdin);
    sscanf(linha,"%s",cmd);
    switch(toupper(cmd[0])){
        case 'N':
            sscanf(linha, "%s %s", cmd, peca);
            e->modo = 0;
            switch (toupper(peca[0])) {
                case 'X': {
                    grelha_inicial(e,a);
                    e->peca = VALOR_X;
                    iniciaArm(e,&a);
                    menu();
                    break;
                }
                case 'O': {
                    grelha_inicial(e,a);
                    e->peca = VALOR_O;
                    iniciaArm(e,&a);
                    menu();
                    break;
                }
                default:
                    printf("Escolha uma peca: 'X' ou 'O'\n");
                    interpretador(e,a);
            }
            break;
        case 'L':
            lerficheiroL();
            break;
        case 'E':
            escreverficheiroE(e);
            printa(*e,a);
            menu();
            break;
        case 'J':
            sscanf(linha,"%s %d %d",cmd,lin,col);
            if(lin[0] < 1 || lin[0]>8 || col[0]<1 || col[0]>8){
                printf("Comando Invalido\n");
                interpretador(e,a);
            }
            jogarJ(e,(lin[0])-1,(col[0])-1,&a);
            break;
        case 'S':
            jogPos(e,a);
            break;
        case 'H':
            hint_bot(e,&a);
            break;
        case 'U':
            undo(e,&a);
            break;
        case 'A':
            sscanf(linha, "%s %s %s", cmd, peca, dif);
            ESTADO est = {0};
            *e=est;
            e->grelha[3][4] = VALOR_X;
            e->grelha[4][3] = VALOR_X;
            e->grelha[3][3] = VALOR_O;
            e->grelha[4][4] = VALOR_O;
            e->modo=1;
            e->peca=VALOR_X;
            iniciaArm(e,&a);
            switch (toupper(peca[0])){
                case 'X':
                    iniciaArm(e,&a);
                    e->pecabot=VALOR_X;
                    if(dif[0]=='1')e->dif=1;
                    if(dif[0]=='2')e->dif=2;
                    if(dif[0]=='3')e->dif=3;
                    if(dif[0]!= '1' && dif[0]!='2' && dif[0]!='3'){
                        printf("Tem de escolher uma dificuldade!\n");
                        interpretador(e,a);
                    }
                    printa(*e,a);
                    hint_bot(e,&a);
                    break;
                case 'O':
                    iniciaArm(e,&a);
                    e->pecabot=VALOR_O;
                    if(dif[0]=='1')e->dif=1;
                    if(dif[0]=='2')e->dif=2;
                    if(dif[0]=='3')e->dif=3;
                    if(dif[0]!= '1' && dif[0]!='2' && dif[0]!='3'){
                        printf("Tem de escolher uma dificuldade!\n");
                        interpretador(e,a);
                    }
                    printa(*e,a);
                    menu();
                    break;
                default:
                    printf("Escolha a peca do bot: 'X' ou 'O'\n");
            }
            random();
            break;
        case 'Q':
            exit(0);
        default:
            printf("Escolha uma opcao valida\n");
            interpretador(e,a);
    }
    interpretador(e,a);
    return(*e);
}


//      INÍCIO DO PROGRAMA

int main() {
    ESTADO e = {0};
    srand(time(0));
    armJOG a;
    a.pos=0;
    printf("REVERSI\n");
    menu();
    interpretador(&e,a);
    return 0;
}