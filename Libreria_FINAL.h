#ifndef LIBRERIA_SPN_H_INCLUDED
#define LIBRERIA_SPN_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define RESET    "\x1b[0m"
#define NEGRO_T        "\x1b[30m"
#define NEGRO_F        "\x1b[40m"
#define ROJO_T     "\x1b[31m"
#define ROJO_F     "\x1b[41m"
#define VERDE_T        "\x1b[32m"
#define VERDE_F        "\x1b[42m"
#define AMARILLO_T "\x1b[33m"
#define    AMARILLO_F  "\x1b[43m"
#define AZUL_T     "\x1b[34m"
#define    AZUL_F      "\x1b[44m"
#define MAGENTA_T  "\x1b[35m"
#define MAGENTA_F  "\x1b[45m"
#define CYAN_T     "\x1b[36m"
#define CYAN_F     "\x1b[46m"
#define BLANCO_T   "\x1b[37m"
#define BLANCO_F   "\x1b[47m"
//Definicion de estructura
struct Trabajo {
    int nombre;
    int prioridad;
    int tiempoLlegada;
    int t_CPU;
    int tiempoRestante; // Agregamos un campo para el tiempo restante
    int tiempoInicio; // Nuevo campo para el tiempo de inicio de ejecución
    int tiempoFinal; // Nuevo campo para el tiempo de finalización de ejecución
    int tiempoEspera; // Nuevo campo para el tiempo de espera
    int tiempoRetorno; // Nuevo campo para el tiempo de retorno
    int tiempoServicio;
    float tiempoRetornoNormalizado;
};

void cargarTrabajosDesdeArchivo(struct Trabajo trabajos[], int n, char* nombreArchivo) {
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo.\n");
    }

    // Leemos el primer número que indica la cantidad de trabajos
    fscanf(archivo, "%d", &n);

    for (int i = 0; i < n; i++) {
        fscanf(archivo, " %d %d %d %d", &trabajos[i].nombre, &trabajos[i].prioridad, &trabajos[i].tiempoLlegada, &trabajos[i].t_CPU);
        trabajos[i].tiempoRestante = trabajos[i].t_CPU;
    }

    fclose(archivo);
}

