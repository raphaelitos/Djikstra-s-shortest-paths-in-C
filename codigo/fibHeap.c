/**
 * @file    fibHeap.c
 * @brief   Implementa as funcoes definindas no respectivo header, referentes
 * a uma heap fibonacci
 * @author  Conrado Antoniazi dos Santos (conrado.santos@edu.ufes.br)
 * @author  Raphael Correia Dornelas (raphael.dornelas@edu.ufes.br)
 * @author  Victor Augusto Fernandes Rangel (victor.a.rangel@edu.ufes.br)
 * 
 * A implementacao da estrutura foi inspirada no codigo em python disponivel em:
 * https://github.com/msambol/dsa/blob/master/data_structures/fibonacci_heap.py
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fibHeap.h"
#include <math.h>

/// @brief Representa uma Heap Fibonacci.
struct fibHeap
{
    // numero de nos na heap
    int qtdNos;

    // no que representa o vertice
    // com menor distancia a origem
    tNodeFH *min;

    // lista circular que guarda os nos que 
    // enraizam os outros, cada um desses nos
    // e' uma arvore ordenada com min-heap 
    tNodeFH *raiz;
};

static bool isNodeAlone(tNodeFH *node)
{
    if (!node)
    {
        printf("dados invalidos em isNodeAlone!\n");
        exit(EXIT_FAILURE);
    }
    return (node == ndFHgetRight(node));
}

static bool isNodeRoot(tFH *fh, tNodeFH *node)
{
    if (!fh || !node)
    {
        printf("dados invalidos em isNodeRoot!\n");
        exit(EXIT_FAILURE);
    }
    return (fh->raiz == node);
}

static void removeFromRoot(tFH *fh, tNodeFH *node)
{
    if (!fh || !node)
    {
        printf("dados invalidos em removeFromRoot!\n");
        exit(EXIT_FAILURE);
    }
    if (isNodeRoot(fh, node))
    {

        (isNodeAlone(node)) ? (fh->raiz = NULL) : (fh->raiz = ndFHgetRight(fh->raiz));
    }

    ndFHremove(node);
}

tFH *fhInit()
{
    tFH *fh = calloc(1, sizeof(tFH));
    if (!fh)
    {
        printf("Falha na alocacao de FH!\n");
        exit(EXIT_FAILURE);
    }
    fh->min = NULL;
    fh->raiz = NULL;

    return fh;
}

void fhDestroy(tFH *fh)
{
    if (!fh)
        return;

    ndFHdestroy(fh->raiz);
    free(fh);
}

tNodeFH *fhInsert(tFH *fh, tVertice *vert)
{
    if (!fh || !vert)
    {
        printf("tentativa de fhInsert com dados invalidos\n");
        exit(EXIT_FAILURE);
    }
    tNodeFH *newNode = ndFHInit(vert);

    // inserindo o novo no na raiz
    if (!(fh->raiz))
    {
        fh->raiz = newNode;
    }
    else
    {
        ndFHinsert(fh->raiz, newNode);
    }

    if (!(fh->min) || (getAccVert(vert) < ndFHgetKey(fh->min)))
    {
        fh->min = newNode;
        // printf("opa\n");
    } // setando novo minimo se necessario

    (fh->qtdNos)++;
    return newNode;
}

tNodeFH *fhMinimum(tFH *fh)
{
    if (!fh)
    {
        printf("dados invalidos em fhMinimum!\n");
        exit(EXIT_FAILURE);
    }
    return fh->min;
}

static void fhLink(tFH *fh, tNodeFH *y, tNodeFH *x)
{
    if (!fh || !y || !x)
    {
        printf("dados invalidos em fhLink");
        exit(EXIT_FAILURE);
    }

    removeFromRoot(fh, y);
    ndFHsetLeft(y, y);
    ndFHsetRight(y, y);
    // y devidamente isolado

    ndFHinsertFilho(x, y);
    ndFHsetMarcado(y, false);
    // y e' filho de x agora
}

static void fhConsolidate(tFH *fh)
{
    if (!fh)
    {
        printf("fh nula em Consolidate!\n");
        exit(EXIT_FAILURE);
    }
    if (!(fh->raiz))
        return;

    /*
    construcao de um vetor auxiliar auxVet onde o indice representa
    o numero de arestas de um vertice. Numa fib Heap, o grau maximo
    eh dado pelo log da quantidade de nos multiplicado por dois
    */
    int grauMaximo = (int)(log(fh->qtdNos) * 2) + 1;
    tNodeFH **auxVet = (tNodeFH **)malloc(grauMaximo * sizeof(tNodeFH *));
    if (!auxVet)
    {
        printf("falha na criacao de auxVet em fhConsolidate!\n");
        exit(EXIT_FAILURE);
    }
    for (int k = 0; k < grauMaximo; k++)
    {
        auxVet[k] = NULL;
    }
    // inicializacao finalizada

    /*dump da lista raiz para um vetor auxiliar nodeVet*/
    int tam = 0;
    tNodeFH *aux = fh->raiz;
    do
    {
        aux = ndFHgetRight(aux);
        tam++;
    } while (aux != fh->raiz);

    tNodeFH **nodeVet = malloc(tam * sizeof(tNodeFH *));
    if (!nodeVet)
    {
        printf("falha na alocacao de nodeVet em fhConsolidate!\n");
        free(auxVet);
        exit(EXIT_FAILURE);
    }

    int i = 0;
    aux = fh->raiz;
    do
    {
        nodeVet[i] = aux;
        i++;
        aux = ndFHgetRight(aux);
    } while (aux != fh->raiz);
    // dump encerrado

    tNodeFH *x = NULL;
    int grau = 0;

    // consolidacao para cada no da raiz
    // basicamente, todo no que tiver o mesmo grau
    // vai terminar junto (fhLink)
    for (i = 0; i < tam; i++)
    {
        x = nodeVet[i];
        grau = ndFHgetGrau(x);

        //enquanto nao achar uma casa nula, 
        //o link acontece
        while (auxVet[grau] != NULL)
        {
            tNodeFH *y = auxVet[grau];
            if (ndFHgetKey(x) > ndFHgetKey(y))
            {
                tNodeFH *temp = x;
                x = y;
                y = temp;
            }//troca x por y, pois x sera o pai
            fhLink(fh, y, x);
            auxVet[grau] = NULL;
            grau++;
        }
    
        auxVet[grau] = x;
    } // consolidacao encerrada

    // Reconstrucao da lista raiz
    fh->raiz = NULL;
    fh->min = NULL;
    for (i = 0; i < grauMaximo; i++)
    {
        if (auxVet[i] != NULL)
        {
            ndFHsetLeft(auxVet[i], auxVet[i]);
            ndFHsetRight(auxVet[i], auxVet[i]);
            //isolando nos que estarao na raiz
            if (fh->raiz == NULL)
            {
                fh->raiz = auxVet[i];
                fh->min = auxVet[i];
            }
            else
            {
                ndFHinsert(fh->raiz, auxVet[i]);
                if (ndFHgetKey(auxVet[i]) < ndFHgetKey(fh->min))
                    fh->min = auxVet[i];
            }
        }
    } // reconstrucao encerrada

    free(auxVet);
    free(nodeVet);
}

