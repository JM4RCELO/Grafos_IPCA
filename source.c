/**
 * @brief Trabalho sobre antenas de telecomunicações com grafos
 * @author João Marques - 31067 EIM-IPCA
 * @date 2023-10-03
 * @details Este trabalho consiste na leitura de um ficheiro com dados sobre antenas de telecomunicações e a sua 
 * representação em memória através de listas ligadas. O programa permite a leitura do ficheiro, a impressão do mapa das antenas, 
 * a procura em profundidade e largura e a interseção de antenas.
 * @bug nas interseções não é validado se a interseção acontece apenas no segmento de reta entre as antenas 
 * ou se se interseccionam fora do segmento de reta. No entanto é validado se essa interseção acontece dentro do mapa.
 * @bug a função de traçar caminhos ainda não está implementada.
 * @
 */

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <ctype.h>
#include <math.h>

/**
 * @brief  Definição do código da cor para o terminal.
 * 
 */
#define RED     12
#define GREEN   10
#define YELLOW  14
#define BLUE    9
#define MAGENTA 13
#define CYAN    11
#define WHITE   15

#define TAM 50

/**
 * @brief Definição da estrutura de dados para as antenas.
 */
typedef struct antenas
{
    int verticeantena;
    char freq;
    int coordenadas;
    struct antenas* seguinte;
    struct adjacentes* listaadjacentes;         /// Ponteiro para a lista de adjacentes
} * antenas;

/**
 * @brief Definição da estrutura de dados para os adjacentes.
 * @details Esta estrutura contém um ponteiro para o próximo adjacente e o número do vértice adjacente.
 */
typedef struct adjacentes
{
    int verticeadjacente;
    struct adjacentes* seguinte;
} *adjacentes;

/**
 * @brief Definição da estrutura de dados para o caminho.
 * @details Esta estrutura contém um ponteiro para o próximo adjacente e o número do vértice adjacente.
 */
// typedef struct caminho
// {
//     int vertice;
//     int verticeadjacente;
//     struct caminho* seguinte;
// } *caminho;

/**
 * @brief Função para contar o número de antenas válidas na lista.
 * @param contador, variável para contar o número de antenas.
 * @param lista, ponteiro para a lista de antenas.
 * @details Esta função percorre a lista de antenas e conta o número de antenas.
 * @return int 
 */
int contarantenas(antenas lista) 
{
    int contador = 0;
    while (lista != NULL) {
        if(lista->freq != '.') /// Verifica se a antena é válida
        {
            contador++;
        }
        lista = lista->seguinte;
    }
    return contador;
}

int contaradj(adjacentes lista) 
{
    int contador = 0;
    while (lista != NULL) {
        
        contador++;
        lista = lista->seguinte;
    }
    return contador;
}

/**
 * @brief Função para libertar a memória alocada para a lista de antenas.
 * 
 * @param lista 
 */
void liberar_memoria_adjacentes(adjacentes lista) 
{
    while (lista != NULL) {
        adjacentes temp = lista;
        lista = lista->seguinte;
        free(temp);
    }
}

/**
 * @brief Função para libertar a memória alocada para a lista de antenas.
 * 
 * @param lista 
 */
void libertar_memoria_antenas(antenas lista) 
{
    while (lista != NULL) {
        liberar_memoria_adjacentes(lista->listaadjacentes); /// Liberta a memória da lista de adjacentes
        antenas temp = lista;
        lista = lista->seguinte;
        free(temp);
    }
}

/**
 * @brief Função para definir a cor do texto no terminal.
 * @param cor Cor a ser definida.
 */
void corletra(int cor)
{
    HANDLE hConsole;                                //ponteiro para um recurso do sistema operativo, neste caso, a consola
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);         
    SetConsoleTextAttribute(hConsole, cor);
}

/**
 * @brief Função para contar o número de colunas no ficheiro.
 * @param cidade Ponteiro para o ficheiro.
 * @return Número de colunas.
 */
