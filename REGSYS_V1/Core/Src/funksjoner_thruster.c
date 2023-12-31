/*
 * funksjoner_thruster.c
 *
 *  Created on: Mar 25, 2023
 *      Author: fridawagner
 */

#include "main.h"
#include <typedef_structures.h>
#include <globvariabler_dek.h>
#include "math.h"


/////////////////////////// THUSTER-STRYING

void null_bidrag_horisontal(thruster_struct *bidrag){
    bidrag->hhb = 0;
    bidrag->hhf = 0;
    bidrag->hvb = 0;
    bidrag->hvf = 0;
}

void null_bidrag_vertikal(thruster_struct *bidrag){
    bidrag->vhb = 0;
    bidrag->vhf = 0;
    bidrag->vvb = 0;
    bidrag->vvf = 0;
}

//// sette alle regvar-bidrag for prosjekt til 0.
void null_bidrag_alle_regvar(void){
    null_bidrag_horisontal(&reguleringbidrag_hiv);
    null_bidrag_vertikal(&reguleringbidrag_hiv);
    null_bidrag_horisontal(&reguleringbidrag_stamp);
    null_bidrag_vertikal(&reguleringbidrag_stamp);
    null_bidrag_horisontal(&reguleringbidrag_rull);
    null_bidrag_vertikal(&reguleringbidrag_rull);
}

void beregning_operatorbidrag(void){
	operator_referanse.jag = (float) operator_referanse_int.jag;
	operator_referanse.svai = (float) operator_referanse_int.svai;
	operator_referanse.gir = (float) operator_referanse_int.gir;
	operator_referanse.hiv = (float) operator_referanse_int.hiv;

	static uint32_t teller_til_aktiver_reg_hiv = 0;

	// dersom er jag eller svai i melding
	if (operator_referanse.jag || operator_referanse.svai){
        float jag = operator_referanse.jag * thrust_sensitivitet.jag;
        float svai = operator_referanse.svai * thrust_sensitivitet.svai;

        operatorbidrag.hhf = (jag - svai) / sqrtf(2);
        operatorbidrag.hvf = (jag + svai) / sqrtf(2);
        operatorbidrag.hhb = - operatorbidrag.hvf;
        operatorbidrag.hvb = - operatorbidrag.hhf;
    }
    else{
    	// må sette til null hvis melding tom, hvis ikke fortsetter å skrive
    	// forrige verdi
    	operatorbidrag.hhf = 0;
    	operatorbidrag.hvf = 0;
    	operatorbidrag.hhb = 0;
    	operatorbidrag.hvb = 0;
    }

    // dersom er gir i melding
    if (operator_referanse.gir){
        // vil ha gir-rotasjon-krefter vesentlig svakere enn de andre
        float gir = operator_referanse.gir * thrust_sensitivitet.gir;
        operatorbidrag.hhf -= gir;
        operatorbidrag.hhb += gir;
        operatorbidrag.hvb -= gir;
        operatorbidrag.hvf += gir;
    }

    // Bidrag til vertikale thrustere i tilfelle hiv
    if (operator_referanse.hiv){

    	// settte flag for reg av hiv til 0
    	REG_AKTIVER_DYBDE = REG_AKTIVER_DYBDE & (~0x08);

    	teller_til_aktiver_reg_hiv = 0;

        float hiv = operator_referanse.hiv * thrust_sensitivitet.hiv;
        operatorbidrag.vhf = hiv;
        operatorbidrag.vhb = hiv;
        operatorbidrag.vvb = hiv;
        operatorbidrag.vvf = hiv;
    }
    else{ // dersom hiv = 0;
    	// går gjennom funksjon 6 ganger
    	if(teller_til_aktiver_reg_hiv == 6){
    		// dersom bit 3 i reg aktiver fra topside er 1
    		if (REG_AKTIVER & (0x08)){
    			// sett reg_aktiver for dybde
    			REG_AKTIVER_DYBDE = REG_AKTIVER_DYBDE | (0x08);
    			// sett referansen
    			PID_hiv.Ref = sensordata.dybde;
    		}
    		else {
    			// hvis ikke reg_aktiver fra topside er 1, sett reg aktiver for dybde til 0
    			REG_AKTIVER_DYBDE = REG_AKTIVER_DYBDE & (~0x08);
    		}
    	}
    	// teller for aktivering av hiv regulering
    	teller_til_aktiver_reg_hiv++;
    	if(teller_til_aktiver_reg_hiv>100){
    		teller_til_aktiver_reg_hiv = 10;
    	}

    	operatorbidrag.vhf = 0;
        operatorbidrag.vhb = 0;
        operatorbidrag.vvb = 0;
        operatorbidrag.vvf = 0;
    }
}

