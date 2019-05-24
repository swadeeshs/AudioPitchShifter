#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define passFreq 1500
#define stopFreq 2000
#define sampleFreq 8000
#define As 60

double Op, Os, Wp, Ws, Wc;
int N;
double * co_eff = NULL;

void init() {
	Op = 2 * M_PI * passFreq;
	Os = 2 * M_PI * stopFreq;

	Wp = Op / sampleFreq;
	Ws = Os / sampleFreq;
	Wc = Wp + (Ws - Wp)/2;

	N = int((8 * M_PI) / (Ws - Wp)) + 1;
}

int main() {
	init();

	co_eff = (double *)malloc(N * sizeof(double));

	co_eff[int((N-1)/2)] = (Wc / M_PI);

	for (int i = 1; i <= (N-1)/2; i++) {
		co_eff[i + (N-1)/2] = (sin(Wc * i) / (M_PI * i)) * (0.54 - 0.46 * cos((2 * M_PI * (i + (N-1)/2)) / (N - 1)));
	}

	for (int i = 0; i < int((N-1)/2) ; i++) {
		co_eff[i] = co_eff[N - 1 - i];
	}

	for (int i = 0; i < N ; i++) {
		printf("%d: %f\n", i, co_eff[i]);
	}

	return 0;
} 
