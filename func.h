#ifndef FUNC_H_INCLUDED
#define FUNC_H_INCLUDED
#include "indice.h"
#include <time.h>
#define AMAYUS(x) (('a' <= (x) && (x) <= 'z') ? ((x - 'a') + 'A') : (x))
#define dniValido(x)        (((x)>10000 && (x)<1000000000) ? 1 : 0)
#define sexoValido(x)       (((x)=='M' || (x)=='F' || (x)=='O' || (x)=='m' || (x)=='f' || (x)=='o') ? 1 : 0)
#define esBisiesto(a) (((a) % 4 == 0 && (a) % 100 != 0) || (a) % 400 == 0)
#define TAM 200
typedef struct
{
    int dia;
    int mes;
    int anio;
}tFecha;
typedef struct
{
    long DNI;///10.000<DNI<100.000.000
    char apellido[60];
    char nombre[60];
    tFecha fechaNac;
    char sexo;  ///'F' FEMENINO , 'M' MASCULINO , '0' OTRO
    tFecha fechaAfi;
    char categoria[10];///'MENOR' ,'CADETE','ADULTO','VITALICIO',HONORARIO','JUBILADO,VALIDAR
    tFecha fechaUlt;
    char estado; /// 'A' ACTIVO ,'I' INACIVO - SE GENERA EN EL ALTA CON 'A'
    tFecha fechaBaja; /// SI EL ESTADO ES 'A' ESTE ES VACIO

}tSocio;
typedef struct
{
    tFecha fech;
    int nro_reg;
}FechaInd;

void mostrarMenu();
int mostrarSociosBinario(char* nombreB);
int textoABinario(char* nombreT,char* nombreB);
void ingresarDni(long* a);
void ingresarSocio(tSocio* soc);
int textoABinario(char* nombreT,char* nombreB);
int crearArchIndice(char* nombreBSocios,char* nombreIndice);
int opciones(char op,t_indice* ind);
int cmpDni(const void* a , const void* b);
void mostrarClave(void* a , unsigned b, void* c);
int textoABinario(char* nombreT,char* nombreB);
int mostrarIndexBinario(char* path);
void mostrarDni(void* a , unsigned b, void* c);
int cmpFecha(const void* a , const void* b);
void ingresarDatosSocios(tSocio* soc);
void procesarNuevoSocio(tSocio* soc, tFecha* hoy);
int categoriaValida(char* cat);
int esFechaValida(const tFecha* fecha);
int cantDiasMes(int mes, int anio);
void buscarYMostrarSociosOrdenado(void* info , unsigned tam,void* fp);
void mostrarCampos();
void imprimirSocio(tSocio soc);
int existeCsv(char* archNombreTexto);
void obtenerFechaHoy (tFecha *fechbaja);

#endif // FUNC_H_INCLUDED
