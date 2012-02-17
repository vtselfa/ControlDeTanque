#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "xm-linux.h"
#include "funciones_entrada.h"
#include "defines.h"

#define PORT_SIM "sim_r0" //Envia simulació al linux
#define PORT_PVS "pvs_r0"  //LLig la posició de la vàlbula d'eixida
#define PORT_PVC "pvc_r0"  //Llig la posició de la vàlbula de control
#define PORT_E1 "e1_w"
#define PORT_E2 "e2_w"
#define PORT_E3 "e3_w"
#define PORT_SIZE 16
#define XM_MSG_VALID 1

static int p_sim, p_pvs, p_pvc, p_e1, p_e2, p_e3; //Ports


void iniciarProcesos(){	
	//Enviem el nou estat EJECUCION a les demés particions
	XM_write_sampling_message(p_e1, 0, sizeof(int)); 
//	XM_write_sampling_message(p_e2, 0, sizeof(int)); 
//	XM_write_sampling_message(p_e3, 0, sizeof(int)); 
}

void submenuRegulador(TipoRegulador t){
	char * tipo;
	if(t==NIVEL){
		tipo="nivel";
	}else{ 
		tipo="temperatura";
	}	
	
	while(1){	

		printf("Submenú del regulador de %s\n", tipo);
    	printf("1- Cambiar valor de referencia\n");
    	printf("2- Cambiar parámetros\n");
    	printf("3- Cambiar periodo(?¿)\n");
    	printf("4- Aplicar cambios\n");
    	printf("0- Volver\n");
    	printf("\n");

		int opcion=-1;
    	int r=-1;
		while(r<=0 || opcion<0 || opcion>5){
			printf("Elija una opción\n");
			r=leerEntero(&opcion);							
		}
			
		switch(opcion){
			case 0:
				return;

			case 1:
				//cambiarValorRef();
				break;

			case 2:
				//cambiarParametros();
				break;
		
			case 3:
				break;
		
			case 4:
				//aplicarCambios(tipo);
				break;
		}
	}
}

void menuPrincipal(){
	printf("\n");
	printf("MENU PRINCIPAL\n");
	printf("1- Arrancar\n");
	printf("2- Parar procesos\n");
	printf("3- Mostrar/modificar regulador de nivel\n");
	printf("4- Mostrar/modificar regulador de temperatura\n");
	printf("5- Cambiar el periodo del monitor (?¿)\n");
	printf("0- Salir\n");
	printf("\n");
	
	int opcion=-1;
    int r=-1;
    while(r<=0 || opcion<0 || opcion>5){
		printf("Elija una opción\n");
        r=leerEntero(&opcion);
    }
    
	switch(opcion){
        case 0:
            exit(0);
			break;
        case 1:
            iniciarProcesos();
			break;
        case 2:
            submenuRegulador(0);
			break;
        case 3:
            submenuRegulador(1);
			break;
        case 4:
            //modificarPeriodo();
			break;
	}
}

int main(int argc, char *argv[]){
	init_libxm(0,0);	
	
	//Puertos de lectura
    if ((p_sim=XM_create_sampling_port(PORT_SIM, PORT_SIZE, XM_DESTINATION_PORT))<0) {
        printf("[ControlNivel] ort [%s] cannot be created\n", PORT_SIM);
        return -1;
    }
	if ((p_pvs=XM_create_sampling_port(PORT_PVS, PORT_SIZE, XM_DESTINATION_PORT))<0) {
        printf("[ControlNivel] port [%s] cannot be created\n", PORT_PVS);
        return -1;
    }
	if ((p_pvc=XM_create_sampling_port(PORT_PVC, PORT_SIZE, XM_DESTINATION_PORT))<0) {
        printf("[ControlNivel] port [%s] cannot be created\n", PORT_PVC);
        return -1;
    }


	//Puertos de escritura
	if ((p_e1=XM_create_sampling_port(PORT_E2, PORT_SIZE, XM_SOURCE_PORT))<0) {
        printf("[ControlNivel] port [%s] cannot be created\n", PORT_E1);
        return -1;
    } 
	if ((p_e2=XM_create_sampling_port(PORT_E2, PORT_SIZE, XM_SOURCE_PORT))<0) {
        printf("[ControlNivel] port [%s] cannot be created\n", PORT_E2);
        return -1;
    } 
	if  ((p_e3=XM_create_sampling_port(PORT_E3, PORT_SIZE, XM_SOURCE_PORT))<0) {
        printf("[ControlNivel] port [%s] cannot be created\n", PORT_E3);
        return -1;
    }

	
	while(1){
		menuPrincipal();
	}


    //int partitionId;
    //int i;
    //int delay;

    //if (argc < 2) {
     //   printf("usage: part_manager partitionId\n");
      //  return -1;
    //}

    //init_libxm(0,0);

    //partitionId = atoi(argv[1]);
    //if (partitionId == 0) {
     //   printf("part_manager: error: invalid partitionId\n");
      //  return -1;
    //}
   // printf("Partition manager running on partition %d\n", XM_PARTITION_SELF);

    //delay = 10;
    //printf("waiting %d seconds to suspend partition %d:", delay, partitionId);
    //for (i = 0; i < delay; i++) {
     //   printf(".");
      //  fflush(stdout);
       // sleep(1);
    //}
    //XM_suspend_partition(partitionId);
    //printf(" done\n");

    //delay = 10;
    //printf("waiting %d seconds to resume partition %d:", delay, partitionId);
    //for (i = 0; i < delay; i++) {
     //   printf(".");
      //  fflush(stdout);
       // sleep(1);
    //}
    //XM_resume_partition(partitionId);
    //printf(" done\n");

    //delay = 20;
    //printf("waiting %d seconds to halt partition %d:", delay, partitionId);
    //for (i = 0; i < delay; i++) {
     //   printf(".");
      //  fflush(stdout);
       // sleep(1);
   // }
    //XM_halt_partition(partitionId);
    //printf(" done\n");

    //delay = 20;
    //printf("waiting %d seconds to reset partition %d:", delay, partitionId);
    //for (i = 0; i < delay; i++) {
      //  printf(".");
     //   fflush(stdout);
    //    sleep(1);
   // }

   // XM_reset_partition(partitionId, XM_COLD_RESET, 0);
   // printf(" done\n");

    return 0;
}
