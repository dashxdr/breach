#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

unsigned char output[0x200000], *op = output;

unsigned char buffer[0x100000];

int g4(int o) {
	return (buffer[o]<<24) | (buffer[o+1]<<16) | (buffer[o+2]<<8) | buffer[o+3];
}

int getfile(char *name) {
	int fd = open(name, O_RDONLY);
	if(fd<0) {
		printf("Can't open %s\n", name);
		exit(-1);
	}
	int len = read(fd, buffer, sizeof(buffer));
	close(fd);
	return len;
}


int main(int argc, char **argv) {
	char line[1024];
	if(argc!=3) {
		printf("Use: %s <listfile> <outputRomfile>\n", argv[0]);
		return -1;
	}
	FILE *infile = fopen(argv[1], "r");
	if(!infile) {
		printf("Couldn't open listfile %s\n", argv[1]);
		return -1;
	}
	int count = 0;
	while(fgets(line, sizeof(line), infile)) {
		char *p=line;
		while(*p) {
			if(*p=='\n' || *p==' ' || *p=='\t') {
				*p=0;
				break;
			}
			++p;
		}
//		printf("%s\n", line);
		int len = getfile(line);
		if(count==0) {
			len = g4(28)*4;
			memmove(buffer, buffer+32, len);
		}
		printf("%06lx:%-20s %06x\n", (op-output), line, len);
		if(count) {
			len+=4;
			*op++ = len>>24;
			*op++ = len>>16;
			*op++ = len>>8;
			*op++ = len;
			len-=4;
		}
		memcpy(op, buffer, len);
		op+=len;
		++count;
	}
	fclose(infile);
	printf("Total size %06lx\n", op-output);
	int fd = open(argv[2], O_WRONLY|O_CREAT|O_TRUNC, 0644);
	int len = write(fd, output, op-output);len=len;
	close(fd);
	return 0;
}

