#include <cstdlib>
#include <iostream>
#include <stdio.h>

#include "level.h"

using namespace std;

int main(int argc, char *argv[])
{

try {
	Level l("lev/level2.plv");

	int w,h;

	h=l.getHeight();
	w=l.getWidth();

	cout<<"h="<<h<<" w="<<w<<endl;

	Tile* t;

	tPos pos;
	
	for (pos.floor=0;pos.floor<h;pos.floor++) {
		for (pos.col=0;pos.col<w;pos.col++) {
			t=l.getTile(pos);
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