tVertice *fhExtractMin(tFH *fh)
{
    if(!fh){
        printf("fh nula em extractMin!\n");
        exit(EXIT_FAILURE);
    }
    
    tNodeFH *v = fh->min;

    if(!v) return NULL;

    
    tNodeFH *filho = ndFHgetFilho(v);
    if (filho)
    {
        /*dump da lista de filhos para um vetor auxiliar nodeVet*/
        int tam = 0;
        tNodeFH *aux = filho;
        do  //obtendo o tamanho do vetor
        {
            aux = ndFHgetRight(aux);
            tam++;
        } while (aux != filho);

        tNodeFH **nodeVet = malloc(tam * sizeof(tNodeFH *));
        if (!nodeVet)
        {
            printf("falha na alocacao de nodeVet em fhExtractMin!\n");
            exit(EXIT_FAILURE);
        }

        int i = 0;
        do
        {
            nodeVet[i] = aux;
            i++;
            aux = ndFHgetRight(aux);
        } while (aux != filho);
        // dump encerrado

        for (i = 0; i < tam; i++)
        {

            ndFHremove(nodeVet[i]);

            if (!(fh->raiz))
            {
                fh->raiz = nodeVet[i];
            }
            else
            {
                ndFHinsert(fh->raiz, nodeVet[i]);
            }
            ndFHsetPai(nodeVet[i], NULL);
        } // filhos de v adotados pela raiz
        free(nodeVet);
    }

    removeFromRoot(fh, v);

    if (!(fh->raiz))
    { // acontece se v nao for pai de ngm
        fh->min = NULL;
        fh->raiz = NULL;
    }
    else
    {
        fh->min = fh->raiz;
        fhConsolidate(fh);
    }
    (fh->qtdNos)--;

    tVertice *aux = ndFHgetVert(v);

    setNodeFHVert(aux, NULL);
    free(v); // liberacao necessaria pois nao vamos usar mais essa estrutura

    return aux;
    
}