//    if(!(REG_AKTIVER & 0x04)){
//    		null_PID_var(&PID_stamp);
   // uint16_t var_shift = 0x0000000000000000;

//	if (operatorbidrag.hhf){
//		var_shift = var_shift | HF;
//	}

//	if (operatorbidrag.hhb){
//		var_shift = var_shift | HB;
//	}
//
//	if (operatorbidrag.hvb){
//		var_shift = var_shift | VB;
//	}
//
//	if (operatorbidrag.hvf){
//		var_shift = var_shift | VF;
//	}

	//shiftUT(var_shift);

void beregn_sum_bidrag(void) {
    // horisontal
    sum_bidrag.hhf =
            operatorbidrag.hhf + reguleringbidrag_hiv.hhf + reguleringbidrag_stamp.hhf + reguleringbidrag_rull.hhf;
    sum_bidrag.hhb =
            operatorbidrag.hhb + reguleringbidrag_hiv.hhb + reguleringbidrag_stamp.hhb + reguleringbidrag_rull.hhb;
    sum_bidrag.hvf =
            operatorbidrag.hvf + reguleringbidrag_hiv.hvf + reguleringbidrag_stamp.hvf + reguleringbidrag_rull.hvf;
    sum_bidrag.hvb =
            operatorbidrag.hvb + reguleringbidrag_hiv.hvb + reguleringbidrag_stamp.hvb + reguleringbidrag_rull.hvb;
    // vertikal
    sum_bidrag.vhf =
            operatorbidrag.vhf + reguleringbidrag_hiv.vhf + reguleringbidrag_stamp.vhf + reguleringbidrag_rull.vhf;
    sum_bidrag.vhb =
            operatorbidrag.vhb + reguleringbidrag_hiv.vhb + reguleringbidrag_stamp.vhb + reguleringbidrag_rull.vhb;
    sum_bidrag.vvf =
            operatorbidrag.vvf + reguleringbidrag_hiv.vvf + reguleringbidrag_stamp.vvf + reguleringbidrag_rull.vvf;
    sum_bidrag.vvb =
            operatorbidrag.vvb + reguleringbidrag_hiv.vvb + reguleringbidrag_stamp.vvb + reguleringbidrag_rull.vvb;

}

void skalere_ned_saturation_neg(float grense_variabel) {
    grense_variabel = grense_variabel;
    float over_grense[8] = {0, 0, 0, 0, 0, 0, 0, 0};

    if (sum_bidrag.hhf < grense_variabel) { over_grense[0] = sum_bidrag.hhf; }
    if (sum_bidrag.hhb < grense_variabel) { over_grense[1] = sum_bidrag.hhb; }
    if (sum_bidrag.hvf < grense_variabel) { over_grense[2] = sum_bidrag.hvf; }
    if (sum_bidrag.hvb < grense_variabel) { over_grense[3] = sum_bidrag.hvb; }

    if (sum_bidrag.vhf < grense_variabel) { over_grense[4] = sum_bidrag.vhf; }
    if (sum_bidrag.vhb < grense_variabel) { over_grense[5] = sum_bidrag.vhb; }
    if (sum_bidrag.vvf < grense_variabel) { over_grense[6] = sum_bidrag.vvf; }
    if (sum_bidrag.vvb < grense_variabel) { over_grense[7] = sum_bidrag.vvb;}

    //Calculate length of array arr
    // int length = sizeof(over_grense) / sizeof(over_grense[0]);
    int length = 8;

    //Initialize min with first element of array.
    float mest_neg = over_grense[0];

    //Loop through the array
    for (int i = 0; i < length; i++) {
        //Compare elements of array with min
        if (over_grense[i] < mest_neg)
            mest_neg = over_grense[i];
    }

    float skalerings_variabel = grense_variabel/mest_neg;

    sum_bidrag.hhf *= skalerings_variabel;
    sum_bidrag.hhb *= skalerings_variabel;
    sum_bidrag.hvf *= skalerings_variabel;
    sum_bidrag.hvb *= skalerings_variabel;

    sum_bidrag.vhf *= skalerings_variabel;
    sum_bidrag.vhb *= skalerings_variabel;
    sum_bidrag.vvf *= skalerings_variabel;
    sum_bidrag.vvb *= skalerings_variabel;

}

