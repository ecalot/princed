#include <stdio.h>

int main(int a, char** b) {
	FILE* fp;
	char c,o;
	int i,j,g;
	char *chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890 -:.;,<>\\/*!\"$%&/()=@^[]'\"-_";
	char* s;
	int pos_x[92] = {0, 7, 14, 21, 28, 34, 40, 47, 54, 59, 66, 74, 80,
		89, 96, 103, 110, 117, 124, 131, 138, 145, 152, 161, 168, 175, 184,
		191, 198, 205, 212, 219, 225, 232, 239, 242, 247, 254, 257, 266, 
		273, 280, 287, 294, 301, 308, 314, 321, 328, 337, 344, 351, 358,
		365, 372, 379, 387, 394, 401, 408, 415, 422, 429, 433, 438, 441,
		444, 448, 452, 458, 464, 473, 482, 490, 493, 499, 507, 516, 525,
		534, 539, 544, 549, 556, 563, 568, 573, 578, 582, 588, 593, 600};
	fp=fopen("raw","rb");
	s=chars;
	g=0;
	while (*s) {
		printf("Character '%c':\n/",*s);
		for (i=pos_x[g]+1;i<pos_x[g+1];i++) printf ("-");
		printf("\\\n");
		for (j=2;j<11;j++) {
			printf("|");
			fseek(fp,600*j+pos_x[g]+1,SEEK_SET);
			for (i=pos_x[g]+1;i<pos_x[g+1];i++) {
				c=getc(fp);
				o=c?'O':' ';
				printf ("%c",o);
			}
			printf("|\n");
		}
		printf("\\");
		for (i=pos_x[g]+1;i<pos_x[g+1];i++) printf ("-");
		printf("/\n\n");
		g++;
		s++;
	}
	return 0;
}
