#ifndef FORK_H
#define FORK_H

#define MAX_INTS 50

void SetUpArray(int *mergeArray, int *count);
void ForkMergeSort(int *mergeArray, int left, int right, int parentPipe[2]);
void ForkMerge(int parentPipe[2], int leftPipe[2], int rightPipe[2],
 int numElements);
#endif