void skalere_ned_saturation_pos(float grense_variabel) {
    grense_variabel = fabsf(grense_variabel);
    float over_grense[8] = {0, 0, 0, 0, 0, 0, 0, 0};

    if (fabsf(sum_bidrag.hhf) > grense_variabel) { over_grense[0] = fabsf(sum_bidrag.hhf); }
    if (fabsf(sum_bidrag.hhb) > grense_variabel) { over_grense[1] = fabsf(sum_bidrag.hhb); }
    if (fabsf(sum_bidrag.hvf) > grense_variabel) { over_grense[2] = fabsf(sum_bidrag.hvf) ; }
    if (fabsf(sum_bidrag.hvb) > grense_variabel) { over_grense[3] = fabsf(sum_bidrag.hvb); }

    if (fabsf(sum_bidrag.vhf) > grense_variabel) { over_grense[4] = fabsf(sum_bidrag.vhf) ; }
    if (fabsf(sum_bidrag.vhb) > grense_variabel) { over_grense[5] = fabsf(sum_bidrag.vhb); }
    if (fabsf(sum_bidrag.vvf) > grense_variabel) { over_grense[6] = fabsf(sum_bidrag.vvf) ; }
    if (fabsf(sum_bidrag.vvb) > grense_variabel) { over_grense[7] = fabsf(sum_bidrag.vvb);}

    //Calculate length of array arr
    // int length = sizeof(over_grense) / sizeof(over_grense[0]);
    // bedre med fast verdi her, slipper dele operasjon.
    int length = 8;

    //Initialize min with first element of array.
    float hoyest = over_grense[0];

    //Loop through the array
    for (int i = 0; i < length; i++) {
        //Compare elements of array with min
        if (over_grense[i] > hoyest)
            hoyest = over_grense[i];
    }

    float skalerings_variabel = grense_variabel/hoyest;

    sum_bidrag.hhf *= skalerings_variabel;
    sum_bidrag.hhb *= skalerings_variabel;
    sum_bidrag.hvf *= skalerings_variabel;
    sum_bidrag.hvb *= skalerings_variabel;

    sum_bidrag.vhf *= skalerings_variabel;
    sum_bidrag.vhb *= skalerings_variabel;
    sum_bidrag.vvf *= skalerings_variabel;
    sum_bidrag.vvb *= skalerings_variabel;

}

// denne trengs ikke, utdatert

uint8_t sjekk_saturation_minus(void) {
    // Sjekk om sum er mer negativ enn feks -70 som er maks i negativ retning relativt til positiv.
    // dersom tilfellet -> trenger da å skalere ned
    if ( (sum_bidrag.hhf < motorkar.max_neg_retn) || (sum_bidrag.hhb < motorkar.max_neg_retn) || \
         (sum_bidrag.hvf < motorkar.max_neg_retn) || (sum_bidrag.hvb < motorkar.max_neg_retn) || \
         (sum_bidrag.vhf < motorkar.max_neg_retn) || (sum_bidrag.vhb < motorkar.max_neg_retn) || \
         (sum_bidrag.vvf < motorkar.max_neg_retn) || (sum_bidrag.vvb < motorkar.max_neg_retn)) {
            return 1;
        }
    return 0;
}


