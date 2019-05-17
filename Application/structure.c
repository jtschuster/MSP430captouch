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
/*
 * structure.c
 * 
 * RO_CSIO_TA2_WDTA_One_Button example
 * Touch the middle element to turn on/off the center button LED
 * RO method capactiance measurement using Cap sense IO, TimerA2, and WDT+
 *
 * Also provided RO_CSIO_TA2_TA3_One_Button and fRO_CSIO_TA2_TA3_One_Button 
 * examples. Users can define by themselves which measuring method is used
 * All the parameters like threshold are all configured based on specific 
 * Launchpad and Boostpack, which may be modified according to user's 
 * own applications.
 *
 * Revised by Holly Gu on March 1st
 */

#include "structure.h"


const struct Element left_button = //P4.1
{
    //.inputBits = CAPSIOPOSEL0 +CAPSIOPISEL2,// + CAPSIOPISEL0,   // select 1.5
     .inputBits = CAPSIOPOSEL2 + CAPSIOPISEL0,
    /* When using an abstracted function to measure the element
     * the 100*(maxResponse - threshold) < 0xFFFF
     * ie maxResponse - threshold < 655
     */

    // RO_CSIO_TA2_WDTA  suggested threshold = 1000;
    // RO_CSIO_TA2_TA3   suggested threshold = 1200;
    // fRO_CSIO_TA2_TA3   suggested threshold = 480;

    .maxResponse = 1000+655,
    .threshold = 1000
};

const struct Element right_button = // P4.2
{
    //.inputBits = CAPSIOPOSEL0 +CAPSIOPISEL2,// + CAPSIOPISEL0,   // select 1.5
     .inputBits = CAPSIOPOSEL2 + CAPSIOPISEL1,
    /* When using an abstracted function to measure the element
     * the 100*(maxResponse - threshold) < 0xFFFF
     * ie maxResponse - threshold < 655
     */

    // RO_CSIO_TA2_WDTA  suggested threshold = 1000;
    // RO_CSIO_TA2_TA3   suggested threshold = 1200;
    // fRO_CSIO_TA2_TA3   suggested threshold = 480;

    .maxResponse = 1000+655,
    .threshold = 1000
};

const struct Element left_slider = //P6.0
{
    //.inputBits = CAPSIOPOSEL0 +CAPSIOPISEL2,// + CAPSIOPISEL0,   // select 1.5
     .inputBits = CAPSIOPOSEL2 + CAPSIOPOSEL1,
    /* When using an abstracted function to measure the element
     * the 100*(maxResponse - threshold) < 0xFFFF
     * ie maxResponse - threshold < 655
     */

    // RO_CSIO_TA2_WDTA  suggested threshold = 1000;
    // RO_CSIO_TA2_TA3   suggested threshold = 1200;
    // fRO_CSIO_TA2_TA3   suggested threshold = 480;

    .maxResponse = 1000+655,
    .threshold = 1000
};

const struct Element middle_slider = //P6.1
{
    //.inputBits = CAPSIOPOSEL0 +CAPSIOPISEL2,// + CAPSIOPISEL0,   // select 1.5
     .inputBits = CAPSIOPOSEL2 + CAPSIOPOSEL1 + CAPSIOPISEL0,
    /* When using an abstracted function to measure the element
     * the 100*(maxResponse - threshold) < 0xFFFF
     * ie maxResponse - threshold < 655
     */

    // RO_CSIO_TA2_WDTA  suggested threshold = 1000;
    // RO_CSIO_TA2_TA3   suggested threshold = 1200;
    // fRO_CSIO_TA2_TA3   suggested threshold = 480;

    .maxResponse = 1000+655,
    .threshold = 1000
};

const struct Element right_slider = //P6.2
{
    //.inputBits = CAPSIOPOSEL0 +CAPSIOPISEL2,// + CAPSIOPISEL0,
     .inputBits = CAPSIOPOSEL2 + CAPSIOPOSEL1 + CAPSIOPISEL1,
    /* When using an abstracted function to measure the element
     * the 100*(maxResponse - threshold) < 0xFFFF
     * ie maxResponse - threshold < 655
     */

    // RO_CSIO_TA2_WDTA  suggested threshold = 1000;
    // RO_CSIO_TA2_TA3   suggested threshold = 1200;
    // fRO_CSIO_TA2_TA3   suggested threshold = 480;

    .maxResponse = 1000+655,
    .threshold = 1000
};



