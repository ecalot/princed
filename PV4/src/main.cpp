#include <cstdlib>
#include <iostream>
#include <stdio.h>

#include "level.h"

using namespace std;

int main(int argc, char *argv[])
{

try {
	Level l("lev\\level002.plv");

	int w,h;

	h=l.getHeight();
	w=l.getWidth();

	cout<<"h="<<h<<" w="<<w<<endl;

	Tile* t;

	for (int i=0;i<h;i++) {
		for (int j=0;j<w;j++) {
			t=l.getTile(i,j);
			printf("%02X ",t->getCode()&0x1f);
		}
		cout<<endl;
	}



  cout<<"Done";
} catch (int error) {
	cout << "error number "<<error<<endl;
}

cout<<"bye bye"<<endl;


}