int8_t sjekk_saturation_pos(void){
    // sjekk om sum større enn 100 -> trenger da å skalere ned

    if ( (fabsf(sum_bidrag.hhf) > 100) || (fabsf(sum_bidrag.hhb) > 100) || \
         (fabsf(sum_bidrag.hvf) > 100) || (fabsf(sum_bidrag.hvb) > 100) || \
         (fabsf(sum_bidrag.vhf) > 100) || (fabsf(sum_bidrag.vhb) > 100) || \
         (fabsf(sum_bidrag.vvf) > 100) || (fabsf(sum_bidrag.vvb) > 100)) {
        return 1;
    }
    return 0;
}

void horisontal_demping(void){
	// first order lowpass
	// y[k] = a*y[k-1] + (1-a)*x[k]
	float a =  lowpass_thruster.a;
    sum_bidrag.hhf = (1-a) *sum_bidrag_forrige1.hhf +   a*sum_bidrag.hhf;
    sum_bidrag.hhb = (1-a) *sum_bidrag_forrige1.hhb +   a*sum_bidrag.hhb ;
    sum_bidrag.hvf = (1-a) *sum_bidrag_forrige1.hvf +   a*sum_bidrag.hvf;
    sum_bidrag.hvb = (1-a) *sum_bidrag_forrige1.hvb +   a*sum_bidrag.hvb;
}

void vertikal_demping(void){
	float a =  lowpass_thruster.a;
    sum_bidrag.vhf = (1-a) *sum_bidrag_forrige1.vhf +   a*sum_bidrag.vhf;
    sum_bidrag.vhb = (1-a) *sum_bidrag_forrige1.vhb +   a*sum_bidrag.vhb ;
    sum_bidrag.vvf = (1-a) *sum_bidrag_forrige1.vvf +   a*sum_bidrag.vvf;
    sum_bidrag.vvb = (1-a) *sum_bidrag_forrige1.vvb +   a*sum_bidrag.vvb;
}

int differanse_sjekk(float var_forrige, float var_naa,float begrensing) {
    // dersom forskjellig fortegn sett til 0 slik at motor får være av til en runde
    if (((var_naa - var_forrige) >= begrensing) || ((var_forrige - var_naa) >= begrensing)) {
        return 1;
    }
    return 0;
}

int horisontal_differanse(void){
    if (differanse_sjekk(sum_bidrag_forrige1.hhf,sum_bidrag.hhf,BEGRENSING)){
        return 1;
    }
    if (differanse_sjekk(sum_bidrag_forrige1.hhb,sum_bidrag.hhb,BEGRENSING)){
        return 1;
    }
    if (differanse_sjekk(sum_bidrag_forrige1.hvf,sum_bidrag.hvf,BEGRENSING)){
        return 1;
    }
    if (differanse_sjekk(sum_bidrag_forrige1.hvb,sum_bidrag.hvb,BEGRENSING)){
        return 1;
    }
    return 0;
}

int vertikal_differanse(void){
    if (differanse_sjekk(sum_bidrag_forrige1.vhf,sum_bidrag.vhf,BEGRENSING)){
        return 1;
    }
    if (differanse_sjekk(sum_bidrag_forrige1.vhb,sum_bidrag.vhb,BEGRENSING)){
        return 1;
    }
    if (differanse_sjekk(sum_bidrag_forrige1.vvf,sum_bidrag.vvf,BEGRENSING)){
        return 1;
    }
    if (differanse_sjekk(sum_bidrag_forrige1.vvb,sum_bidrag.vvb,BEGRENSING)){
        return 1;
    }
    return 0;
}