int n_colunas(char ficheiro[])
{
    FILE *cidade = fopen(ficheiro, "rb"); /// Abre o ficheiro em modo binário
    
    if (!cidade)
        return 0;

    int colunas = 0;
    char c;
    while ((c = fgetc(cidade)) != EOF && c != '\n') /// Adiciona verificação EOF
    {
        colunas++;
    }
    if (c == EOF && colunas == 0) /// Verifica se o ficheiro está vazio
        return 0;

    fclose(cidade); /// Fecha o ficheiro
    return colunas;
}

/**
 * @brief Função para contar o número de linhas no ficheiro.
 * @param cidade Ponteiro para o ficheiro.
 * @return Número de linhas.
 */
int n_linhas(char ficheiro[])
{
    FILE *cidade = fopen(ficheiro, "rb"); /// Abre o ficheiro em modo binário
    if (!cidade)
        return 0;

    int linhas = 0;
    char c;
    while ((c = fgetc(cidade)) != EOF)
    {
        if (c == '\n')
        {
            linhas++;
        }
    }
    fclose(cidade); /// Fecha o ficheiro
    return (linhas + 1); /// Adiciona 1 para contar a última linha
}

/**
 * @brief Função para ler o ficheiro e armazenar os dados na lista de antenas.
 * @param cidade Ponteiro para o ficheiro.
 * @param mapa Ponteiro para a lista de antenas.
 * @return Ponteiro para a lista de antenas.
 */
antenas ler_ficheiro(char ficheiro[], antenas mapa)

{
    FILE *cidade = fopen(ficheiro, "rb");
    if (!cidade)
    {
        corletra(RED);
        printf("Erro ao abrir o ficheiro.\n");
        corletra(WHITE);
        return NULL;
    }
    int contador = 1;
    int linha = n_linhas(ficheiro);
    int coluna = n_colunas(ficheiro);
    char matriz[linha][coluna];

    for (int i = 0; i < linha; i++)
    {
        for (int j = 0; j < coluna; j++)
        {
            fscanf(cidade, " %c", &matriz[i][j]);
        }
    }
    fclose(cidade);

    for (int i = 0; i < linha; i++)
    {
        for (int j = 0; j < coluna; j++)
        {
            antenas novo = (antenas)malloc(sizeof(struct antenas));
            if (!novo)
            {
                corletra(RED);
                printf("Erro ao alocar memória.\n");
                corletra(WHITE);
                return mapa;
            }

            novo->freq = matriz[i][j];
            novo->coordenadas = ((i + 1) * 1000) + (j + 1);
            novo->seguinte = NULL;
            novo->listaadjacentes = NULL; /// Inicializa a lista de adjacentes como vazia
            if (novo->freq != '.')
            {
                novo->verticeantena = contador; // Atribui o número do registo
                contador++;
            }

            else
            {
                free(novo); // Libera a memória se a frequência for '.'
                continue; // Ignora antenas com frequência '.'
            }
            
            if (mapa == NULL)
            {
                mapa = novo;
            }
            else
            {
                antenas aux = mapa;
                while (aux->seguinte != NULL)
                {
                    aux = aux->seguinte;
                }
                aux->seguinte = novo;
            }   
        }
    }

    corletra(GREEN);
    printf("Dados lidos com sucesso!\n");
    corletra(WHITE);
    return mapa;
}

/**
* @brief Função para imprimir o mapa das antenas.
* @param ficheiro Nome do ficheiro.
*/
void impressao_mapa_das_antenas(antenas mapa)
{
    int tamanho;
    int linha, coluna;
    antenas auxiliar = mapa;
    if (!auxiliar)
    {
        corletra(RED);
        printf("Não há dados. Leia o mapa primeiro!\n");
        corletra(WHITE);
        return;
    }

    while (auxiliar!= NULL)
    {
        tamanho= auxiliar->coordenadas;
        auxiliar = auxiliar->seguinte;
    }
    auxiliar = mapa;
    linha = tamanho / 1000;
    coluna = tamanho % 1000;
    
    corletra(GREEN);
    printf("\n***************************\n");
    corletra(WHITE);
    while (auxiliar != NULL)
    {
        if (auxiliar->freq != '.')
        {
            printf("Antena: %c , n %d com coordenada (%d, %d)\n", auxiliar->freq, auxiliar->verticeantena, auxiliar->coordenadas / 1000, auxiliar->coordenadas % 1000);
        }
        auxiliar = auxiliar->seguinte;
    }
    auxiliar = mapa;
    corletra(GREEN);
    printf("\n***************************\n");
    corletra(WHITE);

}

