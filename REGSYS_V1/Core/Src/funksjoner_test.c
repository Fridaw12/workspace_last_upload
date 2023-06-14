/*
 * funksjoner_test.c
 *
 *  Created on: Feb 21, 2023
 *      Author: fridawagner
 */

#include <globvariabler_dek.h>
#include <funksjoner_manpu.h>
#include "main.h"

void sjekk_bryter(void){
	if (GPIOC->IDR & B1_Pin) { 			// Sjekker om bryteren er trykket inn
       if(!bryter_forrige_status) { 				// Var bryteren trykket inn sist kontrollsjekk
    	   bryter_forrige_status = 1;
    	   bryter_trykk = 1;      				// Nytt brytertrykk registrert
       }
	}
	else {
		bryter_forrige_status = 0;
	}


	TIM8 -> CCR1 = 1500;
}


void test_manpu_borstelos(void){
	if (bryter_trykk){
		PWM_bredde++;
		bryter_trykk = 0;

		switch(PWM_bredde){
		case 1:
			TIM8 -> CCR1 = 1900;
			break;
		case 2:
			TIM8 -> CCR1 = 1500;
			break;
		case 3:
			TIM8 -> CCR1 = 1100;
			break;
		case 4:
			TIM8 -> CCR1 = 1500;
			PWM_bredde=0;
			break;
		}
	}
}

//void teste_manpumotorer(void){
//	if(bryter_trykk){
//		PWM_bredde++;
//		bryter_trykk = 0;
//
//		switch(PWM_bredde){
//		case 1:
//			TIM8->CCR1 = 1899;
//			/// steg1 GRIPE
//			TIM8->ARR = 4999;
//			TIM8->CCR1 = 4999/2;
//			/// steg2 SVING
//			TIM15->ARR = 4999;
//			TIM15->CCR1 = 4999/2;
//			/// steg3 SKYV
//			TIM16->ARR = 4999;
//			TIM16->CCR1 = 4999/2;
//			break;
//		case 2:
//			//HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_1);
//			TIM8->CCR1 = 1499;
//			/// steg1 GRIPE
//			TIM8->ARR = 4999;
//			TIM8->CCR1 = 0;
//			/// steg2 SVING
//			TIM15->ARR = 4999;
//			TIM15->CCR1 = 0;
//			/// steg3 SKYV
//			TIM16->ARR = 4999;
//			TIM16->CCR1 = 0;
//			break;
//		case 3:
//			//HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
//			TIM8->CCR1 = 1099;
//			/// steg1 GRIPE
//			sett_rotasjonsretning(operator_manpu.gripe,GPIOC,GPIO_PIN_12);
//			TIM8->ARR = 4999;
//			TIM8->CCR1 = 4999/2;
//			/// steg2 SVING
//			sett_rotasjonsretning(operator_manpu.sving,GPIOC,GPIO_PIN_10);
//			TIM15->ARR = 4999;
//			TIM15->CCR1 = 4999/2;
//			/// steg3 SKYV
//			sett_rotasjonsretning(operator_manpu.skyv,GPIOC,GPIO_PIN_11);
//			TIM16->ARR = 4999;
//			TIM16->CCR1 = 4999/2;
//			break;
//		case 4:
//			//HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_1);
//			TIM8->CCR1 = 1499;
//			/// steg1 GRIPE
//			TIM8->ARR = 4999;
//			TIM8->CCR1 = 0;
//			/// steg2 SVING
//			TIM15->ARR = 4999;
//			TIM15->CCR1 = 0;
//			/// steg3 SKYV
//			TIM16->ARR = 4999;
//			TIM16->CCR1 = 0;
//			break;
//		//case 5:
//			//HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_1);
//			//TIM8->CCR1 = 1499;
//			//PWM_bredde = 0;
//			//break;
////		case 6:
////			TIM8->CCR1 = 1469;
////			break;
////		case 7:
////			TIM8->CCR1 = 1499;
////			break;
////		case 8:
////			TIM8->CCR1 = 1529;
////			PWM_bredde = 0;
////			break;
//
//		}
//	}
//}





	  /////// 21.02.23 oppsett for forste test PWM signal

	  /////// NB; for å lage ny referanse til nytt prosjekt, dupliser
	  /////// RESGSYS_V1 prosjekt og legg i nytt workspace, duplisering
	  /////// av workspace lager ikke ny referanse (ser det ut til)

	  ////////// FOR TESTING


