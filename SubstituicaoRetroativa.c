#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* Se houver memória disponível, 
aloca dinâmicamente uma matriz de double com l linhas e c colunas e devolve um ponteiro para a matriz, 
caso contrário, devolve um ponteiro nulo. */
double **alocaMatriz(int l, int c) {
    double **m;
    int i, j;

    m = malloc(sizeof(m)*l);
    
    if(m != NULL) {
        for(i = 0; i < c; i++) {
            m[i] = malloc(sizeof(double)*c);
            
            if(m[i] == NULL) { //falta de memória
                for(j = 0; j < i; j++) free(m[j]);
                free(m);
                return NULL;
            }
        }
    }

    return m;
}

/* Lê valores para uma matriz de double com l linhas e c colunas que foi alocada dinamicamente*/
void lerMatriz(double **m, int l, int c) {
    int i, j;
    
    for(i = 0; i < l; i++) {
        for(j = 0; j < c; j++) {
            printf("m[%d][%d]: ", i+1, j+1);
            scanf("%lf", &m[i][j]);
        }
    }
}

/* Imprime os valores de uma matrix com l linhas e c colunas*/
void imprimeMatriz(double **m, int l, int c) {
    int i, j;
    
    for(i = 0; i < l; i++) {
        for(j = 0; j < c; j++) printf("%10.3lf ", m[i][j]);
        printf("\n");
    }
}

/* Recebe m (a matriz aumentada de um sistema linear triângular superior) com n variáveis.
Se o sistema linear for determinado, armazena em x a solução do sisrema linear e devolve 0.
Se o SL for indeterminado, armazena em x uma soluçao do SL (variáveis livres = 0) e devolve 1.
Se for incompatível, devolve 2.*/
int subsRetroativa(double **m, int n, double x[]) {
    int i, j, tipoSL = 0;
    double soma;

    for(i = n-1; i>=0; i--) {
        soma = 0;
        for(j = i+1; j<n; j++) soma += m[i][j] * x[j];
        if (m[i][i] == 0) {
            if (fabs(m[i][n] - soma) > __FLT16_EPSILON__) return 2; // SL incompatível
            else { // x[i] -> variável livre
                x[i] = 0;
                tipoSL = 1;
            }
        } 
        else x[i] = (m[i][n] - soma) / m[i][i]; // calculo do valor de x[i] 
    }

    return tipoSL;
}

int main() {
    int n, tipo, i;
    double **a, *x;

    printf("Quantidade de variáveis: ");
    scanf("%d", &n);

    a = alocaMatriz(n, n+1);
    x = malloc(sizeof(double)*n);

    if(a == NULL || x == NULL) {
        printf("Deu pau na alocaçaão de memória"); 
        return 1;
    }

    lerMatriz(a, n, n+1);
    imprimeMatriz(a, n, n+1);

    tipo = subsRetroativa(a, n, x);

    if(tipo == 2) printf("Sistem Linear incompatível \n");
    else {
        printf("SL %sdeterminado \n", tipo ? "in" : "");
        for(i = 0; i < n; i++) {
            printf("x[%d] = %10.3lf \n", i+1, x[i]);
        }
    }

    return 0;
}