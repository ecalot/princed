#include <cstdlib>
#include <iostream>

#include "level.h"

using namespace std;

int main(int argc, char *argv[])
{

try {
	Level l("lev\\level001.plv");

	int x,y;

  cout<<"Done";
} catch (int error) {
	cout << "error number "<<error<<endl;
}

cout<<"bye bye"<<endl;


}
