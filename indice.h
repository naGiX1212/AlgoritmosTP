#ifndef INDICE_H_INCLUDED
#define INDICE_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arbol.h"
typedef int (*Cmp)(const void*, const void*);
typedef struct t_dato
{
    void* clave;
    unsigned nro_reg;
}t_dato;
typedef struct
{
    tArbolBinBusq arb;
    size_t tamClave;
    Cmp cmp;
}t_indice;

int cmpDni(const void* a , const void* b);///
void ind_crear (t_indice* ind, size_t tam_clave,Cmp cmp);///
int ind_insertar (t_indice* ind, void *clave, unsigned nro_reg);///
int ind_eliminar (t_indice* ind, void *clave, unsigned *nro_reg);///
int ind_buscar (const t_indice* ind, void *clave, unsigned *nro_reg);///
int ind_cargar (t_indice* ind, const char* path);///
int ind_grabar (const t_indice* ind, const char* path);///
void ind_vaciar (t_indice* ind);///
int ind_recorrer (const t_indice* ind, void (*accion)(void *, unsigned, void *), void*param);///
#endif // INDICE_H_INCLUDED
