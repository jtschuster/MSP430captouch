/* --COPYRIGHT--,BSD
 * Copyright (c) 2016, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
//******************************************************************************
// RO_CSIO_TA2_WDTA_One_Button example
// Touch the middle element to turn on/off the center button LED
// RO method capactiance measurement using CSIO, TimerA2, and WDTA
//
//          Schematic Description: 
// 
//                         MSP430FR5969
//                      +---------------+
//                      |
//             C--------|P1.4
//           C----------|P3.5
//               C------|P1.3
//             C--------|P3.6
//                      |
//           C----------|P1.5
//                      |
//           C----------|P3.4
// 
//        The WDTA interval represents the measurement window.  The number of
//        counts within the TA2R that have accumulated during the measurement
//        window represents the capacitance of the element. This is lowest 
//        power option with either LPM3 (ACLK WDTp source) or LPM0 (SMCLK WDTp 
//        source).
//
// This is the preliminary code revised by Holly Gu
// 27 Feb, 2013
//******************************************************************************
#include "CTS_Layer.h"

// Uncomment to have this compiler directive run characterization functions only
// Comment to have this compiler directive run example application
//#define ELEMENT_CHARACTERIZATION_MODE

#define DELAY 200 		// Timer delay timeout count - 5000*0.1msec = 500 msec

#ifdef ELEMENT_CHARACTERIZATION_MODE
// Delta Counts returned from the API function for the sensor during characterization
unsigned int dCnt;
unsigned int buttonCnt[5];               // Becuase the Wheel is composed of five elements
#endif

struct Element * keyPressed;            // Pointer to the Element structure


unsigned char tempflag;

// Sleep Function
// Configures Timer A to run off ACLK, count in UP mode, places the CPU in LPM3 
// and enables the interrupt vector to jump to ISR upon timeout 
void sleep(unsigned int time)
{
    TA0CCR0 = time;
    TA0CTL = TASSEL__ACLK + MC__UP + TACLR;
    TA0CCTL0 &= ~CCIFG;
    TA0CCTL0 = CCIE;
    __bis_SR_register(LPM3_bits+GIE);
    __no_operation();
}

// Main Function
void main(void)
{ 

  // Initialize System Clocks
  WDTCTL = WDTPW + WDTHOLD;             // Stop watchdog timer
  
  //Clock Setting
  CSCTL0_H = 0xA5;                          // Unlock CS
  CSCTL1 = DCORSEL + DCOFSEL_5;             // Set DCO = 20MHz, use this clk when fRO
  CSCTL2 = SELA__VLOCLK + SELS_3 + SELM_3;  // set SMCLK = MCLK = DCO  LFXT1 = VLO
  CSCTL0_H = 0;                             // Lock CS registers
  // Initialize better
  P1DIR |= BIT1 + BIT0 + BIT2 + BIT3;
  P1OUT = 0;

  PM5CTL0 &= ~LOCKLPM5;

  // Set up slider state machine
  unsigned int slider_last = 0;
  /* 0: no button last pressed
   * 1: 1 last pressed
   * 2: 1 pressed, then 3 last pressed
   * 4: 5 pressed, then 3 last pressed
   * 5: 5 last pressed
   */



  // Initialize Baseline measurement
  TI_CAPT_Init_Baseline(&buttons);


  // Update baseline measurement (Average 5 measurements)
  TI_CAPT_Update_Baseline(&buttons,5);

  
  // Main loop starts here
  while (1)
  {
  	
  	#ifdef ELEMENT_CHARACTERIZATION_MODE
	// Get the raw delta counts for element characterization 
	TI_CAPT_Custom(&buttons,buttonCnt);
	__no_operation(); 					// Set breakpoint here	
	#endif
	  	
	#ifndef ELEMENT_CHARACTERIZATION_MODE	  	
    // Return the pointer to the element which has been touched
    keyPressed = (struct Element *)TI_CAPT_Buttons(&buttons);

    if(keyPressed){
        if(keyPressed == &left_button){
            P1OUT |= BIT0;
        }
        if(keyPressed == &right_button) {
            P1OUT |= BIT1;
        }

        if(keyPressed == &left_slider) {
            if (slider_last == 4) { // LeftLED
                P1OUT |= BIT2;
                P1OUT &= ~BIT3;
            }
            slider_last = 1;
        }
        if(keyPressed == &middle_slider) {
            if (slider_last == 1 || slider_last == 2) {
                slider_last = 2;
            }
            else if (slider_last == 5 || slider_last == 4) {
                slider_last = 4;
            }
            else {
                slider_last = 3;
            }
        }
        if(keyPressed == &right_slider) {
            if (slider_last == 2) { // Right LED
                P1OUT |= BIT3;
                P1OUT &= ~BIT2;
            }
            slider_last = 5;
        }

    }
    else{
        P1OUT &= ~BIT1;
        P1OUT &= ~BIT0;
    }



//	if(TI_CAPT_Button(&button_one)) {
//		// Do something
//		//P4OUT |= BIT3;                            // Turn on center LED
//		//P4OUT &= ~BIT2;
// 		P1OUT |= BIT0;
//	}
//	else {
//		//P4OUT &= ~BIT3;                           // Turn off center LED
//		//P4OUT |= BIT2;
//		P1OUT &= ~BIT0;
//    }
//
//	if(TI_CAPT_Button(&button_two)) {
//        //P4OUT |= BIT4;                            // Turn on center LED
//        P1OUT |= BIT1;
//    }
//    else {
//        //P4OUT &= ~BIT4;                           // Turn off center LED
//        P1OUT &= ~BIT1;
//    }
//    // Put the MSP430 into LPM3 for a certain DELAY period
//
//    TI_CAPT_Button(&slider_five);

    sleep(DELAY);
    #endif



//  switch(slider_last){
//      case 1:
//          if (TI_CAPT_Button(&slider_three)){
//              slider_last = 2;
//          }
//          else if (TI_CAPT_Button(&slider_five)){
//              slider_last = 5;
//          }
//          break;
//      case 2:
//            if (TI_CAPT_Button(&slider_five)){
//                P4OUT |= BIT3;
//                P4OUT &= ~BIT4;
//                slider_last = 0;
//            }
//            else if (TI_CAPT_Button(&slider_one)){
//                slider_last = 1;
//            }
//            break;
//      case 5:
//          if (TI_CAPT_Button(&slider_three)){
//              slider_last = 4;
//          }
//          else if (TI_CAPT_Button(&slider_one)){
//              slider_last = 1;
//          }
//          break;
//      case 4:
//          if (TI_CAPT_Button(&slider_one)){
//              //P4OUT |= BIT4;
//              P4OUT &= ~BIT3;
//              slider_last = 0;
//          }
//          else if (TI_CAPT_Button(&slider_five)){
//              slider_last = 5;
//          }
//          break;
//      case 0:
//          if (TI_CAPT_Button(&slider_five))
//              slider_last = 5;
//              break;
//          if (TI_CAPT_Button(&slider_one))
//              slider_last = 1;
//          break;
//          }
  }


} // End Main

