/*
 * FILE: main.c
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h> //Totalment inutil
#include <xm.h>
#include "utils.h"

#define PORT_SIM0 "sim_w0" //Envia simulació al linux
#define PORT_SIM1 "sim_w1" //Envia simulació al control de nivell
#define PORT_SIM2 "sim_w2" //Envia simulació al control de temp
#define PORT_PVS "pvs_r3"  //LLig la posició de la vàlbula d'eixida
#define PORT_PVC "pvc_r3"  //Llig la posició de la vàlbula de control
#define PORT_SIZE 16
#define XM_MSG_VALID 1



//BASURA PER A QUE COMPILE
int rand(){return 3;}
void srand(int seed){}
int time(int* crap){return 0;}
//////////////////////////////



typedef struct Simulacion {
    float temp;
    float nivel;
} Simulacion;

static float pvs, pvc;
static int p_pvs, p_pvc, p_sim0, p_sim1, p_sim2; //Puertos

int periodo = 20; //El mateix que s'ha definit al plan

float Qtotal=0; //Caudal net (entrades-eixides)
float Qemedio=0; //Caudal mitja d'entrada per la valbula principal
float Qctmax=0; //Caudal màxim d'entrada per la valbula de control de temp

float Te=15, Tc=40; //Temperatures d'entrada del líquid

float cte1=1, cte2=1, cte3=1; //Constants del dipòsit 

int aleatorio(int ini, int fin){
    // Devuelve un entero pseudoaleatório € [ini, fin]
    
    srand(time(NULL));
    if(ini > fin){
        int tmp = ini;
        ini=fin;
        fin=tmp;
    }
    if (ini == fin)
        return ini;
    else
        return rand()%(-ini+fin+1)+ini;
}

void simular(Simulacion *s){
    float Qe = Qemedio + (float)aleatorio(-2,2) / 5;
    float Qct = Qctmax * pvc;
    float Qs = cte3 * sqrt(s->nivel) * pvs;
    
    Qe = Qe * periodo / 1000;
    Qct = Qct * periodo / 1000;
    Qs = Qs * periodo / 1000;
    
    float f1 = Qe * Te + Qct * Tc + (Qtotal - Qs) * s->temp;
    float f2 = Qe + Qct + Qtotal - Qs;
    
	s->temp = cte2 * f1 / f2;
    
    Qtotal = Qtotal - Qs + Qe + Qct;
    
    s->nivel = s->nivel + (Qe + Qct -Qs) * cte1;
}

void enviar_simulacion(Simulacion *s){
    int w;
    w=XM_write_sampling_message(p_sim0, s, sizeof(Simulacion)); //Al linux li ho enviem tot
    w=XM_write_sampling_message(p_sim1, &s->nivel, sizeof(s->nivel)); //A aquest sols el nivell
    w=XM_write_sampling_message(p_sim2, &s->temp, sizeof(s->temp)); //I aquest sols la temperatura
    printf("[Simulador] written: %d temp=%f nivel=%f\n", w, s->temp, s->nivel);
}

void ExtIrqHandler(int irqnr) {
    int flags;
    float n_pvs;
    XM_read_sampling_message(p_pvs, &n_pvs, sizeof(n_pvs), &flags);
    if (flags&XM_MSG_VALID){
		if(pvs != n_pvs){
        	printf("[Simulador] pvs =  %f\n", n_pvs);
        	pvs = n_pvs;
        }   
	}

    //Recibido estado valvula control
	float n_pvc;
	XM_read_sampling_message(p_pvc, &n_pvc, sizeof(n_pvc), &flags);
	if (flags&XM_MSG_VALID){
		if(pvc != n_pvc){
        	printf("[Simulador] pvc =  %f\n", n_pvc);
           	pvc = n_pvc;
        }   
	}
    XM_unmask_irq(XM_VT_EXT_OBJDESC);
}

void PartitionMain(void){
    
    //Puertos de lectura
    if ((p_pvs=XM_create_sampling_port(PORT_PVS, PORT_SIZE, XM_DESTINATION_PORT))<0) {
	    printf("[Simulador] port [%s] cannot be created\n", PORT_PVS);
	    return;
    }
    if ((p_pvc=XM_create_sampling_port(PORT_PVC, PORT_SIZE, XM_DESTINATION_PORT))<0) {
	    printf("[Simulador] port [%s] cannot be created\n", PORT_PVC);
	    return;
    }
    
    //Puertos de escritura
    if ((p_sim0=XM_create_sampling_port(PORT_SIM0, PORT_SIZE, XM_SOURCE_PORT))<0) {
	    printf("[Simulador] port [%s] cannot be created\n", PORT_SIM0);
	    return;
    }
    if ((p_sim1=XM_create_sampling_port(PORT_SIM1, PORT_SIZE, XM_SOURCE_PORT))<0) {
	    printf("[Simulador] port [%s] cannot be created\n", PORT_SIM1);
	    return;
    }
    if ((p_sim2=XM_create_sampling_port(PORT_SIM2, PORT_SIZE, XM_SOURCE_PORT))<0) {
	    printf("[Simulador] port [%s] cannot be created\n", PORT_SIM2);
	    return;
    }


    XM_enable_irqs();
    XM_unmask_irq(XM_VT_EXT_OBJDESC);
    
    Simulacion s;
	while (1) {
		//Esperamos a recibir algo
		XM_idle_self();
        
		//Actualizamos la simulacion y enviamos
		simular(&s);
		enviar_simulacion(&s);
    }
}
