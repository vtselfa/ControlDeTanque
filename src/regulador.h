#ifndef REGULADOR_H
#define REGULADOR_H

#include <stdio.h>

typedef enum{PROPORCIONAL, TODO_NADA} Clase;
typedef enum{P, N} Accion;

typedef struct {
	Clase clase;
	Accion accion;
	float Ref; //Valor de referència
	float Kp,Ki,Kd;
	float Vmax;
	float Vmin;
	float Hmax;
	float Hmin;
	float Tm;
	float Sal;
	float Sal_Ant;
	float Ent_Ant;
}Regulador;

float segunAccion(Accion a){
	if(a==P){
		return -1;
	}
	else{
		return 1;
	}

}

float regular(Regulador *r, float entrada){
	float Error=0, Error_Ant=0, Aprop=0, Aint=0, Ader=0;

	Error_Ant = Error;
	Error = r->Ref-entrada;

	if(r->clase == PROPORCIONAL){
		Aprop = r->Kp*Error;
		if(r->Ki>0)
			Aint = Error * r->Ki * r->Tm;
		if(r->Kd >0)
			Ader = (Error-(2*Error_Ant)) * r->Kd / (r->Tm /1000);
		r->Sal = r->Sal_Ant + (Aprop + Aint + Ader) * segunAccion(r->accion);
	}
	
	else if(r->clase==TODO_NADA){
		if(Error > r->Hmax)
			if(r->accion==P)
				r->Sal = r->Vmin;
			else
				r->Sal = r->Vmax;
		
		else if(Error < r->Hmin)
            if(r->accion==P)
                r->Sal = r->Vmax;
            else
                r->Sal = r->Vmin;

		else{
			r->Sal=r->Sal_Ant;
		}
	}

	//Redondeo
	if(r->Sal > 1)
		r->Sal = 1;
	else if(r->Sal < 1)
		r->Sal = 0;
	
	r->Ent_Ant = entrada;
	r->Sal_Ant = r->Sal;
	
	return r->Sal;
}


#endif