float programacionPorSRT(struct Trabajo trabajos[], int n) {
    FILE *archivo1 = fopen("FINAL_RESULTADOS.txt","w");
    fprintf(archivo1,"DESARROLLO DE SRT\n");
    int tiempoActual = 0;
    int trabajosTerminados = 0;
    int tiempoServicioTotal = 0;
    int tiempoRetornoTotal = 0;
    int tiempoEsperaTotal = 0;
    for (int i = 0; i < n;i++) {
    printf("Ingrese PID de proceso %d:\n", i); scanf("%d",&trabajos[i].nombre);
    printf("Ingrese Prioridad de proceso %d:\n", i); scanf("%d",&trabajos[i].prioridad);
    printf("Ingrese Tiempo de Llegada de proceso %d:\n", i); scanf("%d",&trabajos[i].tiempoLlegada);
    printf("Ingrese Tiempo de CPU de proceso %d:\n", i); scanf("%d",&trabajos[i].t_CPU);
    trabajos[i].tiempoRestante = trabajos[i].t_CPU;
    }
    while (trabajosTerminados < n) {
        int trabajoElegido = -1;
        int menorTiempoRestante = 9999;

        for (int i = 0; i < n; i++) {
            if (trabajos[i].tiempoLlegada <= tiempoActual && trabajos[i].tiempoRestante > 0) {
                if (trabajos[i].tiempoRestante < menorTiempoRestante ||
                    (trabajos[i].tiempoRestante == menorTiempoRestante && trabajos[i].prioridad < trabajos[trabajoElegido].prioridad) ||
                    (trabajos[i].tiempoRestante == menorTiempoRestante && trabajos[i].prioridad == trabajos[trabajoElegido].prioridad &&
                     trabajos[i].tiempoLlegada < trabajos[trabajoElegido].tiempoLlegada)) {
                    trabajoElegido = i;
                    menorTiempoRestante = trabajos[i].tiempoRestante;
                }
            }
        }

        if (trabajoElegido != -1) {
            trabajos[trabajoElegido].tiempoRestante--;

            if (trabajos[trabajoElegido].tiempoRestante == 0) {
                trabajosTerminados++;

                trabajos[trabajoElegido].tiempoFinal = tiempoActual;
                trabajos[trabajoElegido].tiempoRetorno = trabajos[trabajoElegido].tiempoFinal - trabajos[trabajoElegido].tiempoLlegada+1;
                trabajos[trabajoElegido].tiempoEspera = trabajos[trabajoElegido].tiempoRetorno - trabajos[trabajoElegido].t_CPU;
                trabajos[trabajoElegido].tiempoServicio = trabajos[trabajoElegido].tiempoRetorno - trabajos[trabajoElegido].tiempoEspera;
                trabajos[trabajoElegido].tiempoRetornoNormalizado = trabajos[trabajoElegido].tiempoRetorno/ (float)trabajos[trabajoElegido].tiempoServicio;

                tiempoServicioTotal += trabajos[trabajoElegido].tiempoServicio;
                tiempoRetornoTotal += trabajos[trabajoElegido].tiempoRetorno;
                tiempoEsperaTotal += trabajos[trabajoElegido].tiempoEspera;
            }
        }
        tiempoActual++;
    }
    printf(VERDE_T);
    printf("PID\tPri\tLle\tCPU\n");
    printf(RESET);
    for(int i=0;i<n;i++) {
        printf(CYAN_T"%d\t" MAGENTA_T "%d\t" AMARILLO_T "%d\t" ROJO_T"%d\n",
               trabajos[i].nombre,
               trabajos[i].prioridad,
               trabajos[i].tiempoLlegada,
               trabajos[i].t_CPU);
    }
    printf("\n");
    Imprimir_caracter('*',63,archivo1);
    printf(BLANCO_F NEGRO_T "RESULTADOS FINALES" RESET "\n");
    fprintf(archivo1,"RESULTADOS FINALES\n");
    printf("\n");
    printf(VERDE_T);
    printf("PID\tPri\tLle\tCPU\tSAL\tRET\tESP\tTRN\n");
    printf(RESET);
    for(int i=0;i<n;i++) {
        printf(CYAN_T "%d\t" MAGENTA_T "%d\t"  AMARILLO_T "%d\t" ROJO_T "%d\t" CYAN_T"%d\t" MAGENTA_T"%d\t" AMARILLO_T"%d\t" ROJO_T"%.3f\n" RESET,
               trabajos[i].nombre,
               trabajos[i].prioridad,
               trabajos[i].tiempoLlegada,
               trabajos[i].t_CPU,
               trabajos[i].tiempoFinal,
               trabajos[i].tiempoRetorno,
               trabajos[i].tiempoEspera,
               trabajos[i].tiempoRetornoNormalizado);
    }
    printf("\n");
    fprintf(archivo1,"PID\tPri\tLle\tCPU\tSAL\tRET\tESP\tTRN\n");
    for(int i=0;i<n;i++) {
        fprintf(archivo1,"%d\t%d\t%d\t%d\t%d\t%d\t%d\t%.3f\n",
               trabajos[i].nombre,
               trabajos[i].prioridad,
               trabajos[i].tiempoLlegada,
               trabajos[i].t_CPU,
               trabajos[i].tiempoFinal,
               trabajos[i].tiempoRetorno,
               trabajos[i].tiempoEspera,
               trabajos[i].tiempoRetornoNormalizado);
    }
    printf("\n");
    float promedioRetorno = (float)tiempoRetornoTotal / n;
    float promedioEspera = (float)tiempoEsperaTotal / n;
    float promedioServicio = promedioRetorno - promedioEspera;

    printf("Promedio de Tiempo de Retorno: %.3f\n", promedioRetorno);
    printf("Promedio de Tiempo de Espera: %.3f\n", promedioEspera);
    printf("Promedio de Tiempo de Servicio: %.3f\n", promedioServicio);
    fprintf(archivo1,"Promedio de Tiempo de Retorno: %.3f\n", promedioRetorno);
    fprintf(archivo1,"Promedio de Tiempo de Espera: %.3f\n", promedioEspera);
    fprintf(archivo1,"Promedio de Tiempo de Servicio: %.3f\n", promedioServicio);

    float tiempoRetornoNormalizadoPromedio = promedioRetorno / promedioServicio;
    printf("Tiempo de Retorno Normalizado del Promedio: " AMARILLO_F NEGRO_T "%.3f" RESET "\n", tiempoRetornoNormalizadoPromedio);
    fprintf(archivo1,"Tiempo de Retorno Normalizado del Promedio: %.3f\n", tiempoRetornoNormalizadoPromedio);

    Imprimir_caracter('*',63,archivo1);
    fclose(archivo1);
    return tiempoRetornoNormalizadoPromedio;
}
typedef struct Procesos {
    int PID, Prioridad, T_LLEGADA, T_CPU, T_SALIDA, T_RETORNO, T_ESPERA;
    double TRN;
} Procesos;
typedef struct Conta_Indice {
    int  conta;
    int indices[5];
} Conta_Indice;
//Lectura de datos por teclado
int Leer_entero (char *texto) {
    int a;
    printf("%s",texto);
    scanf("%d",&a);
    return a;
}
double Leer_double (char *texto) {
    double a;
    printf("%s",texto);
    scanf("%lf",&a);
    return a;
}
//Funciones de creacion de dinamicos
void dynamic_arr(void **variable, size_t espacio, size_t conta) {
    *variable = malloc(espacio*conta);
    if (variable == NULL) {
        printf("ERROR");
    } else {
        ;
    }
}
void resize_arr(void **variable, size_t espacio, size_t nuevo_conta) {
    *variable = realloc(*variable,espacio*nuevo_conta);
    if (variable == NULL) {
        printf("ERROR");
    } else {
        ;
    }
}
void intercambiar(void *a, void*b, size_t tamano) {
    void *tmp = malloc(tamano);
    memcpy(tmp,a, tamano);
    memcpy(a,b, tamano);
    memcpy(b,tmp, tamano);
    free(tmp);
}
void particion( void** arreglo, int inicio, int termino, size_t tamano) {
    void *pivote = malloc(tamano);
    printf("%zu",tamano);
}
Conta_Indice Conta_Indice_CPU(Procesos *arreglo, int size, int coincidencia, int cronometro) {
    Conta_Indice Resultado;
    int contador = 0;
    for (int i = 0; i < size; i++) {
        if (arreglo[i].T_CPU == coincidencia && arreglo[i].T_LLEGADA <= cronometro) {
            contador+=1;
            }
        }
    int const CONT = contador;
    for (int i = 0; i < CONT; i++) {
        if (arreglo[i].T_CPU == coincidencia && arreglo[i].T_LLEGADA <= cronometro) {
            Resultado.indices[i] = i;
            }
        }
    Resultado.conta = CONT;
    return Resultado;
}
Conta_Indice Conta_Indice_PRIO(Procesos *arreglo, int size, int coincidencia, int cronometro) {
    Conta_Indice Resultado;
    int contador = 0;
    for (int i = 0; i < size; i++) {
        if (arreglo[i].Prioridad == coincidencia && arreglo[i].T_LLEGADA <= cronometro) {
            contador+=1;
            }
        }
    int const CONT = contador;
    for (int i = 0; i < CONT; i++) {
        if (arreglo[i].Prioridad == coincidencia && arreglo[i].T_LLEGADA <= cronometro) {
            Resultado.indices[i] = i;
            }
        }
    Resultado.conta = CONT;
    return Resultado;
}
Conta_Indice Conta_Indice_LLEGADA(Procesos *arreglo, int size, int coincidencia, int cronometro) {
    Conta_Indice Resultado;
    int contador = 0;
    for (int i = 0; i < size; i++) {
        if (arreglo[i].T_LLEGADA == coincidencia && arreglo[i].T_LLEGADA <= cronometro) {
            contador+=1;
            }
        }
    int const CONT = contador;
    for (int i = 0; i < CONT; i++) {
        if (arreglo[i].T_LLEGADA == coincidencia && arreglo[i].T_LLEGADA <= cronometro) {
            Resultado.indices[i] = i;
            }
        }
    Resultado.conta = CONT;
    return Resultado;
}
int compareLLEGADA(const void *a, const void *b) {
    return ((const Procesos*)a)-> T_LLEGADA - ((const Procesos*)b)->T_LLEGADA;
    }
