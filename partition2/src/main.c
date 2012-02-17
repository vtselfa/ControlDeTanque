/*
 * FILE: main.c
 */

#include <string.h>
#include <stdio.h>
#include <xm.h>
#include "../../src/regulador.h"
#include "../../src/defines.h"

#define PORT_SIM "sim_r1" //Envia simulació al control de nivell
#define PORT_PVS1 "pvs_w0"  //Envia la posició de la vàlbula d'eixida al linux
#define PORT_PVS3 "pvs_w3"  //Envia la posició de la vàlbula d'eixida al simulador
#define PORT_E "e1_r"  //Llig les ordres enviades desde el linux
#define PORT_SIZE 16
#define XM_MSG_VALID 1

static int p_sim, p_pvs1, p_pvs3, p_e;
static int nivel,pvs;
static Regulador r;
static Estado estado = PARADO;

void actualizarPVS(){
	pvs = regular(&r,nivel);
}

void PartitionMain(void){

	//Puertos de lectura
    if ((p_sim=XM_create_sampling_port(PORT_SIM, PORT_SIZE, XM_DESTINATION_PORT))<0) {
        printf("[ControlNivel] port [%s] cannot be created\n", PORT_SIM);
        return;
    }
    if ((p_e=XM_create_sampling_port(PORT_E, PORT_SIZE, XM_DESTINATION_PORT))<0) {
        printf("[ControlNivel] port [%s] cannot be created\n", PORT_E);
        return;
    }

    //Puertos de escritura
    if ((p_pvs1=XM_create_sampling_port(PORT_PVS1, PORT_SIZE, XM_SOURCE_PORT))<0) {
        printf("[Simulador] port [%s] cannot be created\n", PORT_PVS1);
        return;
    }
	if ((p_pvs3=XM_create_sampling_port(PORT_PVS3, PORT_SIZE, XM_SOURCE_PORT))<0) {
        printf("[Simulador] port [%s] cannot be created\n", PORT_PVS3);
        return;
    }

	float n_nivel; 
	Estado n_e; 
	int flags;
    while (1) {
		//Anem llegint estats mentre estiga parat
		do{
    		XM_read_sampling_message(p_e, &n_e, sizeof(n_e), &flags);
    		if (flags&XM_MSG_VALID){
        		if(estado != n_e) estado = n_e;
			}else{
				XM_idle_self();
			}
	        printf("[P%d] Soc el control de nivell i estic PARAT\n", XM_PARTITION_SELF);
		} while(estado==PARADO);
		
		//Si es rep ordre de acabar, parem la partició
		if(estado==TERMINADO) XM_halt_partition(XM_PARTITION_SELF);

	
		//Llegim nou nivell
    	XM_read_sampling_message(p_sim, &n_nivel, sizeof(n_nivel), &flags);
    	if (flags&XM_MSG_VALID){
        	if(nivel != n_nivel){
            	nivel = n_nivel;
				actualizarPVS();
        	}	
    	}
	
		//Enviem la nova pvs
    	XM_write_sampling_message(p_pvs1, &pvs, sizeof(pvs)); 
    	XM_write_sampling_message(p_pvs3, &pvs, sizeof(pvs)); 


        printf("[P%d] Soc el control de nivell\n", XM_PARTITION_SELF);
		XM_idle_self();
	}
    
    XM_halt_partition(XM_PARTITION_SELF);
}
