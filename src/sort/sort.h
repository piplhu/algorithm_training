#ifndef _H_SORT_H_
#define _H_SORT_H_
/**
 * @file sort.h
 * @brief 排序算法
 * @author pipl
 * @version 1.0
 * @date 2020-09-04
 */

/**
 * @brief 打印值
 *
 * @param array
 * @param size
 */
void Prinf(int *array, int size);

/**
 * @brief 冒泡排序
 *
 * @param array
 * @param size
 */
void MPSort(int *array, int size);

/**
 * @brief 快速排序
 *
 * @param array
 * @param size
 */
void QSort(int *array, int size);
#endif //_H_SORT_H_
