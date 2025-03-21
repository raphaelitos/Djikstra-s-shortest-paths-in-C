/**
 * @file    tGrafo.c
 * @brief   Implementa funcoes referentes a um grafo
 * dirigido ponderado
 * @author  Conrado Antoniazi dos Santos (conrado.santos@edu.ufes.br)
 * @author  Raphael Correia Dornelas (raphael.dornelas@edu.ufes.br)
 * @author  Victor Augusto Fernandes Rangel (victor.a.rangel@edu.ufes.br)
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#include "tGrafo.h"
#include "tAresta.h"


/// @brief Representa um grafo dirigido
/// ponderado
struct grafo
{
    // vetor de vertices do grafo
    tVertice **vertices;
    // origem do menor caminho aos
    // outros vertices do grafo
    tVertice* origem;
    // quantidade de vertices do grafo
    int numVertices;
};

#define MAX_TAM_LINHA 1024

tGrafo* GrafoInit(char* path) {
    FILE *file = fopen(path, "r");
    int origem;

    if (!file) {
        perror("Nao foi possivel localizar arquivo de entrada\n");
        exit(EXIT_FAILURE);
    }

    tGrafo *grafo = malloc(sizeof(tGrafo));
    if(!grafo) {
        perror("Nao foi possivel alocar dinamicamente o grafo.\n");
        exit(EXIT_FAILURE);
    }

    fscanf(file, "node_%d\n", &origem);

    long checkpoint = ftell(file); // Salva a posicao atual no arquivo
    if (checkpoint == -1L) {
        perror("Erro ao obter a posição inicial");
        fclose(file);
        return NULL;
    }

    int numNodes = 0;
    char parada;
    while(fscanf(file, "%*[^,\n]") != EOF) { //conta o numero de vertices na primeira linha
        numNodes++;
        fscanf(file, "%c", &parada);
        if(parada == '\n') break;
    }

    grafo->numVertices = numNodes;
    grafo->vertices = malloc(numNodes * sizeof(tVertice*)); //alocando vetor principal de vertices
    for(int i = 0; i < numNodes; i++) {
        char nome[100];
        sprintf(nome, "node_%d", i);
        grafo->vertices[i] = criaVertice(nome);
    }

    if (fseek(file, checkpoint, SEEK_SET) != 0) { //volta ao checkpoint no arquivo
        perror("Erro ao voltar para o checkpoint");
        fclose(file);
        return NULL;
    }

    for(int i = 0; i < numNodes; i++) {
        fscanf(file, "%*[^,\n],");

        for(int j = 0; j < numNodes; j++) {
            if(j == i) continue; //pula para nao atribuir peso a uma aresta para o proprio vertice analisado
            double peso = 0;

            if(fscanf(file, "%lf", &peso) == 1 && peso > 0) {
                addVizinhoVert(grafo->vertices[i], criaAresta(grafo->vertices[j], peso));
                fscanf(file, "%*c");
            }
            else {
                fscanf(file, "%*[^,\n]"); //necessario para evitar conflito com as palavras "bomba"
                fscanf(file, "%*c");
            }
        }
    }

    //Set da origem
    grafo->origem = grafo->vertices[origem];

    fclose(file);
    return grafo;
}

void DesalocaGrafo(tGrafo* grafo) {
    if(grafo == NULL) return;
    if(grafo->vertices != NULL) {
        for(int i = 0; i < grafo->numVertices; i++) {
            apagaVertice(grafo->vertices[i]);
        }
        free(grafo->vertices);
    }
    free(grafo);
}

tVertice *getOrigemGrafo(tGrafo *g){
    if(!g) return NULL;
    return g->origem;
}

tVertice **getVerticesGrafo(tGrafo *g){
    if(!g){
        printf("grafo invalido em getVertices\n");
        exit(EXIT_FAILURE);
    }
    return g->vertices;
}

int getNumVerticesGrafo(tGrafo *g){
    if(!g){
        printf("grafo invalido em getNumVertices\n");
        exit(EXIT_FAILURE);
    }
    return g->numVertices;
}

// Função de comparação para o qsort
static int cmpVertice(const void *v1, const void *v2) {
    tVertice *vert1 = *(tVertice **)v1;
    tVertice *vert2 = *(tVertice **)v2;
    double accV1 = getAccVert(vert1);
    double accV2 = getAccVert(vert2);
    
    if (accV1 < accV2)
        return -1;
    else if (accV1 > accV2)
        return 1;
    else
        return 0;
}

void ImprimeCaminho(FILE* arquivo, tVertice* v, tVertice* src) {
    if(v == NULL) return;
    else if(v == src) {
        fprintf(arquivo, "%s", getNomeVert(v));
        fprintf(arquivo, " <- ");
    }

    tVertice* aux = v;
    while (aux != src) {
        fprintf(arquivo, "%s", getNomeVert(aux));
        fprintf(arquivo, " <- ");
        aux = getPaiVert(aux);
    }
    fprintf(arquivo, "%s", getNomeVert(src));
}

void ImprimeCaminhosMenorCusto(tGrafo *grafo, tVertice *source, char *path) {
    if(!grafo || !source){
        printf("dados invalidos em imprimeCaminhos\n");
        exit(EXIT_FAILURE);
    }

    qsort(grafo->vertices, grafo->numVertices, sizeof(tVertice*), cmpVertice);

    FILE *arquivo = fopen(path, "w");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo para escrita");
        exit(EXIT_FAILURE);
    }

    int numVertices = grafo->numVertices;

    for (int i = 0; i < numVertices; i++) {
        tVertice *v = grafo->vertices[i];
        double dist = getAccVert(v);  // dist

        // Confere se o vertice e' alcancavel
        if (dist == INT_MAX) {
            continue; 
        }

        fprintf(arquivo, "SHORTEST PATH TO %s: ", getNomeVert(v));

        ImprimeCaminho(arquivo, v, getOrigemGrafo(grafo));

        fprintf(arquivo, " (Distance: %.2lf)\n", dist);
    }

    fclose(arquivo);
}
