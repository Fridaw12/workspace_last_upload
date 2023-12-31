/*
 * structures_def.h
 *
 *  Created on: Mar 16, 2023
 *      Author: fridawagner
 *
 *  Lage noe "makrolignende" for å kunne henvise til gyrostruct med ekstern
 *  ikke en definisjon
 *
 *
 */

#ifndef INC_TYPEDEF_STRUCTURES_H_
#define INC_TYPEDEF_STRUCTURES_H_

#include <main.h>
/// NB PLASSERING HØYEST OPP I TYPEDEF LAVEST ADRESSE

////// FOR THRUSTER

typedef struct {
    float max_neg_retn;
} motorkarakteristikk_parametre;


typedef struct{
    float a;
    float b;
}lin_modell;

typedef struct {
	//grunnadresse intialisert først
	float hhf;
	float hhb;
	float hvb;
	float hvf;
	float vhf;
	float vhb;
	float vvb;
	float vvf;
} thruster_struct;

typedef struct {
    int8_t hhf;
    int8_t hhb;
    int8_t hvb;
    int8_t hvf;
    int8_t vhf;
    int8_t vhb;
    int8_t vvb;
    int8_t vvf;
} thruster_struct_int8;


typedef struct {
    int16_t hhf;
    int16_t hhb;
    int16_t hvb;
    int16_t hvf;
    int16_t vhf;
    int16_t vhb;
    int16_t vvb;
    int16_t vvf;
} thruster_struct_int16;

// FOR KALKULASJONER PÅ UC, TRENGER FLOAT
typedef struct {
	float jag; // 0-100 min-maks på thusterene -> må oversettes til u singal
	float svai;
	float gir;
	float hiv;
	float tilte;

    // andre funksjonaliteter
	//float throttling; //?

} id_thrust_struct;


// FOR MELDING (..)
typedef struct {
	int8_t jag; // 0-100 min-maks på thusterene -> må oversettes til u singal
	int8_t svai;
	int8_t gir;
	int8_t hiv;
	int8_t tilte;

    // andre funksjonaliteter
	// int8_t throttling; //?
} id_thrust_struct_int;

////////FOR REGULERING

typedef struct{
    float Ts;
    float a;

}PID_var_felles;

typedef struct{
    float Kp; //1
    float Ki; //2
    float Kd; //3
	float Ref;
    float u;  //4
    float ui_1;
    float e;
    float e_1;
    float Y;
    float YF;
    float YF_1;
    float uimaks;
    float uimin;
    float umaks;
    float umin;
}PID_var;


//////// FOR SENSOR

// FOR MELDING (..)

typedef struct {
	float rull;	// [roll] deg/100
	float stamp;	// [] deg/100
	float dybde;    // cm/1
} id_sensor_struct ;

typedef struct {
	int16_t rull;	// [roll] deg/100
	int16_t stamp;	// [] deg/100
	int16_t dybde;
} id_sensor_struct_int ;

/////// FOR MANIPULATOR

typedef struct {
    uint16_t gripe;
    uint16_t snurr;
    uint16_t sving;
    uint16_t skyv;
} manpu_struct_uint16 ;

typedef struct {
    float gripe;
    float snurr;
    float sving;
    float skyv;

    //float sensitivitet;
    // endre_frekvens;
} id_manpu_struct ;

// FOR MELDING 41

typedef struct {
    int8_t gripe;
    int8_t snurr;
    int8_t sving;
    int8_t skyv;

    //int8_t sensitivitet;  // sensitivitet*10
} id_manpu_struct_int ;

// KRAFT
// FOR MELDING 36
typedef struct {
    float stromforbruk_hoyre;
    float stromforbruk_venstre;
    float EFFEKT_forbruk_hoyre;
    float EFFEKT_forbruk_venstre;
    float EFFEKT_begrensing;
} id_kraft_struct ;

typedef struct {
    int16_t stromforbruk_hoyre; // dele på 1000 ved mottak, resultat i A [A/1000]
    int16_t stromforbruk_venstre;
} id_kraft_struct_int ;

typedef struct {
    float horisontal;
    float vertikal;
} demping_effekt;

///// OPPDATERING AV VARIABLER
typedef struct {
	uint32_t param_id;
	float parameter;
} can_param;



#endif /* INC_TYPEDEF_STRUCTURES_H_ */