/**
 * @brief Função recursiva para procurar em profundidade.
 * 
 * @param mapa 
 * @param vertice 
 * @param visitados 
 * @param caminho 
 * @param nivel 
 */
void recursiva_profundidade(antenas mapa, int vertice, int visitados[], int caminho[], int nivel) {
    antenas aux = mapa;
    while (aux != NULL) {
        if (aux->verticeantena == vertice) {
            if (visitados[vertice] == 0) {
                visitados[vertice] = 1;
                caminho[nivel] = vertice;

                printf("--> Freq: %c n %d (%d, %d) ", aux->freq, aux->verticeantena, aux->coordenadas / 1000, aux->coordenadas % 1000);
                
                // percorre os adjacentes
                adjacentes adj = aux->listaadjacentes;
                while (adj != NULL) {
                    if (!visitados[adj->verticeadjacente]) {
                        recursiva_profundidade(mapa, adj->verticeadjacente, visitados, caminho, ++nivel);
                    }
                    adj = adj->seguinte;
                }
            }
        }
        aux = aux->seguinte;
    }
    printf("\n");
}

/**
 * @brief Função para procurar em profundidade.
 * @details Esta função percorre o grafo em profundidade a partir de um vértice de partida. 
 * @param mapa 
 * @param partida 
 */
void procuraProfundidade(antenas mapa, int partida) {
    int total = contarantenas(mapa);
    int visitados[total + 1];
    int caminho[total + 1];
    
    for (int i = 0; i <= total; i++) {
        visitados[i] = 0;
        caminho[i] = -1; /// serve para inicializar o array caminho[] com um valor “inválido”, que sinaliza que essa posição ainda não foi ocupada.
    }

    printf("\n--- Início da procura em profundidade a partir da antena %d ---\n", partida);
    antenas aux = mapa;
    if (aux->listaadjacentes == NULL){
        corletra(RED);
        printf("Antena sem adjacentes.\n");
        corletra(WHITE);
        return;
    }
    printf("Passou pela antena: \n");
    recursiva_profundidade(mapa, partida, visitados, caminho, 0);
}

/**
 * @brief Função para procurar em profundidade. através dos grafos
 * @param mapa 
 * @param vertice 
 */
void procuraLargura(antenas mapa, int vertice) {
    antenas aux = mapa;
    int qt;
    int opc = contarantenas(mapa);
    while (aux != NULL) {
        if (aux->verticeantena == vertice) {
            adjacentes adj = aux->listaadjacentes;
            qt = contaradj(adj);
            int caminho[qt];
            int visitados[qt];
            for (int i = 0; i <= qt; i++) {
                visitados[i] = 0;
                caminho[i] = -1; /// serve para inicializar o array caminho[] com um valor “inválido”, que sinaliza que essa posição ainda não foi ocupada.
            }
            if (adj == NULL) {
                corletra(RED);
                printf("Antena n %d não tem adjacentes.\n", aux->verticeantena);
                corletra(CYAN);
                printf("Vai ser redirecionado para o menu principal...\n");
                Sleep(2000);
                return;
            }
            printf("Do vertice %d com frequencia: %c é possível chegar a: [ ", aux->verticeantena, aux->freq);
            while (adj != NULL) {
                printf("(%d) ", adj->verticeadjacente);
                adj = adj->seguinte;
            }
            printf("]\nInsira o proximo vertice (0 para sair): ");
            int proximo;
            scanf(" %d", &proximo);
            if (proximo == 0) {
                corletra(RED);
                printf("A sair...\n");
                Sleep(2000);
                corletra(WHITE);
                return;
            }
            else if (proximo > 0 && proximo <= opc) procuraLargura(mapa, proximo);
            else {
                corletra(RED);
                printf("Antena não existe.\n");
                corletra(WHITE);
                continue;
            }
        }
        aux = aux->seguinte;
    }
}