//	 ///// 30.03.23 PWM test pcb
//	if(bryter_trykk){
//		PWM_bredde++;
//		bryter_trykk = 0;
//
//		switch(PWM_bredde){
//		case 1:
//			/// timer 1 thrustere
//			TIM1->CCR1 = 999;
//			TIM1->CCR2 = 999;
//			TIM1->CCR3 = 999;
//			TIM1->CCR4 = 999;
//
//			// timer 2 thrustere
//			TIM2->CCR1 = 999;
//			TIM2->CCR2 = 999;
//			TIM2->CCR3 = 999;
//			TIM2->CCR4 = 999;
//
//			// børsteløs manipulator
//			TIM8->CCR1 = 1099;
//
//			// stepper manipulator
//			TIM3->ARR = 9999;
//			TIM3->CCR2 = 4999;
//
//			TIM15->ARR = 9999;
//			TIM15->CCR1 = 4999;
//
//			TIM16->ARR = 9999;
//			TIM16->CCR1 = 4999;
//
//			break;
//		case 2:
//			TIM1->CCR1 = 1999;
//			TIM1->CCR2 = 1999;
//			TIM1->CCR3 = 1999;
//			TIM1->CCR4 = 1999;
//
//			TIM2->CCR1 = 1999;
//			TIM2->CCR2 = 1999;
//			TIM2->CCR3 = 1999;
//			TIM2->CCR4 = 1999;
//			////
//			TIM8->CCR1 = 1899;
//			////
//			TIM3->ARR = 1666;
//			TIM3->CCR2 = 833;
//			////
//			TIM15->ARR = 1666;
//			TIM15->CCR1 = 833;
//
//			TIM16->ARR = 1666;
//			TIM16->CCR1 = 833;
//			break;
//		case 3:
//			TIM1->CCR1 = 1499;
//			TIM1->CCR2 = 1499;
//			TIM1->CCR3 = 1499;
//			TIM1->CCR4 = 1499;
//			////
//			TIM2->CCR1 = 1499;
//			TIM2->CCR2 = 1499;
//			TIM2->CCR3 = 1499;
//			TIM2->CCR4 = 1499;
//			///
//			TIM8->CCR1 = 1499;
//			///
//			TIM3->ARR = 2499;
//			TIM3->CCR2 = 1249;
//			////
//			TIM15->ARR = 2499;
//			TIM15->CCR1 = 1249;
//			///
//			TIM16->ARR = 2499;
//			TIM16->CCR1  = 1249;
//			PWM_bredde = 0;
//			break;
//		}
//	}
//
//


	/////////// test av alle thrustere 19.03.2023

//	if(bryter_trykk){
//		PWM_bredde++;
//		bryter_trykk = 0;
//
//		switch(PWM_bredde){
//		case 1:
//			TIM1->CCR1 = 999;
//			TIM1->CCR2 = 999;
//			TIM1->CCR3 = 999;
//			TIM1->CCR4 = 999;
//			break;
//		case 2:
//			TIM1->CCR1 = 1299;
//			TIM1->CCR2 = 1299;
//			TIM1->CCR3 = 1299;
//			TIM1->CCR4 = 1299;
//			break;
//		case 3:
//			TIM1->CCR1 = 1499;
//			TIM1->CCR2 = 1499;
//			TIM1->CCR3 = 1499;
//			TIM1->CCR4 = 1499;
//			break;
//		case 4:
//			TIM1->CCR1 = 1999;
//			TIM1->CCR2 = 1999;
//			TIM1->CCR3 = 1999;
//			TIM1->CCR4 = 1999;
//			break;
//		case 5:
//			TIM1->CCR1 = 1699;
//			TIM1->CCR2 = 1699;
//			TIM1->CCR3 = 1699;
//			TIM1->CCR4 = 1699;
//			break;
//		case 6:
//			TIM1->CCR1 = 1499;
//			TIM1->CCR2 = 1499;
//			TIM1->CCR3 = 1499;
//			TIM1->CCR4 = 1499;
//			break;
//		case 7:
//			// NB; var hakkete av og på på 1459
//			// er ennå på
//			TIM1->CCR1 = 1449;
//			break;
//		case 8:
//			// skrur seg av
//			TIM1->CCR1 = 1469;
//			break;
//		case 9:
//			// NB; var hakkete av og på på 1510;
//			// skrur seg på
//			TIM1->CCR1 = 1514;
//			break;
//		case 10:
//			//NB; var hakkete av og på når sto på 1509 , 1505
//			TIM1->CCR1 = 1500;
//			break;
//		case 11:
//			TIM1->CCR1 = 1499;
//			PWM_bredde = 0;
//			break;
//		}
//	}

	/// test av CAN-kommunikasjon

