#include <stdio.h>
#include <stdlib.h>

int main(int c, char** files) {
	char* def;
	char* index;
	FILE* fpd;
	FILE* fpi;
	char filename[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	char filenameaux[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	unsigned short int i,j;
	unsigned short int numbers[500];
	
	if (c!=3) {
		fprintf(stderr,"Syntax: %s file.def file.index\n",files[0]);
		exit(1);
	}

	def=files[1];
	index=files[2];

	//Start processing the file

	//Open files
	fpd=fopen(def,"rt");
	if (fpd==NULL) {
		fprintf(stderr,"Definition file (%s) not found\n",def);
		exit(1);
	}
	fpi=fopen(index,"wb");
	if (fpi==NULL) {
		fprintf(stderr,"Index file (%s) not saved\n",index);
		fclose(fpd);
		exit(1);
	}

	//process filename
	fgets(filename,sizeof(filename),fpd);
	for(i=0,j=0;filename[i];i++) {
		if (filename[i]==' ') {
			j=14;
			continue;
		}
		if (filename[i]=='\n') break;
		filenameaux[j]=filename[i];
		j++;
	}
	fwrite(filenameaux,1,28,fpi);

	//resd numbers
	i=0;
	while (!feof(fpd)) {
		fgets(filename,sizeof(filename),fpd);
		numbers[i]=atoi(filename);
		i++;
	}
	i--; //ignore feof
	
	i--; //the first number is the type
	
	//write numbers
	fwrite(&i,1,2,fpi);
	i++;
	fwrite(numbers,i,2,fpi);

	//end files
	fclose(fpi);
	fclose(fpd);

	exit(0);
}

