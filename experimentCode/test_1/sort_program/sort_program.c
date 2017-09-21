/*
 * sort_program.c
 *
 *  Created on: 2015-10-28
 *      Author: Q.
 */
#include <stdio.h>
#include <stdlib.h>

extern  int sort (int  *buf,int  buflen);

void  showdata (int  *buf, int  buflen) {
	int i;

	printf("buffer data:\n");

	for(i = 0; i < buflen; i++) {
		printf("%4d", *(buf + i));
	}

	printf("\n");
}

int main() {
	int  data;
    int  buffer[256];
    int  bufferlen = 0;

    printf("data input :\n");
    while (1) {
    	scanf("%d", &data);

    	buffer[bufferlen++] = data;
        if((data == -1) || (bufferlen >= 256)) {
        	bufferlen--;
        	break;
        }
    }
    printf("data input end!\n");

    showdata(buffer, bufferlen);
    sort(buffer, bufferlen);
    showdata(buffer, bufferlen);

    return (0);
}
