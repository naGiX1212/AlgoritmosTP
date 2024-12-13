#include "func.h"
void mostrarMenu()
{

    puts("=========================SEGUNDA APLICACION===============================");
    puts("(A) Dar de alta un nuevo socio solicitando todos los campos de la estructura socios");
    puts("(M) Modificar la informacion de un socio,");
    puts("(B) Dar de baja un socio por DNI");
    puts("(L) Listar todos los socios ordenados (sin mostrar los dados de baja) por DNI");
    puts("(P) Listar los 10 socios con mayor retraso en la fecha de pago de la cuota.");
    puts("(S) Para salir");
}
void ingresarDni(long* a)
{
   printf("\nIngrese DNI: ");
   scanf("%ld",a);
}
void ingresarSocio(tSocio* soc)
{
   ingresarDni(&soc->DNI);
}
int cmpDni(const void* a , const void* b)
{
    long a1;
    long b1;
    memcpy(&a1,(char*)a,sizeof(long));
    memcpy(&b1,(char*)b,sizeof(long));
    return a1-b1;
}
void mostrarClave(void* t , unsigned tam, void* c)
{
    long clave;
    unsigned nro_reg;
    memcpy(&clave,(char*)t,sizeof(long));
    memcpy(&nro_reg,((char*)t) + sizeof(long),sizeof(unsigned) );
    printf("%ld--%d\n",clave,nro_reg);
}
int mostrarSociosBinario(char* nombreB)
{
    tSocio soc;
    FILE* b = fopen(nombreB,"rb");
    if(!b)
    {
        fclose(b);
        return ERROR_ARCH;
    }
    mostrarCampos();
    fread(&soc,sizeof(tSocio),1,b);
    while(!feof(b))
    {
        if(soc.estado!='B')
            imprimirSocio(soc);
        fread(&soc,sizeof(tSocio),1,b);
    }
    fclose(b);
    return TODO_BIEN;
}
int mostrarIndexBinario(char* path)
{
    char* dato = malloc(sizeof(long) + sizeof(unsigned));
    if(!dato)
        return !TODO_BIEN;
    long clave;
    unsigned nro_reg;
    FILE* b = fopen(path,"rb");
    if(!b)
    {
        return ERROR_ARCH;
    }
    fread(dato,sizeof(unsigned) + sizeof(long),1,b);
    while(!feof(b))
    {
        memcpy(&clave,dato,sizeof(long));
        memcpy(&nro_reg,dato+sizeof(long),sizeof(unsigned));
        printf("%ld----%d\n",clave,nro_reg);
        fread(dato,sizeof(unsigned) + sizeof(long),1,b);
    }
    fclose(b);
    return TODO_BIEN;
}
int textoABinario(char* nombreT,char* nombreB)
{
    FILE* b;
    FILE* t;
    tSocio soc;
    t = fopen(nombreT,"rt");
    if(!t)
    {
        printf("NO SE ENCONTRO EL ARCHIVO CSV\n");
        return ERROR_ARCH;
    }
    b = fopen(nombreB,"wb");
    if(!b)
    {
        fclose(t);
        return ERROR_ARCH;
    }
    /*
                46582850,Perez,Angel,1/7/1990,M,5/7/2020,VITALICIO,5/7/2022,A,5/7/2027
                11581851,Martinez,Leandro,5/7/1990,O,5/7/2020,CADETE,5/7/2022,I,5/7/2001
                24584852,Gonzales,Joel,5/7/1986,M,5/7/2020,CADETE,5/7/2022,A,5/7/2012
                32586853,Messi,Juan,5/7/1996,M,5/7/2020,CADETE,5/7/2022,A,5/7/2018
                16588854,Ronaldo,Lola,5/7/1950,O,5/7/2020,JUBILADO,5/7/2022,I,5/7/2006
                39581855,Lopez,Camila,5/7/2002,F,5/7/2020,CADETE,5/7/2022,A,5/7/2011
                42582856,Fernandez,Matias,5/7/1995,M,5/7/2020,HONORARIO,5/7/2022,A,5/7/2023
                28586857,Rodrigez,Jose,5/7/1980,M,5/7/2020,JUBILADO,5/7/2022,A,5/7/2020*/

  fscanf(t,"%ld,%60[^,],%60[^,],%d/%d/%d,%c,%d/%d/%d,%10[^,],%d/%d/%d,%c,%d/%d/%d"
               ,&soc.DNI,soc.apellido,soc.nombre,&soc.fechaNac.dia,&soc.fechaNac.mes,&soc.fechaNac.anio,
               &soc.sexo,&soc.fechaAfi.dia,&soc.fechaAfi.mes,&soc.fechaAfi.anio,soc.categoria,
               &soc.fechaUlt.dia,&soc.fechaUlt.mes,&soc.fechaUlt.anio,&soc.estado,
               &soc.fechaBaja.dia,&soc.fechaBaja.mes,&soc.fechaBaja.anio
               );
     while(!feof(t))
     {
        if(soc.estado != 'B')
            fwrite(&soc,sizeof(tSocio),1,b);

        fscanf(t,"%ld,%60[^,],%60[^,],%d/%d/%d,%c,%d/%d/%d,%10[^,],%d/%d/%d,%c,%d/%d/%d"
               ,&soc.DNI,soc.apellido,soc.nombre,&soc.fechaNac.dia,&soc.fechaNac.mes,&soc.fechaNac.anio,
               &soc.sexo,&soc.fechaAfi.dia,&soc.fechaAfi.mes,&soc.fechaAfi.anio,soc.categoria,
               &soc.fechaUlt.dia,&soc.fechaUlt.mes,&soc.fechaUlt.anio,&soc.estado,
               &soc.fechaBaja.dia,&soc.fechaBaja.mes,&soc.fechaBaja.anio
               );
     }
    fclose(t);
    fclose(b);
    return TODO_BIEN;
}
int existeCsv(char* archNombreTexto)
{
    FILE* arch = fopen(archNombreTexto,"rb");
    if(!arch)
        return !TODO_BIEN;
    fclose(arch);
    return TODO_BIEN;
}
void procesarNuevoSocio(tSocio* soc, tFecha* hoy)
{
    soc->fechaAfi.dia = hoy->dia;
    soc->fechaAfi.mes = hoy->mes;
    soc->fechaAfi.anio = hoy->anio;

    soc->estado = 'A';

    soc->fechaBaja.dia = 0;
    soc->fechaBaja.mes = 0;
    soc->fechaBaja.anio = 0;
}
void ingresarDatosSocios(tSocio* soc)
{
    puts("ingrese apellido:");
    scanf("%s", soc->apellido);

    puts("ingrese nombre:");
    scanf("%s", soc->nombre);

    puts("ingrese fecha de nacimiento (dd/mm/aaaa)");
    scanf("%d/%d/%d", &soc->fechaNac.dia, &soc->fechaNac.mes, &soc->fechaNac.anio);
    while(!esFechaValida(&soc->fechaNac))
    {
        puts("fecha invalida. ingrese nuevamente una fecha de nacimiento valida");
        fflush(stdin);
        scanf("%d/%d/%d", &soc->fechaNac.dia, &soc->fechaNac.mes, &soc->fechaNac.anio);
    }

    puts("ingrese sexo: ");
    fflush(stdin);
    scanf("%c", &soc->sexo);
    while(!sexoValido(soc->sexo))
    {
        puts("dato invalido, ingrese nuevamente el sexo:");
        fflush(stdin);
        scanf("%c", &soc->sexo);
    }

    puts("ingrese categoria:");
    scanf("%s", soc->categoria);
    while(!categoriaValida(soc->categoria))
    {
        puts("categoria invalida, ingrese nuevamente la categoria:");
        fflush(stdin);
        scanf("%s", soc->categoria);
    }

    puts("ingrese fecha de ultima cuota paga (dd/mm/aaaa):");
    scanf("%d/%d/%d", &soc->fechaUlt.dia, &soc->fechaUlt.mes, &soc->fechaUlt.anio);

    while(!esFechaValida(&soc->fechaUlt))
    {
        puts("fecha invalida. ingrese una fecha valida");
        fflush(stdin);
        scanf("%d/%d/%d", &soc->fechaUlt.dia, &soc->fechaUlt.mes, &soc->fechaUlt.anio);
    }
}
int categoriaValida(char* cat)
{
    //
    if(strcmp(cat, "MENOR")!=0)
        if(strcmp(cat, "CADETE")!=0)
            if(strcmp(cat, "ADULTO")!=0)
                if(strcmp(cat, "VITALICIO")!=0)
                    if(strcmp(cat, "HONORARIO")!=0)
                        if(strcmp(cat, "JUBILADO")!=0)
                            return !TODO_BIEN;
    return TODO_BIEN;
}
int esFechaValida(const tFecha* fecha)
{
    if(fecha->anio >= 1601)
        if(fecha->mes >=1 && fecha->mes <=12)
            if(fecha->dia >=1 && fecha->dia <= cantDiasMes(fecha->mes, fecha->anio))
                return TODO_BIEN;
    return !TODO_BIEN;
}
int cantDiasMes(int mes, int anio)
{
    static int cdm[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if(mes==2 && esBisiesto(anio))
        return 29;
    return cdm[mes];
}
void buscarYMostrarSociosOrdenado(void* info , unsigned tam,void* fp)
{
    tSocio soc;
    unsigned nro_reg;
    memcpy(&nro_reg,((char*)info) + sizeof(long),sizeof(unsigned));
    fseek((FILE*)fp,nro_reg*sizeof(tSocio),SEEK_SET);
    fread(&soc,sizeof(tSocio),1,(FILE*)fp);
    imprimirSocio(soc);
}
int cmpFecha(const void* a , const void* b)
{
    ///tFecha* a1 = memcpy(a1,*(char**)a,sizeof(tFecha));
    ///tFecha* b1 = memcpy(b1,*(char**)b,sizeof(tFecha));
    FechaInd* a1 = (FechaInd*)a;
    FechaInd* b1 = (FechaInd*)b;
    int cmp = a1->fech.anio - b1->fech.anio;
    if(cmp)
        return cmp;
    cmp = a1->fech.mes - b1->fech.mes;
    if(cmp)
        return cmp;
    return a1->fech.dia - b1->fech.dia;
}

void mostrarCampos()
{
    char campos[][50] = {"DNI","|NOMBRE","|APELLIDO","|FECHA NAC","|SEXO","|FECHA AFIL","|CATEGORIA","|FECHA ULTI|","ESTADO|"};
    puts("------------------------------------------------------------------------------------------");
    printf("%s %11s %13s %12s %s %s%s %s%s\n",campos[0],campos[1],campos[2],campos[3],campos[4],campos[5],campos[6],campos[7],campos[8]);
    puts("------------------------------------------------------------------------------------------");
}
void imprimirSocio(tSocio soc)
{
    printf("%8ld|%-11s|%-11s|%02d-%02d-%d|--%c--|%02d-%02d-%d|%-10s|%02d-%02d-%d|--%c--\n",soc.DNI,soc.nombre,soc.apellido,soc.fechaNac.dia,soc.fechaNac.mes,soc.fechaNac.anio,
           soc.sexo,
           soc.fechaAfi.dia,soc.fechaAfi.mes,soc.fechaAfi.anio,
           soc.categoria,
           soc.fechaUlt.dia,soc.fechaUlt.mes,soc.fechaUlt.anio,
           soc.estado);
}

void obtenerFechaHoy (tFecha *fechbaja)
{
    time_t tiempo = time(NULL);
    struct tm tiempoTranscurrido = *localtime(&tiempo);
    fechbaja->dia = tiempoTranscurrido.tm_mday;
    fechbaja->mes = tiempoTranscurrido.tm_mon + 1;
    fechbaja->anio = tiempoTranscurrido.tm_year + 1900;
}