// One Button Sensor
const struct Sensor button_one =
{ 
    /****** For RO_CSIO_TA2_WDTA **************/
    .halDefinition = RO_CSIO_TA2_WDTA,
    .inputCapsioctlRegister = (uint16_t *)&CAPSIO0CTL,
    .numElements = 1,						
    .baseOffset = 0,						
    // Pointer to elements
    .arrayPtr[0] = &left_button,  		// point to middle element
    // Timer Information
    .measGateSource= GATE_WDTA_VLO,     //  Gate Clock Source
    .accumulationCycles= WDTA_GATE_64


    /****** For RO_CSIO_TA2_TA3 **************/
    /*.halDefinition = RO_CSIO_TA2_TA3,
    .inputCapsioctlRegister = (uint16_t *)&CAPSIO0CTL,
    .numElements = 1,
    .baseOffset = 0,
    // Pointer to elements
    .arrayPtr[0] = &middle_element,  		// point to middle element
    // Timer Information, which determine the Gate Time
    .measGateSource= TIMER_ACLK,     // Clock Source for the Gate Timer A3
    .sourceScale = TIMER_SOURCE_DIV_3,
    .accumulationCycles= 10        //CCRx for Timer A3*/

    /****** For fRO_CSIO_TA2_TA3 **************/
    /*.halDefinition = fRO_CSIO_TA2_TA3,
    .inputCapsioctlRegister = (uint16_t *)&CAPSIO0CTL, 
    .numElements = 1,						
    .baseOffset = 0,						
    // Pointer to elements
    .arrayPtr[0] = &middle_element,  		// point to middle element
    // Timer Information, which determine the Gate Time
    .measGateSource= TIMER_SMCLK,     // Clock source for Timer A3
    .sourceScale = TIMER_SOURCE_DIV_0,
    .accumulationCycles= 10000       
    // accumulationCycles represets CCRx for Gate Timer A2, 
    // the clock source is by OSC*/

};

const struct Sensor button_two =
{
    /****** For RO_CSIO_TA2_WDTA **************/
    .halDefinition = RO_CSIO_TA2_WDTA,
    .inputCapsioctlRegister = (uint16_t *)&CAPSIO0CTL,
    .numElements = 1,
    .baseOffset = 0,
    // Pointer to elements
    .arrayPtr[0] = &right_button,        // point to middle element
    // Timer Information
    .measGateSource= GATE_WDTA_VLO,     //  Gate Clock Source
    .accumulationCycles= WDTA_GATE_64
};



const struct Sensor slider_one =
{
    /****** For RO_CSIO_TA2_WDTA **************/
    .halDefinition = RO_CSIO_TA2_WDTA,
    .inputCapsioctlRegister = (uint16_t *)&CAPSIO0CTL,
    .numElements = 1,
    .baseOffset = 0,
    // Pointer to elements
    .arrayPtr[0] = &left_slider,        // point to middle element
    // Timer Information
    .measGateSource= GATE_WDTA_VLO,     //  Gate Clock Source
    .accumulationCycles= WDTA_GATE_64
};



const struct Sensor slider_three =
{
    /****** For RO_CSIO_TA2_WDTA **************/
    .halDefinition = RO_CSIO_TA2_WDTA,
    .inputCapsioctlRegister = (uint16_t *)&CAPSIO0CTL,
    .numElements = 1,
    .baseOffset = 0,
    // Pointer to elements
    .arrayPtr[0] = &middle_slider,        // point to middle element
    // Timer Information
    .measGateSource= GATE_WDTA_VLO,     //  Gate Clock Source
    .accumulationCycles= WDTA_GATE_64
};



const struct Sensor slider_five =
{
    /****** For RO_CSIO_TA2_WDTA **************/
    .halDefinition = RO_CSIO_TA2_WDTA,
    .inputCapsioctlRegister = (uint16_t *)&CAPSIO0CTL,
    .numElements = 1,
    .baseOffset = 0,
    // Pointer to elements
    .arrayPtr[0] = &right_slider,        // point to middle element
    // Timer Information
    .measGateSource= GATE_WDTA_VLO,     //  Gate Clock Source
    .accumulationCycles= WDTA_GATE_64
};
