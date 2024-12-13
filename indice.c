#include "indice.h"

void ind_crear(t_indice *ind, size_t tam_clave,Cmp cmp )
{
    crearArbolBinBusq(&ind->arb);
    ind->tamClave = tam_clave;
    ind->cmp = cmp;
}

int ind_insertar (t_indice* ind, void *clave, unsigned nro_reg)
{
    char* dato = (char*)malloc(ind->tamClave + sizeof(unsigned));
    if(!dato)
    {
        return !TODO_BIEN;
    }
    memcpy(dato,clave,ind->tamClave);
    memcpy(dato+ind->tamClave,&nro_reg,sizeof(unsigned));
    insertarArbolBinBusq(&ind->arb,dato,ind->tamClave + sizeof(unsigned),ind->cmp);
    free(dato);
    return TODO_BIEN;
}
int ind_recorrer (const t_indice* ind, void (*accion)(void *, unsigned, void *), void*param)
{
    if(!ind->arb)
    {
        return !TODO_BIEN;
    }
    recorrerEnOrdenSimpleArbolBinBusq(&ind->arb,param,accion);
    return TODO_BIEN;
}

void ind_vaciar (t_indice* ind)
{
    eliminarArbol(&ind->arb);
    ind->cmp = NULL;
    ind->tamClave = 0;
}
int ind_buscar (const t_indice* ind, void *clave, unsigned *nro_reg)///
{
    tNodoArbol** r;
    r = buscarNodoArbolBinBusq(&ind->arb,clave,ind->cmp);
    if(!r)
    {
        return !TODO_BIEN;
    }
    memcpy(nro_reg,((char*)(*r)->info)+ind->tamClave,sizeof(unsigned));
    return TODO_BIEN;
}

int ind_eliminar (t_indice* ind, void *clave, unsigned *nro_reg)
{
    char* dato = malloc(sizeof(unsigned) + ind->tamClave);
    if(!dato)
    {
        return !TODO_BIEN;
    }
    if(!eliminarElemArbolBinBusq(&ind->arb,clave,sizeof(unsigned)+ind->tamClave,ind->cmp))
    {
        return !TODO_BIEN;
    }
    free(dato);
    return TODO_BIEN;
}
void escribirEnArchBin(void* data,unsigned data_tam,unsigned a,void* arch)
{
     fwrite(data,data_tam,1,(FILE*)arch);
}
int ind_grabar (const t_indice* ind, const char* path)
{
    FILE* arch = fopen(path,"wb");
    if(!arch)
        return ERROR_ARCH;
    recorrerEnOrdenArbolBinBusq(&ind->arb,arch,escribirEnArchBin);
    fclose(arch);
    return TODO_BIEN;
}

int ind_cargar(t_indice* ind,const char* path)
{
    return cargarArchivoBinOrdenadoArbolBinBusq(&ind->arb,path,sizeof(unsigned)+ind->tamClave);
}