// LL PARA GUARDAR OS CAMINHOS
//COMEÇO NUM VERTICE, GUARDO ESSE VERTICE NA LISTA CAMINHO, DEPOIS VOU AOS ADJACENTES E GUARDO O PROXIMO NO CAMINHO, 
//DEPOIS VOU AOS ADJACENTES DO PROXIMO E GUARDO O PROXIMO NO CAMINHO, ASSIM SUCESSIVAMENTE. 

// void caminho_a_para_b(antenas mapa, int inicio, int fim){
//     antenas aux = mapa;
//     adjacentes adj = NULL;
//     int visitados[contarantenas(mapa)];
//     while (aux != NULL) {
//         if (aux->verticeantena == inicio) {
//             adj = aux->listaadjacentes;
//         }
//         aux = aux->seguinte;
//     }
//     int n_antenas = contaradj(adj);
//     caminho circuito = (caminho)malloc(sizeof(struct adjacentes));
//     circuito->verticeadjacente = inicio;
//     circuito->seguinte = NULL;
// }

/**
 * @brief Função para determinar se dois pares de antenas com frequências de ressonância distintas A e B se intersetam 
 * @details A função percorre a lista de antenas e verifica se as antenas A e B têm frequências diferentes e se não são a mesma antena.
 * Depois, calcula o declive e o ponto de interseção entre as duas antenas.
 * Se o ponto de interseção estiver dentro dos limites das antenas A e B, imprime a coordenada de interseção.
 * Caso sejam paralelas é imprimo uma mensagem com essa informação.
 * @bug Problemas com verificações de dados e iterações. Ajustes na lógica necessários.
 * @param mapa 
 */
void intersecao(antenas mapa, int tamanho) {
    
    int n_antenas = contarantenas(mapa);
    for (antenas a = mapa; a != NULL; a = a->seguinte) {
        for (antenas b = mapa; b != NULL && b != a; b = b->seguinte) {
            if (a->freq != b->freq || a->verticeantena == b->verticeantena) continue;           /// Ignorar se são frequências diferentes ou se for a mesma antena
            for (antenas c = a->seguinte; c != NULL && c != b; c = c->seguinte) {
                for (antenas d = b->seguinte; d != NULL && d != a && d != c; d = d->seguinte) {
                    if (c->freq != d->freq || c->verticeantena == d->verticeantena) continue;
                    if (a->freq != c->freq &&
                        a->verticeantena != c->verticeantena && a->verticeantena != d->verticeantena &&
                        b->verticeantena != c->verticeantena && b->verticeantena != d->verticeantena) {             /// Garantir que são antenas distintas
                        float declivefuncao1 = (float)(b->coordenadas%1000 - a->coordenadas%1000) / (b->coordenadas/1000 - a->coordenadas/1000); ///declive em numerico (4/2 = 2)
                        float bfuncao1 = (float)(a->coordenadas % 1000) - declivefuncao1 * (a->coordenadas / 1000);  ///calculo do b da função (b = y - mx) => b = 2 - 3*3 = -7
                        //bfuncao1 = 2x + -7
                        float declivefuncao2 = (float)(d->coordenadas%1000 - c->coordenadas%1000) / (d->coordenadas/1000 - c->coordenadas/1000); // declive em numerico (8/2 = 4)
                        float bfuncao2 = (float)(c->coordenadas%1000) - declivefuncao2*(c->coordenadas%1000);  //calculo do b da função (b = y - mx) => b = 1 - 4*6 = -23
                        //Atenção, que uma linha está guardada como x, num plano cartesiano seria y e vice versa
                        //basicamente já tenho:
                        //y1=m1x + b1, segundo o exemplo: y1= 2x + -7
                        //y2=m2x + b2, segundo o exemplo: y2= 4x + -23
                        // agora é igualar as duas funções, que dará x=(b2-b1)/(m1-m2) => 
                        //x= (-23-(-7))/(2-4) => x= -16/-2 => x=8
                        
                        // para descobrir o ponto de interseção, igualamos as duas funções:
                        float xintercessao = (bfuncao2 - bfuncao1) / (declivefuncao1 - declivefuncao2); ///calculo do x da coordenada de interseção usando a fórmula x=(b2-b1)/(m1-m2)
                        float yintercessao = declivefuncao1 * xintercessao + bfuncao1; ///calculo do y da coordenada de interseção usando a fórmula y=mx+b
                        int coordenadasintercessao = (xintercessao * 1000) + yintercessao; ///calculo da coordenada de interseção
                        if (coordenadasintercessao == 0) continue;
                        if (coordenadasintercessao > 0 && coordenadasintercessao < tamanho) {
                                corletra(GREEN);
                                printf("As antenas %c(%d,%d)  %c(%d,%d) e %c(%d,%d) %c(%d,%d) interceptam-se na coordenada (%.2f, %.2f)\n", a->freq, a->coordenadas/1000, a->coordenadas%1000, b->freq, b->coordenadas/1000, b->coordenadas%1000, 
                                    c->freq, c->coordenadas/1000, c->coordenadas%1000, d->freq, d->coordenadas/1000, d->coordenadas%1000, xintercessao, yintercessao);
                                corletra(WHITE);
                            }
                    }
                }
            }
        }
    }
    return;
}    

