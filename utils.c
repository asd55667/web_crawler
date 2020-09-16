#include "utils.h"


void error(const char *s)
{
	perror(s);
	exit(EXIT_FAILURE);
}

void file_error(char *s, const char op)
{
	fprintf(stderr, "Failed to %c file: %s\n", op, s);
	exit(EXIT_FAILURE);
}

void alloc_error(const char *s, size_t size)
{
	fprintf(stderr, "fatal: memory exhausted (%s error of %ld bytes).\n", s, size);
	exit(EXIT_FAILURE);
}

void *xmalloc(size_t size)
{
	void *p = malloc(size);
	if(!p)
		alloc_error("Malloc", size);
	return p;
}

void *xcalloc(size_t nitem, size_t size)
{
	void *p = calloc(nitem, size);
	if(!p)
		alloc_error("Calloc", size);
	return p;
}

void *xrealloc(void *p, size_t size)
{
	p = realloc(p, size);
	if(!p)
		alloc_error("Realloc", size);
	return p;
}


char *fgetls(FILE *fp){
    // pointer locate to file end
    fseek(fp, 0L, SEEK_END);
    // total length of file.txt
    long length = ftell(fp);
    // reset pointer for file head
    rewind(fp);
    
    char *whole_text = calloc(1, length + 1);
    if(1 != fread(whole_text, length, 1, fp)){
        free(whole_text);
        fprintf(stderr, "fread failed");
        exit(1);
    }
    return whole_text;
}

char *fgetl(FILE *fp){
    if(feof(fp)) return 0;
    int size = 512;
    char *line = xmalloc(size * sizeof(char));
    if(!fgets(line, size, fp)){
        free(line);
        return 0;
    }
    
    size_t cur = strlen(line);
    // when alloc size insufficient, 2 times realloc
    while((line[cur-1] != '\n') && !feof(fp)){
        if(cur == size - 1){
            size *= 2;
            line = xrealloc(line, sizeof(char) * size);
        }
        int read_size = size - (int)cur;
        // overflow
        if(read_size > INT_MAX) read_size = INT_MAX - 1;
        fgets(&line[cur], read_size, fp);
        cur = strlen(line);
    }
    
    // delete \r\n
    if(cur >= 2)
        if(line[cur - 2] == 0x0d) line[cur - 2] = 0x00;
    if(cur >= 1)
        if(line[cur - 1] == 0x0a) line[cur - 1] = 0x00;
    return line;
}
