#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tGrafo.h"
#include <time.h>

int main(int argc, char *argv[])
{

    if(argc <= 2){
        printf("Forneca um caminho valido para o arquivo de entrada e outro para o de saida!\n");
        return 0;
    }
    
    clock_t start = clock();
    tGrafo *grafo = GrafoInit(argv[1]);
    
    tVertice *src = getOrigemGrafo(grafo);
    
    if(!src){
        printf("src nulo :(");
        DesalocaGrafo(grafo);
        return 0;
    }

    Dijkstra(grafo, src);

    //ImprimeCaminhosMenorCusto(grafo, src, argv[2]);

    DesalocaGrafo(grafo);
    clock_t end = clock();
    double seconds = ((double)end - start) /CLOCKS_PER_SEC;
    printf("%lf seconds",seconds);
    return 0;
}
