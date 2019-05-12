#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>

#define N 2048
#define ZERO (double)(0.0)
#define THREE (double)(3.0)

// 問題(1)
// 参照方向の変更
// a[i][k], b[k][j], c[i][j]
// 配列のアクセス方向はメモリが連続する方向
// 添字の変化は最も右側のものから行うようにする

// 結果
// time =   78.60498
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

  static double tmp;

  // c[i][j]の計算
  for (i = 0; i < N; i++){
    for (j = 0; j < N; j++){
      c[i][j] = ZERO;
    }

    for (k = 0; k < N; k++){
      tmp = a[i][k];
      for (j = 0; j < N; j++){
      	c[i][j] = c[i][j] + tmp*b[k][j]/THREE;
      }
    }
  }

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