void effekt_begrensing(void){
	/// FORSKJELLIG DEMPING PÅ HORISONTAL OG VERTIKAL -> HVOR VIL LA EFFEKTEN VÆRE IGJEN?
    sum_bidrag.hhb = sum_bidrag.hhb*d_effekt.horisontal;
    sum_bidrag.hhf = sum_bidrag.hhf*d_effekt.horisontal;
    sum_bidrag.hvb = sum_bidrag.hvb*d_effekt.horisontal;
    sum_bidrag.hvf = sum_bidrag.hvf*d_effekt.horisontal;
    sum_bidrag.vhb = sum_bidrag.vhb*d_effekt.vertikal;
    sum_bidrag.vhf = sum_bidrag.vhf*d_effekt.vertikal;
    sum_bidrag.vvb = sum_bidrag.vvb*d_effekt.vertikal;
    sum_bidrag.vvf = sum_bidrag.vvf*d_effekt.vertikal;
}


int effekt_sjekk(void){
	kraftdata.EFFEKT_forbruk_hoyre = 	(float) (kraftdata_int.stromforbruk_hoyre/1000) *12.0;
	kraftdata.EFFEKT_forbruk_venstre = (float) (kraftdata_int.stromforbruk_venstre/1000) *12.0;

    if (kraftdata.EFFEKT_forbruk_hoyre>kraftdata.EFFEKT_begrensing){
        return 1;
    }
    else if (kraftdata.EFFEKT_forbruk_venstre>kraftdata.EFFEKT_begrensing){
    	return 1;
    }
    return 0;
}

/// NB MÅ GJØRE DENNE OM TIL MOTSATT PGA COUNTERCLOCKWISE THRUSTERE, IFJORD
/// HADDE DE CLOCKWISE
//int16_t konverter_paadrag_PWM(float paadrag){
//    if(paadrag<0) {return (int16_t) (neg_paa_PWM.a*paadrag + neg_paa_PWM.b);}
//    else if(paadrag>0) {return (int16_t) (pos_paa_PWM.a*paadrag + pos_paa_PWM.b);}
//    else {return (int16_t) 1499;}
//}

// resultat her er at med mindre fullstendig 0, vil motorene for reguleringen være litt på
// -> burde pådrag her være over 1 eller under 1 for at skal regne på skalaen?



int16_t konverter_paadrag_PWM(float paadrag){
    if(paadrag>0) {return (int16_t) (neg_paa_PWM.a*(-paadrag) + neg_paa_PWM.b);}
    else if(paadrag<0) {return (int16_t) (pos_paa_PWM.a*(-paadrag) + pos_paa_PWM.b);}
    else {return (int16_t) 1500;}
}



// dobbelsjekke om større eller mindre enn 100 -> saturering?

//int16_t konverter_paadrag_PWM(float paadrag) {
//    //printf("KOVERTERER PAADRAG TIL PWM\n");
//    if (paadrag < 0) {
//        if (paadrag < -100) {
//            return 1000;
//        } else {
//            return (int16_t) (neg_paa_PWM.a * paadrag + neg_paa_PWM.b);
//        }
//    }
//    else if (paadrag > 0){
//        if (paadrag > 100) {
//            return 1933;
//        }
//        else{
//            return (int16_t) (pos_paa_PWM.a * paadrag + pos_paa_PWM.b);
//        }
//    }
//    else
//    {
//        return (int16_t) 0;
//    }
//}


void konverter_sum_bidrag_PWM(void){
    sum_bidrag_PWM.hhf = konverter_paadrag_PWM(sum_bidrag.hhf);
    sum_bidrag_PWM.hhb = konverter_paadrag_PWM(sum_bidrag.hhb);
    sum_bidrag_PWM.hvf = konverter_paadrag_PWM(sum_bidrag.hvf);
    sum_bidrag_PWM.hvb = konverter_paadrag_PWM(sum_bidrag.hvb);
    //
    sum_bidrag_PWM.vhf = konverter_paadrag_PWM(sum_bidrag.vhf);
    sum_bidrag_PWM.vhb = konverter_paadrag_PWM(sum_bidrag.vhb);
    sum_bidrag_PWM.vvf = konverter_paadrag_PWM(sum_bidrag.vvf);
    sum_bidrag_PWM.vvb = konverter_paadrag_PWM(sum_bidrag.vvb);
}

