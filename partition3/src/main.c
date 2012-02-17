/*
 * FILE: main.c
 */

#include <string.h>
#include <stdio.h>
#include <xm.h>
#include "../../src/regulador.h"

#define PORT_SIM "sim_r2" //Per a rebre la temperatura del simulador
#define PORT_PVC1 "pvc_w0"  //Envia la posició de la vàlbula d'eixida al linux
#define PORT_PVC3 "pvc_w3"  //Envia la posició de la vàlbula d'eixida al simulador
#define PORT_SIZE 16
#define XM_MSG_VALID 1

static int p_sim, p_pvc1, p_pvc3;
static int temp,pvc;
static Regulador r;

void actualizarPVC(){
	pvc = regular(&r,temp);
}

void PartitionMain(void){

	//Puertos de lectura
    if ((p_sim=XM_create_sampling_port(PORT_SIM, PORT_SIZE, XM_DESTINATION_PORT))<0) {
        printf("[ControlTemp] port [%s] cannot be created\n", PORT_SIM);
        return;
    }

    //Puertos de escritura
    if ((p_pvc1=XM_create_sampling_port(PORT_PVC1, PORT_SIZE, XM_SOURCE_PORT))<0) {
        printf("[Simulador] port [%s] cannot be created\n", PORT_PVC1);
        return;
    }
	if ((p_pvc3=XM_create_sampling_port(PORT_PVC3, PORT_SIZE, XM_SOURCE_PORT))<0) {
        printf("[Simulador] port [%s] cannot be created\n", PORT_PVC3);
        return;
    }

	float n_temp; 
	int flags;
    while (1) {
	
	//Llegim nou templ
    	XM_read_sampling_message(p_sim, &n_temp, sizeof(n_temp), &flags);
    	if (flags&XM_MSG_VALID){
        	if(temp != n_temp){
            		temp = n_temp;
			actualizarPVC();
        	}	
    	}
	
	//Enviem la nova pvc
    	XM_write_sampling_message(p_pvc1, &pvc, sizeof(pvc)); 
    	XM_write_sampling_message(p_pvc3, &pvc, sizeof(pvc)); 


        printf("[P%d] Soc el control de temp\n", XM_PARTITION_SELF);
		XM_idle_self();
	}
    
    XM_halt_partition(XM_PARTITION_SELF);
}
