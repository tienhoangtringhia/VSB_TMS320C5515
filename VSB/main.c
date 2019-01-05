/*****************************************************************************/
/*                                                                           */
/*                                                                           */
/* FILENAME                                                                  */
/*  main.c                                                                   */
/*                                                                           */
/* DESCRIPTION                                                               */
/*   TMS320C5515 USB Stick. Code containing main() function.                 */
/*****************************************************************************/

#include "stdio.h"
#include "usbstk5515.h"
#include "aic3204.h"
#include "sinewaves.h"
#include "Transfer.h"
#include "PLL.h"

#define SAMPLES_PER_SECOND 48000
#define GAIN_IN_dB  5

Int16 left_input;
Int16 right_input;
Int16 left_output;
Int16 right_output;

unsigned long int i = 0;

#define Amp 3				// The amplitude of carrier wave
#define N 1000				// The number of samples


Int16 BP[256] = {
     -114,    -61,     17,     86,    114,     90,     27,    -46,    -95,
     -100,    -59,      5,     63,     89,     74,     28,    -27,    -65,
      -70,    -45,     -3,     34,     51,     44,     18,     -9,    -26,
      -27,    -16,     -2,      5,      3,     -3,     -5,      2,     17,
       30,     29,     11,    -22,    -53,    -64,    -44,      4,     59,
       95,     90,     38,    -41,   -110,   -136,    -98,     -8,     97,
      166,    162,     80,    -48,   -165,   -214,   -165,    -34,    124,
      235,    244,    138,    -41,   -212,   -294,   -244,    -76,    137,
      298,    328,    207,    -17,   -245,   -369,   -327,   -131,    134,
      349,    410,    284,     21,   -262,   -433,   -411,   -198,    114,
      383,    483,    364,     73,   -260,   -482,   -488,   -270,     78,
      397,    542,    440,    134,   -239,   -510,   -552,   -342,     28,
      391,    580,    506,    200,   -201,   -516,   -598,   -408,    -30,
      365,    596,    557,    264,   -151,   -499,   -622,   -462,    -92,
      321,    588,    588,    321,    -92,   -462,   -622,   -499,   -151,
      264,    557,    596,    365,    -30,   -408,   -598,   -516,   -201,
      200,    506,    580,    391,     28,   -342,   -552,   -510,   -239,
      134,    440,    542,    397,     78,   -270,   -488,   -482,   -260,
       73,    364,    483,    383,    114,   -198,   -411,   -433,   -262,
       21,    284,    410,    349,    134,   -131,   -327,   -369,   -245,
      -17,    207,    328,    298,    137,    -76,   -244,   -294,   -212,
      -41,    138,    244,    235,    124,    -34,   -165,   -214,   -165,
      -48,     80,    162,    166,     97,     -8,    -98,   -136,   -110,
      -41,     38,     90,     95,     59,      4,    -44,    -64,    -53,
      -22,     11,     29,     30,     17,      2,     -5,     -3,      3,
        5,     -2,    -16,    -27,    -26,     -9,     18,     44,     51,
       34,     -3,    -45,    -70,    -65,    -27,     28,     74,     89,
       63,      5,    -59,   -100,    -95,    -46,     27,     90,    114,
       86,     17,    -61,   -114
};
     
 Int16 Temp[256] = {
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0
};

/*****************************************************************************/
/* DSB()                                                                     */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* Perform DSB modulation by multiplying input by sinewave.                  */
/*                                                                           */
/*                                                                           */
/* PARAMETER 1: Audio input.                                                 */ 
/* PARAMETER 2: Carrier signal. Frequency fc                                 */
/*                                                                           */
/*****************************************************************************/

static int DSB( int input, int fc)
{ 
  signed long result;
  Int16 carrierwave;
  /* For TMS320C5505 it is necessary to first cast one int to long before */
  /* carrying out the multiplication                                      */
  carrierwave = generate_sinewave_1 (fc, 32767); 			// The amplitude of fc sine wave 1.000 = 32767
  result = Amp * ( ( (long) input * carrierwave) >> 15);    // 
  return ( (int) result);
}

/*****************************************************************************/
/* Filter()                                                                  */
/*---------------------------------------------------------------------------*/
/* Apply VSB Filter on DSB signal to obtain VSB modulation.                  */
/* PARAMETER 1: DSB Signal.                                                  */ 
/*****************************************************************************/

static int Filter(int input)
{	signed long result = 0;
	int i;
	//Dich cac gia tri cu di 1 don vi
	for (i = 255  ; i >= 0 ; i--)
	{
		result = result + (long)BP[i+1] * (long)Temp[i];
		Temp[i+1] = Temp[i];
	}
	result = result + (long)input * (long)BP[0];
	result = result >> 15;
	Temp[0] = input;
    return ( (int) result);
}


/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  main( )                                                                 *
 *                                                                          *
 * ------------------------------------------------------------------------ */

void main( void ) 
{	
	Uint16 *px;
	Int16 input[N],output[N];
	int i = 0;
	
    /* Initialize BSL */
    USBSTK5515_init( );

	/* Initialize Phase Locked Loop (PLL) */
    pll_frequency_setup(100);

    /* Initialise hardware interface and I2C for code */
    aic3204_hardware_init();
    
    /* Initialise the AIC3204 codec */
	aic3204_init(); 
	
	/* Setup sampling frequency and 30dB gain for microphone */
    set_sampling_frequency_and_gain(SAMPLES_PER_SECOND, GAIN_IN_dB);

    puts("\nDieu che VSB Song mang 5 kHz");


	while(1) {
		aic3204_codec_read(&left_input, &right_input); // Configured for one interrupt per two channels.
		
		left_output = DSB(right_input, 5000);  // DSB modulating with carrier frequency 5 kHz
		left_output = Filter(left_output);     // Bandpass Filter -> VSB modulating 
		right_output = right_input;	           // Directly connect input to output
		
		if(i < N ) {
			input[i] = right_output;
			output[i] = left_output;
			i++;
		} else {
			px = &input[0];
			ExportFile( N, (Uint16*) px, 0) ;  // Export input signal to binary file
			px = &output[0];
			ExportFile( N, (Uint16*) px, 1) ;  // Export VSB output signal to binary file
			}

		aic3204_codec_write(left_output, right_output);
	}
}

/*****************************************************************************/
/* End of main.c                                                             */
/*****************************************************************************/