int compareLLEGADAD(const void *a, const void *b) {
return ((const Procesos*)b)-> T_LLEGADA - ((const Procesos*)a)->T_LLEGADA;
}
int compareCPU(const void *a, const void *b) {
    return ((const Procesos*)a)-> T_CPU - ((const Procesos*)b)->T_CPU;
    }
int comparePRIO(const void *a, const void *b) {
    return ((const Procesos*)a)->Prioridad - ((const Procesos*)b)->Prioridad;
    }
int comparePID(const void *a, const void *b) {
return ((const Procesos*)a)->PID - ((const Procesos*)b)->PID;
}
void CalcularParametros(Procesos *PX, int *cronos) {
    (*cronos) += PX->T_CPU;
    PX->T_RETORNO = (*cronos) - PX->T_LLEGADA;
    PX->T_ESPERA = PX->T_RETORNO - PX->T_CPU;
    PX->TRN = (double)PX->T_RETORNO / PX->T_CPU;
    PX->T_SALIDA = (*cronos);
}
int buscarIndicePID(Procesos *arr, int PID, int tamano, int crono) {
    int ind;
    for (int i = 0; i < tamano;i++) {
        if (arr[i].PID == PID && arr[i].T_LLEGADA <= crono) {
        ind = i;
        return ind;
        }
    }
    return -1;
}
int buscarIndicePRIO(Procesos *arr, int PRIO, int tamano, int crono) {
    int ind;
    for (int i = 0; i < tamano;i++) {
        if (arr[i].Prioridad == PRIO && arr[i].T_LLEGADA <= crono) {
        ind = i;
        return ind;
        }
    }
    return -1;
}
int buscarIndiceCPU(Procesos *arr, int CPU, int tamano, int crono) {
    int ind;
    for (int i = 0; i < tamano;i++) {
        if (arr[i].T_CPU == CPU && arr[i].T_LLEGADA <= crono) {
        ind = i;
        return ind;
        }
    }
    return -1;
}
int buscarIndiceLLEGADA(Procesos *arr, int LLEGADA, int tamano, int crono) {
    int ind;
    for (int i = 0; i < tamano;i++) {
        if (arr[i].T_LLEGADA == LLEGADA && arr[i].T_LLEGADA <= crono) {
        ind = i;
        return ind;
        }
    }
    return -1;
}
void MostrarPrimero(Procesos *arreglo, int tamano) {
    printf(VERDE_T);
    printf("PID\tPri\tLle\tCPU\n");
    printf(RESET);
    for(int i=0;i<tamano;i++) {
        printf(CYAN_T"%d\t" MAGENTA_T "%d\t" AMARILLO_T "%d\t" ROJO_T "%d\n" RESET,
               arreglo[i].PID,
               arreglo[i].Prioridad,
               arreglo[i].T_LLEGADA,
               arreglo[i].T_CPU);
    }
    printf("\n");
}
void MostrarFinal(Procesos *arreglo, int tamano,FILE *archivo1) {
    printf(VERDE_T);
    printf("PID\tPri\tLle\tCPU\tSAL\tRET\tESP\tTRN\n");
    printf(RESET);
    for(int i=0;i<tamano;i++) {
        printf(CYAN_T"%d\t" MAGENTA_T "%d\t" AMARILLO_T "%d\t" ROJO_T "%d\t" CYAN_T "%d\t" MAGENTA_T "%d\t" AMARILLO_T "%d\t" ROJO_T "%.3lf\n" RESET,
               arreglo[i].PID,
               arreglo[i].Prioridad,
               arreglo[i].T_LLEGADA,
               arreglo[i].T_CPU,
               arreglo[i].T_SALIDA,
               arreglo[i].T_RETORNO,
               arreglo[i].T_ESPERA,
               arreglo[i].TRN);
    }
    printf("\n");
    fprintf(archivo1,"PID\tPri\tLle\tCPU\tSAL\tRET\tESP\tTRN\n");
    for(int i=0;i<tamano;i++) {
        fprintf(archivo1,"%d\t%d\t%d\t%d\t%d\t%d\t%d\t%.3lf\n",
               arreglo[i].PID,
               arreglo[i].Prioridad,
               arreglo[i].T_LLEGADA,
               arreglo[i].T_CPU,
               arreglo[i].T_SALIDA,
               arreglo[i].T_RETORNO,
               arreglo[i].T_ESPERA,
               arreglo[i].TRN);
    }
    printf("\n");
}

