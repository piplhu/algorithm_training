#include "sort.h"

#include <stdio.h>

void Swap(int *a, int *b) {
  int tmp = *a;
  *a = *b;
  *b = tmp;
}

void MPSort(int *array, int size) {
  for (int i = 0; i < size - 1; i++) { //每轮循环找出第i+1大的数
    for (int j = 0; j < size - i - 1; j++) {
      if (array[j] > array[j + 1])
        Swap(&array[j], &array[j + 1]);
    }
  }
}

/**
 * @brief 用end索引处的做哨兵左边小于哨兵右边大于哨兵，并返回哨兵最后的索引
 *
 * @param array
 * @param begin
 * @param end
 *
 * @return 
 */
int FindIndex(int *array, int begin, int end) {
  int sentry = array[end];
  int start = begin - 1;
  for (int i = begin; i < end; i++) {
    if (array[i] < sentry) {
      ++start;
      Swap(&array[start], &array[i]);
    }
  }
  Swap(&array[start + 1], &array[end]);
  return start;
}

void QSortWork(int *array, int begin, int end) {
  if (begin < end) {
    int index = FindIndex(array, begin, end);
    QSortWork(array, begin, index - 1);
    QSortWork(array, index + 1, end);
  }
}

void QSort(int *array, int size) {
  int begin = 0, end = size - 1;
  QSortWork(array, begin, end);
}

void Prinf(int *array, int size) {
  for (int i = 0; i < size; i++) {
    printf("%d ", array[i]);
  }
  printf("\n");
}