static tNodeFH *getUltimoFH(tFH *fh)
{
    if (!fh)
    {
        printf("dados invalidos em getUltimoFH!\n");
        exit(EXIT_FAILURE);
    }
    return ndFHgetLeft(fh->raiz);
}

tFH *fhUnion(tFH *fh, tFH *outra)
{
    if (!fh || !outra)
    {
        printf("Dados invalidos em fhUnion!\n");
        exit(EXIT_FAILURE);
    }

    if (!fh->raiz)
        return outra;
    if (!outra->raiz)
        return fh;

    tFH *nova = fhInit();
    nova->raiz = fh->raiz;

    // min de nova sera o menor entre min de fh e outra
    (ndFHgetKey(fh->min) < ndFHgetKey(outra->min)) ? (nova->min = fh->min) : (nova->min = outra->min);

    tNodeFH *ultimo = getUltimoFH(outra);

    ndFHsetLeft(outra->raiz, ndFHgetLeft(nova->raiz));
    ndFHsetRight(ndFHgetLeft(nova->raiz), outra->raiz);

    ndFHsetLeft(nova->raiz, ultimo);
    ndFHsetRight(ndFHgetLeft(nova->raiz), nova->raiz);

    nova->qtdNos = fh->qtdNos + outra->qtdNos;

    // zerando heaps originais
    fh->raiz = fh->min = NULL;
    fh->qtdNos = 0;
    outra->raiz = outra->min = NULL;
    outra->qtdNos = 0;

    return nova;
}

/// @brief retira a da lista de filhos de b
/// e joga a pra raiz
/// @param fh uma fib Heap valida
/// @param a um no da heap valido, que
/// deve ser filho de b
/// @param b um no da heap valido, que
/// perdera o filho a
static void corte(tFH *fh, tNodeFH *a, tNodeFH *b)
{
    if (!fh || !a || !b)
    {
        printf("dados invalidos em corte!\n");
        exit(EXIT_FAILURE);
    }
    ndFHremoveFilho(b, a);
    /*A funcao acima esta
    decrementadno grau de b
    setando pai de a como null
    definindo "marcado" de a como false
    */
    ndFHinsert(fh->raiz, a);
}

static void corteRec(tFH *fh, tNodeFH *y)
{
    if (!fh || !y)
    {
        printf("dados inválidos em corteRec!\n");
        exit(EXIT_FAILURE);
    }

    tNodeFH *z = ndFHgetPai(y);
    if (z != NULL)
    {
        // Se 'y' não estava marcado, marca-o e para por aqui.
        if (!ndFHgetMarcado(y))
        {
            ndFHsetMarcado(y, true);
        }
        // Caso já estivesse marcado, corta 'y' de seu pai e propaga o corte.
        else
        {
            corte(fh, y, z);
            corteRec(fh, z);
        }
    }
}

void fhDecreaseKey(tFH *fh, tNodeFH *node, double newKey)
{
    if (!fh || !node)
    {
        printf("dados invalidos em fhDecreaseKey!\n");
        exit(EXIT_FAILURE);
    }
    if (newKey > ndFHgetKey(node))
        return;

    ndFHsetKey(node, newKey);

    tNodeFH *pai = ndFHgetPai(node);

    if (pai && (ndFHgetKey(node) < ndFHgetKey(pai)))
    {
        corte(fh, node, pai);
        corteRec(fh, pai);
    } // subindo o filho

    if (ndFHgetKey(node) < ndFHgetKey(fh->min))
    {
        fh->min = node;
    } // setando novo min se necessario
}

bool fhIsEmpty(tFH *fh)
{
    if (!fh)
    {
        printf("dado invalido em fhIsEmpty!\n");
        exit(EXIT_FAILURE);
    }
    return (fh->qtdNos == 0);
}