double CalcularRetornoP(Procesos *arreglo, int tamano) {
    double acumula = 0;
    for( int i = 0; i < tamano;i++) {
        acumula+=arreglo[i].T_RETORNO;
    }
    return acumula/5;
}
double CalcularEsperaP(Procesos *arreglo, int tamano) {
    double acumula = 0;
    for (int i = 0; i < tamano; i++) {
        acumula+=arreglo[i].T_ESPERA;
    }
    return acumula/5;
}

int CalcularTodoPrimero(int insize,Procesos *Lista,Procesos *Final,Procesos *Secuencial,int parametro, int crono, int tamano, char opcion) {
    int indicadorOrigen, indicadorSele;
    switch (opcion) {
    case 'L':
        indicadorOrigen = buscarIndiceLLEGADA(Lista,parametro,insize,crono);
        indicadorSele = buscarIndiceLLEGADA(Secuencial,parametro,tamano, crono);
        break;
    case 'C' :
        indicadorOrigen = buscarIndiceCPU(Lista,parametro,insize, crono);
        indicadorSele = buscarIndiceCPU(Secuencial,parametro,tamano, crono);
        break;
    case 'P':
        indicadorOrigen = buscarIndicePRIO(Lista,parametro,insize, crono);
        indicadorSele = buscarIndicePRIO(Secuencial,parametro,tamano, crono);
        break;
    case 'I':
        indicadorOrigen = buscarIndicePID(Lista,parametro,insize, crono);
        indicadorSele = buscarIndicePID(Secuencial,parametro,tamano, crono);
        break;
    default:
        printf("Error\n");
    }
    int ahora = insize - 1;
    CalcularParametros(&Secuencial[indicadorSele],&crono);
    Lista[indicadorOrigen] = Secuencial[indicadorSele];
    Final[0] = Secuencial[indicadorSele];
    intercambiar(&Lista[indicadorOrigen],&Lista[ahora],sizeof(Procesos));
    resize_arr((void**)&Lista,sizeof(Procesos),ahora);
    return crono;
}

