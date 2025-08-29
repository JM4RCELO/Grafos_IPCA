#ifndef HEADER_H // PROTEÇÃO PARA NÃO INCLUIR O HEADER MAIS QUE UMA VEZ
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <ctype.h>
//DEFINIÇÃO DO CÓDIGO DA COR
#define RED     12
#define GREEN   10
#define YELLOW  14
#define BLUE    9
#define MAGENTA 13
#define CYAN    11
#define WHITE   15

typedef struct adjacentes
{
    int verticeadjacente;
    struct adjacentes* seguinte;
} *adjacentes;

typedef struct antenas
{
    char freq;
    int coordenadas;
    struct antenas* seguinte;
} * antenas;


void corletra(int cor);
int contarantenas(antenas lista);
int contaradj(adjacentes lista);
void liberar_memoria_adjacentes(adjacentes lista);
void libertar_memoria_antenas(antenas lista);
int n_colunas(char ficheiro[]);
int n_linhas(char ficheiro[]);
int contarRegistos(antenas lista);
void impressao_dados_antenas(antenas mapa);
void impressao_mapa_das_antenas(antenas mapa, char ficheiro[]);
void recursiva_profundidade(antenas mapa, int vertice, int visitados[], int caminho[], int nivel);
void procuraProfundidade(antenas mapa, int partida);
void procuraLargura(antenas mapa, int vertice);
void imprimirAdjacentes(antenas mapa);
void intersecao(antenas mapa, int tamanho);
antenas adicionarAdjacentes(antenas mapa);

int sistema();

antenas ler_ficheiro(FILE *cidade, antenas mapa);
antenas inserir_antena(antenas mapa);
antenas remover_antena(antenas mapa);

#endif // HEADER_H