/*
 * funksjoner_manpu.c
 *
 *  Created on: 14. apr. 2023
 *      Author: fridawagner
 */

#include "main.h"
#include <typedef_structures.h>
#include <globvariabler_dek.h>
#include "math.h"

/////////////////////////// MANIPULATOR-STYRING

//typedef struct {
//    int8_t gripe;
//    int8_t snurr;
//    int8_t sving;
//    int8_t skyv;
//
//    int8_t sensitivitet;
//    int8_t endre_frekvens;
//} id_manpu_struct_int ;
//

//  // TIMERE FOR MANIUPLATOR
//  //  PA4 GRIP
//  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
//
//  // PC6 SNURR --> BØRSTELØSE
//  HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
//
//  // PB14 SVING
//  HAL_TIM_PWM_Start(&htim15, TIM_CHANNEL_1);
//
//  // PB4 SKYV
//  HAL_TIM_PWM_Start(&htim16, TIM_CHANNEL_1);
//
//

///// SJEKKE OM SKAL KUNNE ENDRE FREKVENS

//	if((operator_manpu_int.endre_frekvens & 0x02)){
//		// beregne ny frekvens
//	}
//	if((operator_manpu_int.endre_frekvens & 0x04)){
//
//	}
//	if((operator_manpu_int.endre_frekvens & 0x08)){
//
//	}
//	if((operator_manpu_int.endre_frekvens & 0x10)){
//
//	}

void manipulator_data_lagring(void){
	 operator_manpu_forrige1.gripe =  operator_manpu.gripe;
	 operator_manpu_forrige1.snurr = operator_manpu.snurr;
	 operator_manpu_forrige1.sving = operator_manpu.sving;
	 operator_manpu_forrige1.skyv = operator_manpu.skyv;

	 operator_manpu_forrige1_int.gripe = (int8_t) operator_manpu_forrige1.gripe;
	 operator_manpu_forrige1_int.snurr = (int8_t) operator_manpu_forrige1.snurr;
	 operator_manpu_forrige1_int.sving = (int8_t) operator_manpu_forrige1.sving;
	 operator_manpu_forrige1_int.skyv =  (int8_t) operator_manpu_forrige1.skyv;
}


uint8_t sjekk_for_forskjellig_fortegn(int8_t var_naa,int8_t var_forrige){
	// return 1 trenger ikke skrive til pinne for rotasjonsretning
	if((var_naa > 0) & (var_forrige > 0)){
		return 1;
	}
	else if((var_naa < 0) & (var_forrige < 0)){
		return 1;
	}
	else if((var_naa < 0) & (var_forrige > 0)){
		return 0;
	}
	else if((var_naa > 0) & (var_forrige < 0)){
		return 0;
	}
	else if((var_naa == 0) & (var_forrige != 0)){
			return 0;
	}
	else if((var_naa != 0) & (var_forrige == 0)){
			return 10;
	}
	else if((var_naa == 0) & (var_forrige == 0)){
			return 0;
	}
	else{
		return 13;
	}
}


void sett_rotasjonsretning(float manpulator_variabel,GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin){
    if(manpulator_variabel > 0){
        HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET); // én retning
    }
    else if(manpulator_variabel < 0){
        HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);// annen retning
    }
    else{
        //
    }
}

uint16_t beregn_ARR(float manpulator_variabel,float maks_frek, float sensitivitet){
    float NUM = (1/maks_frek) / 0.000001;
    float a = ( -(65535-NUM) / 100);

    float manpu_var = fabs(manpulator_variabel)*sensitivitet;

    float ARR = (manpu_var*a) + 65535.0;
    return (uint16_t) ARR;
}

uint16_t beregn_CCR_snurr(float manpulator_variabel,float maks_bredde, float sensitivitet){
    float a = ((maks_bredde - 1500)) /100;
    float bredde;
    float manpu_var = manpulator_variabel*sensitivitet;

    if (manpu_var>0){
        bredde = manpu_var*a + 1500;
    }
    else if(manpu_var<0){
        bredde = manpu_var*a + 1500;
    }
    else{
        bredde = 1500;
    }
    return (int16_t) bredde;
}

//
//int16_t konverter_paadrag_PWM_eksempel(float paadrag){
//    //printf("KOVERTERER PAADRAG TIL PWM\n");
//    if(paadrag<0) {return (int16_t) (neg_paa_PWM.a*paadrag + neg_paa_PWM.b);}
//    else if(paadrag>0) {return (int16_t) (pos_paa_PWM.a*paadrag + pos_paa_PWM.b);}
//    else {return (int16_t) 0;}
//}


///// EKSEMPELOPPSETT LOWPASSFILTER

