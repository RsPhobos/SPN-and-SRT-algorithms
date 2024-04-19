#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "Libreria_FINAL.h"
int Insize = 5;
int main() {
    int const n = 5;
    struct Trabajo trabajos[n];
    printf(BLANCO_F NEGRO_T"DESARROLLO DE SRT" RESET"\n");
    //cargarTrabajosDesdeArchivo(trabajos, n, "PRUEBAS_FINAL.txt");

    float RPTA_SRT = programacionPorSRT(trabajos, n);
    int Crono = 0;
    int Contador = 5;
    //Apertura de Archivo
    FILE *archivo = fopen("Pruebas_FINAL.txt","r");
    FILE *archivo1 = fopen("FINAL_RESULTADOS.txt","a");
    if (archivo == NULL) {
        printf("Error, no se logro abrir el archivo");
        exit(-1);
    }
    else {
        fscanf(archivo,"%d",&Insize);
    }
    printf(BLANCO_F NEGRO_T"DESARROLLO DE SPN" RESET "\n");
    fprintf(archivo1,"DESARROLLO DE SPN\n");
    // Creacion de dinamicos
    Procesos *ListaOrigen, *ListaFinal;
    dynamic_arr((void **)&ListaOrigen,sizeof(Procesos),Insize);
    dynamic_arr((void **)&ListaFinal,sizeof(Procesos),Insize);

    for (int i = 0; i < Insize;i++) {
    printf("Ingrese PID de proceso %d:\n", i); scanf("%d",&ListaOrigen[i].PID);
    printf("Ingrese Prioridad de proceso %d:\n", i); scanf("%d",&ListaOrigen[i].Prioridad);
    printf("Ingrese Tiempo de Llegada de proceso %d:\n", i); scanf("%d",&ListaOrigen[i].T_LLEGADA);
    printf("Ingrese Tiempo de CPU de proceso %d:\n", i); scanf("%d",&ListaOrigen[i].T_CPU);
    }
    /*Lectura de datos por texto
    for(int i=0;i<Insize;i++) {
        fscanf(archivo,"%d %d %d %d",
               &ListaOrigen[i].PID,
               &ListaOrigen[i].Prioridad,
               &ListaOrigen[i].T_LLEGADA,
               &ListaOrigen[i].T_CPU);
    } */
    //Muestra de datos
    MostrarPrimero(ListaOrigen,Insize);
    //Ordenamiento inicial
    qsort(ListaOrigen,Insize,sizeof(Procesos),compareLLEGADA);
    Crono+=ListaOrigen[0].T_LLEGADA;
    int une = 1, repe = 1;
    //Algoritmo SPN
    while (Contador !=0) {
        //Solo ejecutar para el primer proceso
        if (une == 1) {
        int menor_llegada = MENOR_LLEGADA(ListaOrigen,Insize,Crono);
        Conta_Indice Primera = Conta_Indice_LLEGADA(ListaOrigen, Insize, menor_llegada, Crono);
        if (Primera.conta == 1) {
            int cronito = CalcularTodoPrimero(Insize,ListaOrigen,ListaFinal,ListaOrigen,menor_llegada,Crono,Insize,'L');
            Insize--;
            Contador--;
            une = 2;
            Crono = cronito;
            } else {
            Procesos *tmp1;
            dynamic_arr((void **)&tmp1,sizeof(Procesos),Primera.conta);
            for (int i=0;i<Primera.conta;i++) {
                tmp1[i] = ListaOrigen[Primera.indices[i]];
            }
            // 1er desempate: Menor CPU
            qsort(tmp1,Primera.conta,sizeof(Procesos),compareCPU);
            int menor_CPU = MENOR_CPU(tmp1,Primera.conta,Crono);
            Conta_Indice Segunda = Conta_Indice_CPU(tmp1,Primera.conta,menor_CPU, Crono);
            if (Segunda.conta == 1) {
                int cronito = CalcularTodoPrimero(Insize,ListaOrigen,ListaFinal,ListaOrigen,menor_CPU,Crono,Primera.conta,'C');
                Insize--;
                Contador--;
                une = 2;
                Crono = cronito;
                } else {
                Procesos *tmp2;
                dynamic_arr((void **)&tmp2,sizeof(Procesos),Segunda.conta);
                for (int i=0;i<Segunda.conta;i++) {
                    tmp2[i] = tmp1[Segunda.indices[i]];
                }
                //2do desempate: Prioridad
                qsort(tmp2,Segunda.conta,sizeof(Procesos),comparePRIO);
                int menor_PRIO = MENOR_PRIO(tmp2,Segunda.conta,Crono);
                Conta_Indice Tercer = Conta_Indice_PRIO(tmp2,Segunda.conta,menor_PRIO, Crono);
                //Proceso
                if (Tercer.conta == 1) {
                    int cronito = CalcularTodoPrimero(Insize,ListaOrigen,ListaFinal,tmp2,menor_PRIO,Crono,Segunda.conta,'P');
                    Insize--;
                    Contador--;
                    une = 2;
                    Crono = cronito;
                } else {
                Procesos *tmp3;
                dynamic_arr((void **)&tmp3,sizeof(Procesos),Tercer.conta);
                for (int i=0;i<Tercer.conta;i++) {
                    tmp3[i] = tmp2[Tercer.indices[i]];
                }
                //3er desempate: PID
                int menor_pid = MENOR_PID(tmp3,Tercer.conta,Crono);
                qsort(tmp3,Tercer.conta,sizeof(Procesos),comparePID);
                //Proceso
                int cronito = CalcularTodoPrimero(Insize,ListaOrigen,ListaFinal,tmp3,menor_pid,Crono,Tercer.conta,'I');
                Insize--;
                Contador--;
                une = 2;
                Crono = cronito;
                    }
                }
            }
        }
        //A partir del Segundo proceso
        if (une == 2) {
            qsort(ListaOrigen,Insize,sizeof(Procesos),compareLLEGADA);
            if (ListaOrigen[0].T_LLEGADA > Crono) {
                 Crono = ListaOrigen[0].T_LLEGADA;
            }
            qsort(ListaOrigen,Insize,sizeof(Procesos),compareCPU);
            int menor_CPU = MENOR_CPU(ListaOrigen,Insize,Crono);
            Conta_Indice Primera = Conta_Indice_CPU(ListaOrigen,Insize,menor_CPU, Crono);
            if (Primera.conta == 1) {
                int cronito = CalcularTodoPosterior(Insize,ListaOrigen,ListaFinal,ListaOrigen,menor_CPU,Crono,Insize,'C',repe);
                Insize--;
                repe++;
                Contador--;
                Crono = cronito;
                } else if (Primera.conta != 1) {
                Procesos *tmp1;
                dynamic_arr((void **)&tmp1,sizeof(Procesos),Primera.conta);
                for (int i=0;i<Primera.conta;i++) {
                    tmp1[i] = ListaOrigen[Primera.indices[i]];
                }
                //1er desempate: Ordenamiento por prioridad
                qsort(tmp1,Primera.conta,sizeof(Procesos),comparePRIO);
                int menor_prio = MENOR_PRIO(tmp1,Primera.conta,Crono);
                Conta_Indice Segunda = Conta_Indice_PRIO(tmp1,Primera.conta,menor_prio, Crono);
                if (Segunda.conta == 1) {
                    int cronito = CalcularTodoPosterior(Insize,ListaOrigen,ListaFinal,tmp1,menor_prio,Crono,Primera.conta,'P',repe);
                    Insize--;
                    repe++;
                    Contador--;
                    Crono = cronito;
                } else if (Segunda.conta != 1) {
                    Procesos *tmp2;
                    dynamic_arr((void **)&tmp2,sizeof(Procesos),Segunda.conta);
                    for (int i=0;i<Segunda.conta;i++) {
                        tmp2[i] = tmp1[Segunda.indices[i]];
                    }
                    //2do desempate: Ordenamiento por T_LLEGADA
                    qsort(tmp2,Segunda.conta,sizeof(Procesos),compareLLEGADA);
                    int menor_llegada = MENOR_LLEGADA(tmp2,Segunda.conta,Crono);
                    Conta_Indice Tercer = Conta_Indice_LLEGADA(tmp2,Segunda.conta,menor_llegada, Crono);
                    if (Tercer.conta == 1) {
                        int cronito = CalcularTodoPosterior(Insize,ListaOrigen,ListaFinal,tmp2,menor_llegada,Crono,Segunda.conta,'L',repe);
                        Insize--;
                        repe++;
                        Contador--;
                        Crono = cronito;
                    } else if (Tercer.conta != 1) {
                    Procesos *tmp3;
                    dynamic_arr((void **)&tmp3,sizeof(Procesos),Tercer.conta);
                    for (int i=0;i<Tercer.conta;i++) {
                        tmp3[i] = tmp2[Tercer.indices[i]];
                    }
                    qsort(tmp3,Tercer.conta,sizeof(Procesos),comparePID);
                    int menorPID = MENOR_PID(tmp3,Tercer.conta,Crono);
                    //Proceso
                    int cronito = CalcularTodoPosterior(Insize,ListaOrigen,ListaFinal,tmp3,menorPID,Crono,Tercer.conta,'I',repe);
                    Insize--;
                    repe++;
                    Contador--;
                    Crono = cronito;
                        }
                    }
                }
            }
        }
    //Cerrar archivo y salir
    double RPTA_SPN = Display(ListaFinal, archivo1);
    printf("CONCLUSION\n"); fprintf(archivo1,"CONCLUSION\n");
    float RPTA_SPNF;
    RPTA_SPNF = (float)RPTA_SPN;
    if (RPTA_SPNF < RPTA_SRT) {
    printf("El mejor algoritmo es SPN para este caso\n");
    fprintf(archivo1,"El mejor algoritmo es SPN para este caso\n");
    } else if(RPTA_SPNF == RPTA_SRT) {
    printf("Ambos algoritmos son eficientes para este caso\n");
    fprintf(archivo1,"Ambos algoritmos son eficientes para este caso\n");
    } else if(RPTA_SPNF > RPTA_SRT) {
    printf("El mejor algoritmo es SRT para este caso\n");
    fprintf(archivo1,"El mejor algoritmo es SRT para este caso\n");
    }
    fclose(archivo);
    fclose(archivo1);
    free(ListaOrigen);
    free(ListaFinal);
    return 0;
}