void send_thruster_PWM(void){
    //oppdater timer verdier
    //SJEKK AT RIKTIG MED TANKE PÅ HORISONTAL/VERTIKAL OG PLASSERING

    TIM2->CCR1 = sum_bidrag_PWM.hhf - 1;  		// HHF: T5-PWM-PA0-TIM2_CH1
    TIM2->CCR2 = sum_bidrag_PWM.hhb - 1;  		// HHB: T6-PWM-PA1-TIM2_CH2
    TIM2->CCR3 = sum_bidrag_PWM.hvb - 1;  		// HVB: T7-PWM-PB10-TIM2_CH3
    TIM2->CCR4 = sum_bidrag_PWM.hvf - 1;  		// HVF: T8-PWM-PB11-TIM2_CH4
    TIM1->CCR1 = sum_bidrag_PWM.vhf - 1;  		// VHF: T1-PWM-PC0-TIM1_CH1
    TIM1->CCR2 = sum_bidrag_PWM.vhb - 1;  		// VHB: T2-PWM-PC1-TIM1_CH2
    TIM1->CCR3 = sum_bidrag_PWM.vvb - 1;  		// VVB: T3-PWM-PC2-TIM1_CH3
    TIM1->CCR4 = sum_bidrag_PWM.vvf - 1;  		// VVF: T4-PWM-PC3-TIM1_CH4
}

void thruster_data_lagring(void){

	operator_referanse_int_forrige.hiv =  operator_referanse_int.hiv;

    // lagre samplen k-1 for som skal brukes til filtering
    sum_bidrag_forrige1.hhf = sum_bidrag.hhf;
    sum_bidrag_forrige1.hhb = sum_bidrag.hhb;
    sum_bidrag_forrige1.hvf = sum_bidrag.hvf;
    sum_bidrag_forrige1.hvb = sum_bidrag.hvb;
    sum_bidrag_forrige1.vhf = sum_bidrag.vhf;
    sum_bidrag_forrige1.vhb = sum_bidrag.vhb;
    sum_bidrag_forrige1.vvf = sum_bidrag.vvf;
    sum_bidrag_forrige1.vvb = sum_bidrag.vvb;
    // på int8_t kan informasjon om hvilket pådrag som blir satt på motorene sendt opp igjen topside.
    sum_bidrag_int8.hhf = (int8_t) sum_bidrag.hhf;
    sum_bidrag_int8.hhb = (int8_t) sum_bidrag.hhb;
    sum_bidrag_int8.hvf = (int8_t) sum_bidrag.hvf;
    sum_bidrag_int8.hvb = (int8_t) sum_bidrag.hvb;
    sum_bidrag_int8.vhf = (int8_t) sum_bidrag.vhf;
    sum_bidrag_int8.vhb = (int8_t) sum_bidrag.vhb;
    sum_bidrag_int8.vvf = (int8_t) sum_bidrag.vvf;
    sum_bidrag_int8.vvb = (int8_t) sum_bidrag.vvb;
}


void behandle_sum_bidrag(void){

	// trenger ikke denne lenger, sjekk_saturation_pos har abs
    if (sjekk_saturation_minus()){
        skalere_ned_saturation_neg(-100);
    }

    // Sjekke om sum er større en maks som mulig thrustere generelt
    if (sjekk_saturation_pos()){
        skalere_ned_saturation_pos(100);
    }

    /// må finne variabler slik at ikke blir hakkete
    // sjekker om såppass stor differanse at må filtrere (horisontal)
    if (horisontal_differanse()){
        horisontal_demping();
    }

    // sjekker om såppass stor differanse at må filtrere (vertikal)
    if (vertikal_differanse()){
        vertikal_demping();
    }

    // blir ikke brukt, ignorer
//    // sjekke om innenfor effektbegrensing
//    if (effekt_sjekk()){
//        effekt_begrensing();
//    }

    konverter_sum_bidrag_PWM();

    send_thruster_PWM();

    thruster_data_lagring();

}