//void vertikal_demping(void){
//	float a =  lowpass_thruster.a;
//    sum_bidrag.vhf = (1-a) *sum_bidrag_forrige1.vhf +   a*sum_bidrag.vhf;
//    sum_bidrag.vhb = (1-a) *sum_bidrag_forrige1.vhb +   a*sum_bidrag.vhb ;
//    sum_bidrag.vvf = (1-a) *sum_bidrag_forrige1.vvf +   a*sum_bidrag.vvf;
//    sum_bidrag.vvb = (1-a) *sum_bidrag_forrige1.vvb +   a*sum_bidrag.vvb;
//}

void behandle_gripe(void){
	//// først sjekke om 0 (dersom under 1 eller -1 regnes som 0 her)

	//!(ENDRE_FREKVENS & 0x02)
	if(ENDRE_FREKVENS){
		// operator manpu gripe fra operator
		//manipulator_demping();
		// DEMPING
/*
 	 	float a = lowpass_manpu_steg.a;
		operator_manpu.gripe = (1-a)*operator_manpu_forrige1.gripe + a*operator_manpu.gripe ;
*/

		manu_ARR.gripe = beregn_ARR(operator_manpu.gripe,1100, manpu_sensitivitet.gripe);
	}

	else{
		/// default paadrag
		/// uansett verdi fra stikke, samme paadrag
		if(operator_manpu.gripe > 0){
			operator_manpu.gripe = 100;
		}
		else if(operator_manpu.gripe < 0){
			operator_manpu.gripe = -100;
		}
/*
 	 	float a = lowpass_manpu_steg.a;
		operator_manpu.gripe = (1-a)*operator_manpu_forrige1.gripe + a*operator_manpu.gripe ;

*/
		// her maks frekvens bare satt til 200Hz, kan endre
		manu_ARR.gripe = beregn_ARR(operator_manpu.gripe, 1100, 1.0);
	}

	uint8_t sjekk_var = sjekk_for_forskjellig_fortegn(operator_manpu_int.gripe,operator_manpu_forrige1_int.gripe);

	if(sjekk_var){
		// rotasjonsretning blir bare satt dersom forrige verdi var 0.
		if(sjekk_var == 10){
			// PC12 grip flipp
			sett_rotasjonsretning(operator_manpu.gripe,GPIOC,GPIO_PIN_11);
		}

		/// skriv til timer registere
		TIM16 -> ARR = manu_ARR.gripe - 1 ;
		TIM16 -> CCR1 = (manu_ARR.gripe/2) - 1 ; // PA4 gripe TIM3_CH2
	}
	else{
		TIM16 -> CCR1 = 0; // NB BYTTET TIMER MED SKYV
		operator_manpu.gripe = 0;
		//TIM3 -> ARR = 4999 ;
		//TIM3 -> CCR2 = 2499 ;
	}

}

///// SVING ---
void behandle_sving(void){
	//// først sjekke om 0 (dersom under 1 eller -1 regnes som 0 her)
	//!(ENDRE_FREKVENS & 0x04)
	if(ENDRE_FREKVENS){
		// operator manpu gripe fra operator

/*
		float a = lowpass_manpu_steg.a;
		operator_manpu.sving = (1-a)*operator_manpu_forrige1.sving + a*operator_manpu.sving;
*/

		manu_ARR.sving = beregn_ARR(operator_manpu.sving,800, manpu_sensitivitet.sving);
	}
	else{
		/// default paadrag
		/// uansett verdi fra stikke, samme paadrag
		if(operator_manpu.sving > 0){
			operator_manpu.sving = 100;
		}
		else if(operator_manpu.sving < 0){
			operator_manpu.sving = -100;
		}


/*
		float a = lowpass_manpu_steg.a;
		operator_manpu.sving = (1-a)*operator_manpu_forrige1.sving + a*operator_manpu.sving;
*/

		manu_ARR.sving = beregn_ARR(operator_manpu.sving, 800, 1.0);
	}

	uint8_t sjekk_var = sjekk_for_forskjellig_fortegn(operator_manpu_int.sving,operator_manpu_forrige1_int.sving);

	if(sjekk_var){
		// rotasjonsretning blir bare satt dersom forrige verdi var 0.
		if(sjekk_var == 10){
			// PC10 sving flipp
			sett_rotasjonsretning(operator_manpu.sving,GPIOC,GPIO_PIN_10);
		}

		/// skriv til timer registere
		TIM15 -> ARR = manu_ARR.sving - 1 ;  // TIM15_CH1-PB14
		TIM15 -> CCR1 = (manu_ARR.sving/2) - 1 ;
	}
	else{
		TIM15 -> CCR1 = 0;
		operator_manpu.sving = 0;
		//TIM15 -> ARR = 4999 ;
		//TIM15 -> CCR1 = 2499 ;
	}

}