int CalcularTodoPosterior(int insize,Procesos *Lista,Procesos *Final,Procesos *Secuencial,int parametro, int crono, int tamano, char opcion, int repeticion) {
    int indicadorOrigen, indicadorSele;
    switch (opcion) {
    case 'L':
        indicadorOrigen = buscarIndiceLLEGADA(Lista,parametro,insize, crono);
        indicadorSele = buscarIndiceLLEGADA(Secuencial,parametro,tamano, crono);
        break;
    case 'C' :
        indicadorOrigen = buscarIndiceCPU(Lista,parametro,insize, crono);
        indicadorSele = buscarIndiceCPU(Secuencial,parametro,tamano, crono);
        break;
    case 'P':
        indicadorOrigen = buscarIndicePRIO(Lista,parametro,insize, crono);
        indicadorSele = buscarIndicePRIO(Secuencial,parametro,tamano, crono);
        break;
    case 'I':
        indicadorOrigen = buscarIndicePID(Lista,parametro,insize, crono);
        indicadorSele = buscarIndicePID(Secuencial,parametro,tamano, crono);
        break;
    default:
        printf("Error\n");
    }
    int ahora = insize -1;
    CalcularParametros(&Secuencial[indicadorSele],&crono);
    Lista[indicadorOrigen] = Secuencial[indicadorSele];
    Final[repeticion] = Secuencial[indicadorSele];
    intercambiar(&Lista[indicadorOrigen],&Lista[ahora],sizeof(Procesos));
    resize_arr((void**)&Lista,sizeof(Procesos),ahora);
    return crono;
}

int filtroCPU(Procesos *arr, int tamano, int cronometro) {
    int menor = 9999;
    for (int i = 0;i < tamano;i++) {
        if (arr[i].T_CPU < menor && arr[i].T_LLEGADA <= cronometro) {
            menor = arr[i].T_CPU;
        }
    }
    return menor;
}

