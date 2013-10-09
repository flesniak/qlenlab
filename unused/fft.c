#define _GNU_SOURCE

#define COUNT 200

#include <fftw3.h>
#include <math.h>
#include <stdio.h>

int main() {
  double input[COUNT];
  double delta = 0.001; //1ms zwischen samples, willkürlich,
  printf("sinuswerte:\n");
  for(unsigned int i = 0; i < COUNT; i++) {
    //input[i] = sin((double)2*M_PI*i/10); //alle 20 i eine periode -> delta = 1ms -> T = 20ms -> f = 50Hz
    input[i] = sin((double)2*M_PI*i/10) + sin((double)2*M_PI*i/2.5); // 50Hz + 200Hz
    printf("sin(2pi*%d/10) = %f\n",i,input[i]);
  }

  double output[COUNT];
  for(unsigned int i = 0; i < COUNT; i++)
    output[i] = 0;
  fftw_plan plan = fftw_plan_r2r_1d(COUNT,input,output,FFTW_R2HC,FFTW_ESTIMATE);
  if( plan == 0 ) {
    printf("plan ist null\n");
    return 1;
  }
  fftw_execute(plan);

  printf("ffw-werte:\n");
  for(unsigned int i = 0; i < COUNT; i++) {
    double f = (double)i/COUNT/delta/2; //frequenz berechnen
    printf( "fft[%d = %fHz] = %f\n", i, f, fabs(output[COUNT-i]*2/COUNT) ); //r2hc fft ist "umgekehrt", betrag anpassen!
  }

  fftw_destroy_plan(plan);
  fftw_cleanup();

  return 0;
}
