#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <limits.h>
#include "Fork.h"

void SetUpArray(int *mergeArray, int *count) { 
	int tempNumber = 0;
	while (*count < MAX_INTS && scanf("%d", &tempNumber)) {
		mergeArray[(*count)] = tempNumber;
		(*count)++;
	}
}

void ForkMergeSort(int *mergeArray, int left, int right, int parentPipe[2]) {
   int middle = (left + right) / 2;
   int leftPid, rightPid, leftPipe[2], rightPipe[2], numElements = right - left;

   if ((right - left) > 1) {

      pipe(leftPipe);

      leftPid = fork();
      if (leftPid == 0) {
         close(leftPipe[0]);
         ForkMergeSort(mergeArray, left, middle, leftPipe);
         exit(0); 
      }
      else if (leftPid > 0) {
         pipe(rightPipe);
         close(leftPipe[1]);

         rightPid = fork();
         if (rightPid == 0) {
            close(rightPipe[0]);
            close(leftPipe[0]);
            close(leftPipe[1]);
            ForkMergeSort(mergeArray,  middle, right, rightPipe);
            exit(0);
         }
         else if (rightPid > 0) {
            close(rightPipe[1]);
         }
         else {
            fprintf(stderr, "Bad fork call on right pid\n");
            exit(EXIT_FAILURE);
         }
      }
      else {
         fprintf(stderr, "Bad fork call on left pid\n");
         exit(EXIT_FAILURE);
      }	
   }
   else {
      write(parentPipe[1], &(mergeArray[left]), sizeof(int));
      return;
   }
   waitpid(leftPid, NULL, 0);
   waitpid(rightPid, NULL, 0);
   ForkMerge(parentPipe, leftPipe, rightPipe, numElements);  	
}

void ForkMerge(int parentPipe[2], int leftPipe[2], int rightPipe[2],
 int numElements) {
   int tempL, tempR, leftRead = 0, rightRead = 0;

   leftRead = read(leftPipe[0], &tempL, sizeof(int));
   rightRead = read(rightPipe[0], &tempR, sizeof(int));

   while (leftRead && rightRead) {
      if (tempL <= tempR) {
         write(parentPipe[1], &tempL, sizeof(int));
         leftRead = read(leftPipe[0], &tempL, sizeof(int));
      }
      else {
         write(parentPipe[1], &tempR, sizeof(int));
         rightRead = read(rightPipe[0], &tempR, sizeof(int));
      }
   }
   while (leftRead) {
      write(parentPipe[1], &tempL, sizeof(int));
      leftRead = read(leftPipe[0], &tempL, sizeof(int));
   }
   while (rightRead) {
      write(parentPipe[1], &tempR, sizeof(int));
      rightRead = read(rightPipe[0], &tempR, sizeof(int));
   }

}



