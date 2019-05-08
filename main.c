#include "dsk6713_aic23.h"
#define BUFF_LEN 1024

#define DIV 4

Uint32 fs = DSK6713_AIC23_FREQ_8KHZ;
#define DSK6713_AIC23_INPUT_MIC 0x0015
#define DSK6713_AIC23_INPUT_LINE 0x0011

int up = 0;
int in_loop_index = 0;
int skip_flag = 0;
int buff[BUFF_LEN];
Uint16 inputsource = DSK6713_AIC23_INPUT_LINE;

int filter_in[7];
//double filter_coeff[11] = { 0.0003,   -0.0001,    0.0002,   -0.0002,    0.0002,    0.9998,    0.0002,   -0.0002,    0.0002,   -0.0001,    0.0003};
double filter_coeff[7] = {0.0003,   -0.0001,    0.0001,    0.9999,    0.0001,   -0.0001, 0.0003};
void output(int n) {
	int i, j, sum = 0;

	filter_in[0] = n;

	for (i = 0; i < 7; i++) {
	     sum += filter_coeff[i] * filter_in[i];
	}

	for (j = 6; j > 0; j--) {
		filter_in[j] = filter_in[j - 1];
	}



	output_sample(sum);

}


void main() {

	comm_poll();
	int i;

	DSK6713_DIP_init();

    while(1) {

    	if (DSK6713_DIP_get(0) == 1) {
    		up = 1;
    	} else if (DSK6713_DIP_get(1) == 1) {
    		up = -1;
    	} else {
    		up = 0;
    	}

        buff[in_loop_index] = input_sample();

        if (up == 1) {
            for (i = 0; i < DIV; i++) {
//                output_sample(buff[in_loop_index]);
            	output(buff[in_loop_index]);
            	DSK6713_waitusec(125/DIV);
            }
        } else if (up == -1) {
//        	output_sample(buff[in_loop_index]);
        	output(buff[in_loop_index]);
        	DSK6713_waitusec(125 * DIV);

        } else {
        	output(buff[in_loop_index]);
 //            output_sample(buff[in_loop_index]);
        }

        if (in_loop_index++ == (BUFF_LEN - 1)) {
            in_loop_index = 0;
        }

    }
}
