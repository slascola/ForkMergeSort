#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "Fork.h"

int main(int argc, char *argv[]) {
	int mergeArray[MAX_INTS], sorted;;
	int count = 0, parentPipe[2], parentPid;

	printf("Enter up to 50 numbers and press F when done\n");
	SetUpArray(mergeArray, &count);
   if (count == 0) {
      fprintf(stderr, "Not enough numbers inputted\n");
      return 1;
   }

   pipe(parentPipe);
   parentPid = fork();
   if (parentPid == 0) {
      close(parentPipe[0]);
      ForkMergeSort(mergeArray, 0, count, parentPipe);
      
   }
   else if (parentPid > 0) {
      close(parentPipe[1]);
      waitpid(parentPid, NULL, 0);
      printf("Printing sorted numbers...\n");
      while (read(parentPipe[0], &sorted, sizeof(int)) != 0) {
         printf("%d\n", sorted);
      }
   }
   else {
      fprintf(stderr, "Bad fork call on parent\n");
      exit(1);
   }
	return 0;
}