/**
 * @brief Função para adicionar adjacentes.
 * @param mapa Ponteiro para a lista de antenas.
 * @param novo Ponteiro para a lista de adjacentes.
 * 
 */
antenas adicionarAdjacentes(antenas mapa) {
    for (antenas a = mapa; a != NULL; a = a->seguinte) {
        for (antenas b = mapa; b != NULL; b = b->seguinte) {
            if (a->freq == b->freq && a->verticeantena != b->verticeantena) {
                adjacentes novo = (adjacentes)malloc(sizeof(struct adjacentes));
                if (!novo) {
                    corletra(RED);
                    printf("Erro ao alocar memória para adjacente.\n");
                    corletra(WHITE);
                    continue;
                }
                novo->verticeadjacente = b->verticeantena;
                novo->seguinte = NULL;

                // Inserir no fim da lista de adjacentes de 'a'
                if (a->listaadjacentes == NULL) {
                    a->listaadjacentes = novo;
                } else {
                    adjacentes temp = a->listaadjacentes;
                    while (temp->seguinte != NULL) {
                        temp = temp->seguinte;
                    }
                    temp->seguinte = novo;
                }
            }
        }
    }

    corletra(GREEN);
    printf("Adjacentes adicionados com sucesso!\n");
    corletra(WHITE);
    return mapa;
}

/**
 * @brief Função para imprimir os adjacentes.
 * @param mapa Ponteiro para a lista de antenas.
 * @param lista Ponteiro para a lista de adjacentes.
 */
void imprimirAdjacentes(antenas mapa) {
    antenas auxiliar = mapa;
    
    while (auxiliar != NULL){
        printf("Antena: %c, n %d com coordenada (%d, %d) tem como adjacentes: [", auxiliar->freq, auxiliar->verticeantena, auxiliar->coordenadas / 1000, auxiliar->coordenadas % 1000);
        adjacentes listadjacente = auxiliar->listaadjacentes;
        while (listadjacente != NULL) {
            printf("%d ", listadjacente->verticeadjacente);
            listadjacente = listadjacente->seguinte;
        }
        printf("]\n");
        auxiliar = auxiliar->seguinte;
    } 
    
}

/**
 * @brief Função principal do sistema.
 * @return 0 se o programa terminar com sucesso.
 */
