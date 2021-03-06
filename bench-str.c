/*
 * $Id: bench-str.c,v 0.2 2012/01/13 14:51:39 dankogai Exp dankogai $
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#define EXIT(msg) { perror(msg); exit(errno); }
#define LINESIZE 65536

#include "bucketsort.h"
#include "bucketsort.c"

int cmp(const void *a, const void *b){
    return strcmp(*(char **)a, *(char **)b);
}

int main(int argc, char **argv)
{
    if (argc < 2)
	EXIT("no file");
    FILE *fp = fopen(argv[1], "r");
    if (!fp)
	EXIT(argv[1]);
    char buf[LINESIZE];
    size_t lcur, llen = LINESIZE;
    char **lines = (char **) malloc(llen * sizeof(char *));
    if (!lines)
	EXIT("malloc failed");
    for (lcur = 0; fgets(buf, LINESIZE, fp); lcur++) {
	if (lcur == llen) {
	    llen <<= 1;
	    lines = realloc(lines, llen * sizeof(char *));
	    if (!lines)
		EXIT("malloc failed");
	}
	int slen = strlen(buf);
	if (slen > 0 && buf[slen - 1] == '\n')
	    buf[slen - 1] = '\0';	// do not copy \n
	lines[lcur] = strdup(buf);
	if (!lines[lcur])
	    EXIT("malloc failed");
    }
    size_t i;
#if defined(QSORT)
    qsort(lines, lcur, sizeof(char *), cmp);
#elif defined(MERGESORT)
    mergesort(lines, lcur, sizeof(char *), cmp);
#elif defined(HEAPSORT)
    heapsort(lines, lcur, sizeof(char *), cmp);
#elif defined(RADIXSORT)
    radixsort(lines, lcur, NULL, 0);
#elif defined(SRADIXSORT)
    sradixsort(lines, lcur, NULL, 0);
#elif defined(BUCKETSORT)
    bucketsort((void **) lines, lcur, NULL, NULL, NULL);
#else
#endif
    for (i = 0; i < lcur; i++)
	printf("%s\n", lines[i]);
    {
	for (i = 0; i < lcur; i++)
	    free(lines[i]);
	free(lines);
    }
    return 0;
}
