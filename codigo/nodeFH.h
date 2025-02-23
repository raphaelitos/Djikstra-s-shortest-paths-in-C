#ifndef _FIB_HEAP_NODE_H_
#define _FIB_HEAP_NODE_H_

#include <stdbool.h>


typedef struct fibHeapNode tNodeFH;

#include "tVertice.h"

tNodeFH *ndFHInit(tVertice *v);

void ndFHdestroy(tNodeFH* nodeFH);

void ndFHinsert(tNodeFH *lista, tNodeFH *novo);

//Essa funcao basicamente isola um no
void ndFHremove(tNodeFH *node);

void ndFHinsertFilho(tNodeFH *nodeFH, tNodeFH *filho);

tNodeFH* ndFHremoveFilho(tNodeFH *pai, tNodeFH *filho);

//getters e setters

float ndFHgetKey(tNodeFH *nodeFH);

void ndFHsetKey(tNodeFH *nodeFH, float newKey);

tVertice* ndFHgetVert(tNodeFH *nodeFH);

void ndFHsetVert(tNodeFH *nodeFH, tVertice *vert);

tNodeFH* ndFHgetLeft(tNodeFH *nodeFH);

void ndFHsetLeft(tNodeFH *nodeFH, tNodeFH *left);

tNodeFH* ndFHgetRight(tNodeFH *nodeFH);

void ndFHsetRight(tNodeFH *nodeFH, tNodeFH *right);

tNodeFH* ndFHgetPai(tNodeFH *nodeFH);

void ndFHsetPai(tNodeFH *nodeFH, tNodeFH *pai);

tNodeFH* ndFHgetFilho(tNodeFH *nodeFH);

int ndFHgetGrau(tNodeFH *node);

void ndFHsetGrau(tNodeFH *node, int grauNovo);

void ndFHincGrau(tNodeFH *node);

bool ndFHgetMarcado(tNodeFH *node);

void ndFHsetMarcado(tNodeFH *node, bool valor);

#endif