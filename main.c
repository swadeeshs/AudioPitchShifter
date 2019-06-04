#include "dsk6713_aic23.h"
#define BUFF_LEN 8192 // Buffer length
#define N 7 


Uint32 fs = DSK6713_AIC23_FREQ_8KHZ;
#define DSK6713_AIC23_INPUT_MIC 0x0015
#define DSK6713_AIC23_INPUT_LINE 0x0011


Uint16 inputsource = DSK6713_AIC23_INPUT_LINE;

int ring_buffer[BUFF_LEN] = 0; // Circular/Ring buffer

float index;

float pitch_up = 0.85; 
float pitch_down = 1.25;
float no_change = 1;
float delta;

int write_addr = 0; // Write address
int read_addr = 0; // Read address


int echo_ptr1 = 0; // Echo read address 1
int echo_ptr2 = BUFF_LEN / 2; // Echo read address 2

int filter_in[N];

double filter_coeff[N] = {0.0003,   -0.0001,    0.0001,    0.9999,    0.0001,   -0.0001, 0.0003}; // Filter coefficients

//  FIR filter

void output(int n) {
	int i, j, sum = 0;

	filter_in[0] = n;

	for (i = 0; i < N; i++) {
	     sum += filter_coeff[i] * filter_in[i];
	}

	for (j = N-1; j > 0; j--) {
		filter_in[j] = filter_in[j - 1];
	}

	output_sample(sum);
}


void main() {

	comm_poll();


	DSK6713_DIP_init();

    while(1) {

    	ring_buffer[write_addr] = input_sample();
    	write_addr++;
    	write_addr %= BUFF_LEN;

    	if (DSK6713_DIP_get(3) == 0) {
    		if (DSK6713_DIP_get(0) == 1) {
    			delta = pitch_down;
    		} else if (DSK6713_DIP_get(1) == 1) {
    			delta = pitch_up;
    		} else {
    			delta = no_change;
    		}

    	    output(ring_buffer[read_addr]);
    	    index = index + delta;
    	    read_addr = (int)index % BUFF_LEN;

    	} else {


    		int echo = 0.9 * ring_buffer[echo_ptr1] + 0.3 * ring_buffer[echo_ptr2];

    		output(echo >> 14);

    		echo_ptr1++;
    		echo_ptr2++;

    		echo_ptr1 %= BUFF_LEN;
    		echo_ptr2 %= BUFF_LEN;

    	}
    }
}
