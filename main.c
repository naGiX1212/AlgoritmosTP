#include "func.h"
int main()
{

    char archNombreTexto[TAM];
    char* archNombreBin = "socios.dat";
    char* idxNombreBin = "socios.idx";
    char res;
    ///PRIMERA APLICACION
    puts("-----------------------Primera aplicacion------------------------------------------");

    do{
        puts("Ingrese el nombre del archivo CSV abrir");
        scanf("%s",archNombreTexto);

    }while(!existeCsv(archNombreTexto));
    if(!mostrarSociosBinario(archNombreBin))
    {
        textoABinario(archNombreTexto,archNombreBin);
        crearArchIndice(archNombreBin,idxNombreBin);
    }
    puts("\nARCHIVO INDICE");
    if(!mostrarIndexBinario(idxNombreBin))
    {
        puts("No se encontro el archivo INDEX, CREANDO...");
        crearArchIndice(archNombreBin,idxNombreBin);
        system("pause");
        mostrarIndexBinario(idxNombreBin);
        system("pause");
    }
   ///COMIENZO DE LA SEGUNDA APLICACION
    t_indice ind;
    ind_crear(&ind,sizeof(long),cmpDni);
    ind_cargar(&ind,idxNombreBin);
    puts("Indice en Memoria");
    ind_recorrer(&ind,mostrarClave,NULL);
    puts("");
    system("pause");
    do
    {
        system("cls");
        mostrarMenu();
        fflush(stdin);
        puts("");
        res = getchar();
    }while(opciones(res,&ind));
    ind_grabar(&ind,idxNombreBin);
    ind_vaciar(&ind);
    puts("---------------------------------FIN DE TRABAJO PRACTICO----------------------------");
    mostrarSociosBinario(archNombreBin);
    return 0;
}
int crearArchIndice(char* nombreBSocios,char* nombreIndice)
{
    unsigned cont = 0;
    t_indice ind;
    ind_crear(&ind,sizeof(long),cmpDni);
    tSocio soc;
    FILE* archB;
    archB = fopen(nombreBSocios,"rb");
    if(!archB)
    {
        return ERROR_ARCH;
    }
    fread(&soc,sizeof(tSocio),1,archB);
    while(!feof(archB))
    {
        if(soc.estado != 'B')
            ind_insertar(&ind,&soc.DNI,cont);
        fread(&soc,sizeof(tSocio),1,archB);
        cont++;
    }
    fclose(archB);
    ind_grabar(&ind,nombreIndice);///GRABO EL INDICE EN UN ARCHIVO
    ind_vaciar(&ind);
    return TODO_BIEN;
}
int alta(t_indice* ind)
{
    int ret;
    tSocio nueSoc;
    unsigned nroReg;
    int cantReg;
    FILE* pf = fopen("socios.dat", "r+b");

    if(!pf)
    {
        puts("Error al abrir archivo socios. no se puede dar de alta al usuario");
        system("pause");
        return !TODO_BIEN;
    }
    //fecha del dia, solicitar ingreso de fecha del dia o usar time.h
    tFecha hoy;
    //obtenerFechaHoy(&hoy);
    hoy.dia = 18;
    hoy.mes = 6;
    hoy.anio = 2024;

    system("cls");
    puts("ingrese dni: ");
    scanf("%ld", &nueSoc.DNI);

    while(!dniValido(nueSoc.DNI))
    {
        puts("DNI invalido, ingrese nuevamente un DNI:");
        fflush(stdin);
        scanf("%ld", &nueSoc.DNI);
    }
    ret = ind_buscar(ind, &nueSoc.DNI, &nroReg);

    if(ret == !TODO_BIEN)
    {
        ingresarDatosSocios(&nueSoc);
        system("pause");
        system("cls");
        puts("No se encontro el socio. Generando nuevo socio con los datos ingresados...");
        system("pause");
        procesarNuevoSocio(&nueSoc, &hoy);

        //insertar nuevo socio en archivo socios al final
        fseek(pf, 0, SEEK_END);
        cantReg = ftell(pf)/sizeof(tSocio);//para usar como nroReg para un nuevo socio
        fwrite(&nueSoc, sizeof(tSocio), 1, pf);
        //inserto en nuevo socio en indice
        if(ind_insertar(ind, &nueSoc.DNI, cantReg))
            puts("socio insertado en indice con exito!");
    }
    else
    {
        //existe socio en indice, se desconoce si esta dado de alta o no
        fseek(pf, nroReg*sizeof(tSocio), SEEK_SET);
        fread(&nueSoc, sizeof(tSocio), 1, pf);

        if(nueSoc.estado == 'A' || nueSoc.estado=='a')
        {
            system("cls");
            puts("Socio encontrado. El socio ya esta dado de alta, no es necesaria ninguna accion.");
            system("pause");
        }
        else
        {
            system("cls");
            puts("Socio encontrado inactivo o dado de baja.");
            puts("se requiere continuar con la carga de datos del socio para generar el alta.\n");
            ingresarDatosSocios(&nueSoc);
            system("pause");
            procesarNuevoSocio(&nueSoc, &hoy);
            fseek(pf, -(long)sizeof(tSocio), SEEK_CUR);
            fseek(pf,0,SEEK_CUR);
            fwrite(&nueSoc, sizeof(tSocio), 1, pf);
        }
    }

    fclose(pf);

    if(ret==!TODO_BIEN || nueSoc.estado!='a' || nueSoc.estado!='A')
    {
        puts("\nMostrando archivo actual de socios.dat: ");
        mostrarSociosBinario("socios.dat");
        system("pause");
    }
    return TODO_BIEN;
}
int baja(t_indice* ind)
{
    tSocio soc;
    FILE* archSocios;
    unsigned nro_reg;
    puts("Ingrese DNI de Socio a Eliminar");
    ingresarDni(&soc.DNI);
    if(!ind_buscar(ind,&soc.DNI,&nro_reg))
    {
        puts("No se encontro el DNI\n");
        system("pause");
        return !TODO_BIEN;
    }
    archSocios = fopen("socios.dat","r+b");
    if (!archSocios)
    {
        perror("Error al abrir el archivo de socios");
        return !TODO_BIEN;
    }
    fseek(archSocios,(long)(nro_reg*sizeof(tSocio)),SEEK_SET);
    fread(&soc,sizeof(tSocio),1,archSocios);
    if(soc.estado == 'B')
    {
        puts("El socio ya esta dado de baja.\n");
        fclose(archSocios);
        system("pause");
        return !TODO_BIEN;
    }
    soc.estado = 'B';
    /// podemos poner la fecha con time.h  o la hardcodeamos
    obtenerFechaHoy(&soc.fechaBaja);
//    soc.fechaBaja.dia = 21;
//    soc.fechaBaja.mes = 06;
//    soc.fechaBaja.anio = 2024;

    fseek(archSocios,-(long)sizeof(tSocio),SEEK_CUR);
    fwrite(&soc,sizeof(tSocio),1,archSocios);
    fclose(archSocios);

    ind_eliminar(ind, &soc.DNI, &nro_reg);
    puts ("Socio dado de baja correctamente.\n");
    system("pause");
    return TODO_BIEN;
}
int modificar(t_indice* ind)
{
    tSocio soc;
    FILE* archSocios;
    unsigned nro_reg;
    char op;

    ingresarDni(&soc.DNI);
    if(!ind_buscar(ind,&soc.DNI,&nro_reg))
    {
        puts("No se encontro el DNI\n");
        system("pause");
        return !TODO_BIEN;
    }
    do
    {
        system("cls");
        puts("Ingrese campo a modificar");
        puts("(A) Apellido");
        puts("(N) Nombre");
        puts("(S) Sexo");
        puts("(F) Fecha de ultima cuota paga");
        puts("(R) Retornar");
        fflush(stdin);
        scanf("%c",&op);
        op=AMAYUS(op);
    }while(op!='A'&&op!='N'&&op!='S'&&op!='F'&&op!='R');

    archSocios = fopen("socios.dat","r+b");
    if(!archSocios)
    {
        printf("No se ha podido abrir el archivo\n");
        return !TODO_BIEN;
    }

    if(op=='A')
    {
        fseek(archSocios,nro_reg*sizeof(tSocio),SEEK_SET);
        fread(&soc,sizeof(tSocio),1,archSocios);

        printf("Ingrese nuevo apellido: ");
        scanf("%s",soc.apellido);

        fseek(archSocios,-(long)sizeof(tSocio),SEEK_CUR);
        fwrite(&soc,sizeof(tSocio),1,archSocios);

        fclose(archSocios);

    }
    else
        if(op=='N')
        {
            fseek(archSocios,nro_reg*sizeof(tSocio),SEEK_SET);
            fread(&soc,sizeof(tSocio),1,archSocios);

            printf("Ingrese nuevo nombre: ");
            scanf("%s",soc.nombre);

            fseek(archSocios,-(long)sizeof(tSocio),SEEK_CUR);
            fwrite(&soc,sizeof(tSocio),1,archSocios);

            fclose(archSocios);

        }
        else
            if(op=='S')
            {
                fseek(archSocios,nro_reg*sizeof(tSocio),SEEK_SET);
                fread(&soc,sizeof(tSocio),1,archSocios);

                do
                {
                    printf("Ingrese nuevo sexo: ");
                    scanf("%s",&soc.sexo);
                }while(soc.sexo!='F'&&soc.sexo!='M'&&soc.sexo!='O');

                fseek(archSocios,-(long)sizeof(tSocio),SEEK_CUR);
                fwrite(&soc,sizeof(tSocio),1,archSocios);

                fclose(archSocios);
            }
            else
                if(op=='F')
                {
                    fseek(archSocios,nro_reg*sizeof(tSocio),SEEK_SET);
                    fread(&soc,sizeof(tSocio),1,archSocios);

                    do{
                        printf("Ingrese dia: ");
                        scanf("%d",&soc.fechaUlt.dia);
                    }while(soc.fechaUlt.dia<1||soc.fechaUlt.dia>31);
                    do{
                        printf("Ingrese mes: ");
                        scanf("%d",&soc.fechaUlt.mes);
                    }while(soc.fechaUlt.mes<1||soc.fechaUlt.mes>12);
                    do{
                        printf("Ingrese anio: ");
                        scanf("%d",&soc.fechaUlt.anio);
                    }while(soc.fechaUlt.anio<1970||soc.fechaUlt.anio>2024);

                    fseek(archSocios,-(long)sizeof(tSocio),SEEK_CUR);
                    fwrite(&soc,sizeof(tSocio),1,archSocios);

                    fclose(archSocios);
                }
                else
                {
                    fclose(archSocios);
                    return !TODO_BIEN;
                }

    mostrarSociosBinario("socios.dat");
    system("pause");
    printf("Se ha modificado el campo correctamente!\n");

    return TODO_BIEN;
}
int listarOrdenados(t_indice * ind)
{
    FILE* arch = fopen("socios.dat","rb");
    if(!arch)
    {
        return ERROR_ARCH;
    }
    mostrarCampos();
    ///RECRORRO EL INDICE
    ind_recorrer(ind,buscarYMostrarSociosOrdenado,arch);
    fclose(arch);
    system("pause");

    return TODO_BIEN;
}
void guardarEnVector(void* info , unsigned tam, void* param)
{
   char** a = (char**)param;
   memcpy((*a)+sizeof(tFecha),(char*)info + sizeof(long),sizeof(unsigned));
   *a += sizeof(FechaInd);

}
int mostrarTopTen(FechaInd* f,FILE* arch,int cant)
{
    tSocio soc;
    int i= 0;
    mostrarCampos();
    while(i<10 && cant)
    {
        fseek(arch,f->nro_reg*sizeof(tSocio),SEEK_SET);
        fread(&soc,sizeof(tSocio),1,arch);
        imprimirSocio(soc);
        f++;
        cant--;
        i++;
    }
    system("pause");
    return TODO_BIEN;
}
int listarMayorRetrasos(t_indice* ind)
{
    int cant = cantNodosArbolBin(&ind->arb);
    FechaInd f[cant];
    tSocio soc;
    void* aux = &f[0];
    ind_recorrer(ind,guardarEnVector,&aux);
    FILE* arch = fopen("socios.dat","rb");
    if(!arch)
    {
        return ERROR_ARCH;
    }
    for(int i = 0;i<cant;i++)
    {
        fseek(arch,f[i].nro_reg*sizeof(tSocio),SEEK_SET);
        fread(&soc,sizeof(tSocio),1,arch);
        f[i].fech.dia = soc.fechaUlt.dia;
        f[i].fech.mes = soc.fechaUlt.mes;
        f[i].fech.anio = soc.fechaUlt.anio;
    }
    qsort(f,cant,sizeof(FechaInd),cmpFecha);
    mostrarTopTen(f,arch,cant);
    fclose(arch);
    return TODO_BIEN;
}

int opciones(char op,t_indice * ind)
{
    system("cls");
    switch(op)
    {
    case 'S':return !TODO_BIEN;;
        break;

    case 'A':alta(ind);
        break;
    case 'M':modificar(ind);
        break;
    case 'B':baja(ind);
        break;
    case 'L':listarOrdenados(ind);
        break;
    case 'P':listarMayorRetrasos(ind);
        break;
    }

    return TODO_BIEN;
}