int sistema() 
{
    corletra(MAGENTA);
    printf("\t\t\t***Sistema de Antenas***\n\t\t\t\tBEM-VINDO!\n");
    corletra(WHITE);
    char nome_ficheiro1[TAM];
    do
    {
    
        antenas mapaantenas = NULL; /// Inicializa a lista de antenas como vazia
        adjacentes listaadjacentes = NULL; /// Inicializa a lista de adjacentes como vazia
        adjacentes grafo = NULL; /// Inicializa o grafo como vazio
        printf("Insira o nome do ficheiro com a extensão ou \"sair\" para encerrar o programa: ");
        scanf(" %s", nome_ficheiro1);
        if (strcmp(nome_ficheiro1, "sair") == 0) {
            corletra(RED);
            printf("Volte sempre :D.\n");
            corletra(WHITE);
            Sleep(2000);
            return 0;
        } 
        else if (n_colunas(nome_ficheiro1) == 0 && n_linhas(nome_ficheiro1) == 0)
        {
            corletra(RED);
            printf("Erro: ficheiro vazio, inexistente ou formato inválido.\n");
            corletra(WHITE);
            continue;
        }
        else
        {

            mapaantenas = ler_ficheiro(nome_ficheiro1, mapaantenas); /// Lê o ficheiro e armazena os dados na lista de antenas
            mapaantenas = adicionarAdjacentes(mapaantenas); /// Adiciona os adjacentes à lista de antenas
            int tamanho = n_linhas(nome_ficheiro1) * 1000 + n_colunas(nome_ficheiro1); 
            int opc;
            do
            {
                corletra(RED);
                printf("\t\t***Menu principal***\n");
                corletra(MAGENTA);
                printf("1--> Ver mapa de antenas.\n");
                printf("2--> Procura em profundidade.\n");
                printf("3--> Procura em largura.\n");
                printf("4--> Traçar um caminho.\n");
                printf("5--> Ver adjacentes.\n");
                printf("6--> Ver pares que se intersetam.\n");
                printf("0--> Escolher outro ficheiro!\n");
                corletra(WHITE);
                printf("Escolha uma opção: ");
                scanf(" %d", &opc);
                switch (opc){
                    case 1:
                        impressao_mapa_das_antenas(mapaantenas);
                        break;
                    case 2:
                        int partida;
                        printf("Insira o número da antena de partida: ");
                        scanf(" %d", &partida);
                        if (partida < contarantenas(mapaantenas) && partida >= 1) {
                            procuraProfundidade(mapaantenas, partida);
                        }
                        else {
                            corletra(RED);
                            printf("Antena não existe.\n");
                            corletra(WHITE);
                        }
                        break;
                    case 3:
                        int parti;
                        printf("Insira o número da antena de partida: ");
                        scanf(" %d", &parti);
                        if (parti < contarantenas(mapaantenas) && parti >= 1) 
                        procuraLargura(mapaantenas, parti);
                        else {
                            corletra(RED);
                            printf("Antena não existe.\n");
                            corletra(WHITE);
                        }
                        break;
                    case 4:
                        // int a, b;
                        // printf("Insira o número da antena de partida: ");
                        // scanf("%d", &a);
                        // printf("Insira o número da antena de chegada: ");
                        // scanf("%d", &b);
                        // caminho_apb(mapaantenas, &grafo, &a, a, b, NULL, 0);
                        // break;
                        printf("Função ainda não implementada.\n");
                    case 5: 
                        imprimirAdjacentes(mapaantenas);
                        break;
                    case 6:
                        intersecao(mapaantenas, tamanho);
                        break; 
                    case 0:
                        corletra(BLUE);
                        liberar_memoria_adjacentes(listaadjacentes); /// Liberta a memória da lista de adjacentes
                        libertar_memoria_antenas(mapaantenas); /// Liberta a memória da lista de antenas
                        nome_ficheiro1[0] = '\0'; /// Limpa o nome do ficheiro
                        printf("Aguarde...\n");
                        printf("A ser redirecionado para o início...\n");
                        corletra(WHITE);
                        Sleep(1000);
                        break;
                    default:
                        corletra(RED);
                        printf("Opção inválida. Tente novamente.\n");
                        corletra(WHITE);
                        break;
                }
            } while (opc != 0);  /// Enquanto o utilizador não escolher sair do menu
        }
    } while (strcmp(nome_ficheiro1, "sair")!=0); /// Enquanto o utilizador não escolher sair do programa

    return 0;

}