/******************************************************************************/
// Timer0_A0 Interrupt Service  Routine: Disables the timer and exists LPM3
/******************************************************************************/
#pragma vector=TIMER0_A0_VECTOR
__interrupt void ISR_Timer0_A0(void)
{
  TA0CTL &= ~(MC_1);
  TA0CCTL0 &= ~(CCIE);
  __bic_SR_register_on_exit(LPM3_bits+GIE);
}

#pragma vector=RTC_VECTOR,PORT2_VECTOR,TIMER0_A1_VECTOR,TIMER3_A1_VECTOR,     \
  USCI_A1_VECTOR,PORT1_VECTOR,TIMER1_A1_VECTOR,DMA_VECTOR,TIMER2_A1_VECTOR,   \
  USCI_B0_VECTOR,USCI_A0_VECTOR,TIMER0_B1_VECTOR,TIMER0_B0_VECTOR,            \
  UNMI_VECTOR,SYSNMI_VECTOR,AES256_VECTOR, PORT3_VECTOR,TIMER3_A0_VECTOR,     \
  PORT4_VECTOR, TIMER1_A0_VECTOR, TIMER2_A0_VECTOR,                           \
  ADC12_VECTOR /* ,COMP_E_VECTOR */
__interrupt void ISR_trap(void)
{
  // the following will cause an access violation which results in a SW BOR
    PMMCTL0 = PMMPW | PMMSWBOR;
}