//	switch(gyrodata_m1.rull){
//	case 1:
//		TIM1->CCR1 = 1099;
//		break;
//	case 10:
//		TIM1->CCR1 = 1899;
//		break;
//	case 100:
//		TIM1->CCR1 = 1499;
//		break;
//	}
//




//	if (send_melding){
//		ny_melding++;
////	// Eksempel med 2x uint8, 1x uint16 og 1x uint32
//		switch(ny_melding){
//		case 1:
//			førsteuint8 = 1;
//			break;
//		case 2:
//			førsteuint8 = 10;
//			break;
//		case 3:
//			førsteuint8 = 100;
//			ny_melding = 0;
//			break;
//		}
//		memcpy(&TxData, &førsteuint8, 2); // Kopierer 1 byte fra førsteuint8 til første byte i TxData
////	memcpy(&TxData[3],&førsteuint16, 2); // Kopierer 2 byte fra førsteuint16 til tredje og fjerde byte i TxData
////	memcpy(&TxData[5],&førsteuint32, 4); // Kopierer 4 byte fra førsteuint32 til femte, sjette, syvende og åttende byte i TxData
//		sendCAN(34, &hfdcan1);
//		send_melding = 0;
//}


//	// Eksempel med 4x uint16
//	memcpy(TxData, &førsteuint16, 2); // Kopierer 2 byte fra førsteuint16 til to første byte i TxData
//	memcpy(&TxData[2], &andreuint16, 2); // Kopierer 2 byte fra andreuint16 til tredje og fjerde byte i TxData
//	memcpy(&TxData[4], &tredjeuint16, 2); // Kopierer 2 byte fra tredjeuint16 til femte og sjette byte i TxData
//	memcpy(&TxData[6], &fjerdeuint16, 2); // Kopierer 2 byte fra fjerdeuint16 til syvende og åttende byte i TxData
//


//void sjekk_bryter(void){
//	if (GPIOC->IDR & B1_Pin) { 			// Sjekker om bryteren er trykket inn
//       if(!bryter_forrige_status) { 				// Var bryteren trykket inn sist kontrollsjekk
//    	   bryter_forrige_status = 1;
//    	   bryter_trykk = 1;      				// Nytt brytertrykk registrert
//    	   bryter_antall++;
//       }
//	}
//    else {                 						// Hvis bryteren ikke er trykket inn
//    	bryter_forrige_status = 0; 				// Bryterstatus settes til 0.
//    	bryter_trykk = 0;
//    }
//}






// FOR syvsegment; first bit in to shift goes out on Q7
// for å fylle data til output register, må først fylle skift register me 8*2 bits
// dette er fordi to shiftregister er koblet i serie,
// og begge to skal fylles opp med 8 bit

//LATCH pin må være lav før data har blitt fullstendig overført


// void shiftOut(uint8_t * data, uint16_t size){

// FRA RICARDO VIDEO I ARDUINO;
// digitalwrite(LatchPin,LOW); set to low potential before sending data
// shiftOut(dataPin,clockPin,MSBFIRST,B00000000)
// digitalwrite(LatchPin,HIGH); set to high potential after sending data

// FRA MUTE VIDEO;


