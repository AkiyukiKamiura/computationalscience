#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>

#define N 2048
#define ZERO (double)(0.0)
#define THREE (double)(3.0)

// https://www.xlsoft.com/jp/products/intel/perflib/mkl/11.2/mkl_tutorial_f/GUID-36BFBCE9-EB0A-43B0-ADAF-2B65275726EA.htm

// Intel Math Kernel Library を使用して高速化を行う
// TRANSA, TRANSB: 転置しない 'N'
// M, N, K: A[M][K], B[K][N], C[M][N]
// A, B, C: 配列
// LDA, LDB, LDC: 配列のリーディングディメンジョン = 行数
// C = ALPHA*A*B + BETA*C なので ALPHA = 1/3, BETA = 0.0

// 結果
// time =    0.94124
// 184.458017


// for 秒数計測
double getrusage_sec(){

  struct rusage t;
  struct timeval tv;
  getrusage(RUSAGE_SELF, &t);
  tv = t.ru_utime;
  return tv.tv_sec + (double)tv.tv_usec*1e-6;

}

int main(){

  static int i, j, k;
  static double a[N][N], b[N][N], c[N][N], s;
  static double t1, t2;

  srand(1); // 乱数初期化

  // 配列初期化 (0 <= x <= 1) の乱数で初期化
  for (i = 0; i < N; i++){
    for (j = 0; j < N; j++){
      a[i][j] = rand()/(double)RAND_MAX;
      b[i][j] = rand()/(double)RAND_MAX;
    }
  }

  t1 = getrusage_sec();

  static double alpha = 1.0/3.0;
  static double beta = 0.0;
  static int n = N;

  dgemm_("N", "N", &n, &n, &n, &alpha, b, &n, a, &n, &beta, c, &n);

  t2 = getrusage_sec();

  printf("time = %10.5f\n", t2 - t1);

  s = ZERO;

  for (i = 0; i < N; i += 10) {
    for (j = 0; j < N; j += 10){
      if (a[i][j] > s) {
	      s = a[i][j];
      }
      if (b[i][j] > s) {
	       s = b[i][j];
      }
      if (c[i][j] > s) {
	       s = c[i][j];
      }
    }
  }

  printf("%f\n", s);

  return 0;
}
