# include <cmath>
# include <complex>
# include <cstdio>
# include "signal.h"

#define PI 3.14159265

using namespace std;

/////////////////////////////////////////////////////
/* Functions Related to Discrete Fourier Transform */
////////////////////////////////////////////////////
complex<double> *dft_base(int len)
{
  complex<double> w(cos(2*PI/len), sin(2*PI/len)), t(1, 0);
  complex<double> *l = new complex<double>[len];
  cout << w << endl;
  for(int i = 0; i < len; i++){
    cout << t << endl;
    l[i] = t;
    t = t * w;
  }
  return l;
}

// accepts len only power of 2 //
complex<double> *fft_(complex<double> *f, int len, int inverse)
{
  complex<double> *F, *f1, *f2, *F1, *F2, w_n, w;
  if(len == 1){
    F = new complex<double>[1];
    F[0] = f[0];
    return F;
  }
  else{
    F = new complex<double>[len];
    f1 = new complex<double>[len/2];
    f2 = new complex<double>[len/2];
    for(int i = 0; i < len / 2; i++){
      f1[i] = f[2 * i];
      f2[i] = f[2 * i + 1];
    }
    F1 = fft_(f1, len/2, inverse);
    F2 = fft_(f2, len/2, inverse);
    if(inverse) w = complex<double>(cos(2*PI/len), sin(2*PI/len));
    else w = complex<double>(cos(-1*2*PI/len), sin(-1*2*PI/len));
    w_n = complex<double>(1, 0);
    for(int n = 0; n < len/2; n++){
      F[n] = F1[n] + w_n * F2[n];
      F[n + len / 2] = F1[n] - w_n * F2[n];
      w_n *= w;
    }
    delete[] f1;
    delete[] f2;
    delete[] F1;
    delete[] F2;
    return F;
  }
}

complex<double> *fft(complex<double> *f, int len, int inverse)
{
  complex<double> *F = fft_(f, len, inverse);
  if(inverse)
    for(int i = 0; i < len; i++) F[i] /= len;
  return F;
}

complex<double> *dft(complex<double> *f, int len, int inverse)
{
  complex<double> *F = new complex<double>[len];
  complex<double> w, w_n, w_ni;
  if(inverse) w = complex<double>(cos(2*PI/len), sin(2*PI/len));
  else w = complex<double>(cos(-1*2*PI/len), sin(-1*2*PI/len));
  w_n = complex<double>(1, 0);
  for(int i = 0; i < len; i++) F[i] = complex<double>(0, 0);
  for(int n = 0; n < len; n++){
    w_ni = complex<double>(1, 0);
    for(int i = 0; i < len; i++){
      F[n] += f[i] * w_ni;
      w_ni *= w_n;
    }
    if(inverse) F[n] /= len;
    w_n *= w;
  }
  return F;
}

/////////////////////////////
/* DFT functions end here  */
/////////////////////////////

int main()
{
  complex<double> a[16] = {1, 2, 3, 4, 5, 6, 7, 0, 9, 10, 11, 0, 13, 14, 15, 16}, *r;
  r = fft(fft(a, 16, 0), 16, 1);
  for(int i = 0; i < 16; i++){
    if(r[i].imag() < 0) printf("%.5f - %.5fi\n", r[i].real(), -1*r[i].imag());
    else printf("%.5f + %.5fi\n", r[i].real(), r[i].imag());
  }
  return 0;
}