///// SKYV - teleskop
void behandle_skyv(void){
	//// først sjekke om 0 (dersom under 1 eller -1 regnes som 0 her)
	/// ønsker at skyv/teleskop funksjon ikke skal variere på frekvens men
	/// ha fast hastighet, derfor !(ENDRE_FREKVENS & 0x08)
	/// rask men ikke beste løsning

	//!(ENDRE_FREKVENS & 0x08)
	if(ENDRE_FREKVENS){
		// operator manpu gripe fra operator

/*
		float a = lowpass_manpu_steg.a;
		operator_manpu.skyv = (1-a)*operator_manpu_forrige1.skyv + a*operator_manpu.skyv;

*/

		manu_ARR.skyv = beregn_ARR(operator_manpu.skyv, 1100, manpu_sensitivitet.skyv);
	}
	else{
		/// default paadrag
		/// uansett verdi fra stikke, samme paadrag
		if(operator_manpu.skyv > 0){
			operator_manpu.skyv = 100;
		}
		else if(operator_manpu.skyv < 0){
			operator_manpu.skyv = -100;
		}

		/*

		float a = lowpass_manpu_steg.a;
		operator_manpu.skyv = (1-a)*operator_manpu_forrige1.skyv + a*operator_manpu.skyv;

*/
		manu_ARR.skyv = beregn_ARR(operator_manpu.skyv, 1100, 1.0);
	}

	uint8_t sjekk_var = sjekk_for_forskjellig_fortegn(operator_manpu_int.skyv,operator_manpu_forrige1_int.skyv);

	if(sjekk_var){
		// rotasjonsretning blir bare satt dersom forrige verdi var 0.
		if(sjekk_var == 10){
			// PC11 skyv flipp
			sett_rotasjonsretning(operator_manpu.skyv,GPIOC,GPIO_PIN_12);
		}

		/// skriv til timer registere
		TIM3 -> ARR = manu_ARR.skyv - 1 ;  // PB04 , TIM16_CH1
		TIM3 -> CCR2 = (manu_ARR.skyv/2) - 1 ;
	}
	else{
		TIM3 -> CCR2 = 0;
		operator_manpu.skyv = 0;
		//TIM16 -> ARR = 4999 ;
		//TIM16 -> CCR1 = 2499 ;
	}

}

////////// IKKE DENNE
////// SNURR - BØRSTELØS
void behandle_snurr(void){
 ///ENDRE_FREKVENS & 0x10
	// dersom bit for endre frekvens lik 1, skal kunne endre frek
	if(ENDRE_FREKVENS){
		// operator manpu gripe fra operator


 	 	float a = lowpass_manpu_borste.a;
		operator_manpu.snurr = (1-a)*operator_manpu_forrige1.snurr + a*operator_manpu.snurr;


		manu_CCR.snurr = beregn_CCR_snurr(operator_manpu.snurr, 1900, manpu_sensitivitet.snurr);
	}
	else{
		/// default paadrag
		/// uansett verdi fra stikke, samme paadrag
		if(operator_manpu.snurr > 0){
			operator_manpu.snurr = 100;
		}
		else if(operator_manpu.snurr < 0){
			operator_manpu.snurr = -100;
		}


 	 	float a = lowpass_manpu_borste.a;
		operator_manpu.snurr = (1-a)*operator_manpu_forrige1.snurr + a*operator_manpu.snurr;


		// setter maks bredde som er fast til feks 1700, kan endres.
		manu_CCR.snurr = beregn_CCR_snurr(operator_manpu.snurr, 1900, 1.0);
	}

	uint8_t sjekk_var = sjekk_for_forskjellig_fortegn(operator_manpu_int.snurr,operator_manpu_forrige1_int.snurr);

	// dersom endring av fortegn, skrur av motor for så at skus på andre retning
	// neste gang.

	/// NB SETT ØNSKET FREKVENS I IOC
	if(sjekk_var){
		/// skriv til timer registere
		TIM8 -> CCR1 = manu_CCR.snurr; // PC06 snurr TIM8_CH1
	}
	else{
		TIM8 -> CCR1 = 1500;
		operator_manpu.snurr = 0;
	}

}



void behandle_manipulator(void){
	/// casting
    operator_manpu.gripe = (float) operator_manpu_int.gripe;
    operator_manpu.skyv = (float) operator_manpu_int.skyv;
    operator_manpu.snurr = (float) operator_manpu_int.snurr;
    operator_manpu.sving = (float) operator_manpu_int.sving;


	behandle_gripe();
	behandle_skyv();
	behandle_sving();
	behandle_snurr();

	manipulator_data_lagring();
}