void filtrar(Procesos *Van, Procesos *Novan, int *tamanoSI, int *tamanoNO, int crono) {
    if ((*tamanoSI) !=0) {
        qsort(Van,(*tamanoSI), sizeof(Procesos),compareLLEGADA);
    int indice = 0;
    int validador = (*tamanoNO);
    for (int i = 0;i < (*tamanoSI);i++) {
        if(Van[i].T_LLEGADA > crono) {
            Novan[indice] = Van[i];
            indice++;
            (*tamanoNO) +=1;
        }
    }

    if (validador != (*tamanoNO)) {
        int nuevo = (*tamanoSI) - (*tamanoNO);
        (*tamanoSI) = nuevo;
        resize_arr((void*)&Van,sizeof(Procesos),(*tamanoSI));
    }
    }
}
void anadirCapilla(Procesos *Van, Procesos *Novan, int *tamanoSI, int *tamanoNO, int crono) {
    int validador = (*tamanoSI);
    int veces = 0;
    for (int i = 0;i < (*tamanoNO);i++) {
        if(Novan[i].T_LLEGADA <= crono) {
            veces++;
            resize_arr((void*)&Van,sizeof(Procesos),(*tamanoSI+1));
            Van[*tamanoSI] = Novan[i];
            (*tamanoSI)+=1;
        }
    }
    qsort(Novan,(*tamanoNO), sizeof(Procesos),compareLLEGADAD);
    if (validador != (*tamanoSI)) {
        int nuevo = (*tamanoNO) - veces;
        (*tamanoNO) = nuevo;
        if ((*tamanoNO) != 0) {
            resize_arr((void*)&Van,sizeof(Procesos),(*tamanoNO));
        }
    }
}

int MENOR_CPU(Procesos *arr, int tamano, int cronometro) {
    int menor = 9999;
    for (int i = 0; i < tamano;i++) {
        if (arr[i].T_CPU < menor && arr[i].T_LLEGADA <= cronometro) {
            menor = arr[i].T_CPU;
        }
    }
    return menor;
}

int MENOR_PRIO(Procesos *arr, int tamano, int cronometro) {
    int menor = 9999;
    for (int i = 0;i < tamano;i++) {
        if (arr[i].Prioridad < menor && arr[i].T_LLEGADA <= cronometro) {
            menor = arr[i].Prioridad;
        }
    }
    return menor;
}

int MENOR_LLEGADA(Procesos *arr, int tamano, int cronometro) {
    int menor = 9999;
    for (int i = 0;i < tamano;i++) {
        if (arr[i].T_LLEGADA < menor && arr[i].T_LLEGADA <= cronometro) {
            menor = arr[i].T_LLEGADA;
        }
    }
    return menor;
}


int MENOR_PID(Procesos *arr, int tamano, int cronometro) {
    int menor = 9999;
    for (int i = 0;i < tamano;i++) {
        if (arr[i].PID < menor && arr[i].T_LLEGADA <= cronometro) {
            menor = arr[i].PID;
        }
    }
    return menor;
}

void Imprimir_caracter(char caracter, int numero, FILE *archivo1) {
    for (int i = 0; i < numero;i++) {
        printf(ROJO_T "%c" RESET,caracter);
        fprintf(archivo1,"%c",caracter);
    }
    printf("\n");
    fprintf(archivo1,"\n");

}

double Display(Procesos *ListaFinal, FILE *archivo1) {
    Imprimir_caracter('*',63,archivo1);
    printf(BLANCO_F NEGRO_T "RESULTADOS FINALES" RESET "\n");
    fprintf(archivo1,"RESULTADOS FINALES\n");
    qsort(ListaFinal,5,sizeof(Procesos),comparePID);
    MostrarFinal(ListaFinal,5,archivo1);
    double RP = CalcularRetornoP(ListaFinal,5);
    printf("Tiempo de Retorno Promedio: %.3lf\n",RP);
    fprintf(archivo1,"Tiempo de Retorno Promedio: %.3lf\n",RP);
    double EP = CalcularEsperaP(ListaFinal,5);
    printf("Tiempo de Espera Promedio: %.3lf\n",EP);
    fprintf(archivo1,"Tiempo de Espera Promedio: %.3lf\n",EP);
    double SP = RP - EP;
    printf("Tiempo de Servicio Promedio: %.3lf\n",SP);
    fprintf(archivo1,"Tiempo de Servicio Promedio: %.3lf\n",SP);
    double TRNP = RP/SP;
    printf("Tiempo de Retorno Normalizado Promedio: " AMARILLO_F NEGRO_T "%.3lf" RESET "\n",TRNP);
    fprintf(archivo1,"Tiempo de Retorno Normalizado Promedio: %.3lf\n",TRNP);
    Imprimir_caracter('*',63,archivo1);
    return TRNP;
}


#endif // LIBRERIA_SPN_H_INCLUDED
