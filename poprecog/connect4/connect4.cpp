#include <cstdlib>
#include <iostream>

#include "boards.c"

using namespace std;

int getBit(unsigned long long number, int bit) {
	unsigned long long tmp = 1;

	if (!((bit>=0)&&(bit<=63)))
	  return -1;
	while(bit--)
	  tmp*=2;
	//cout << "[number=" << number << ", mask=" << tmp << " " << "]";
	tmp&=number;
	//cout << "XX:" << tmp;
	return tmp!=0;
}

void displayBoard(classBoard board) {
	int x, y;

	cout << "\n";
	for (y=3; y>=0; y--) {
		for (x=0; x<4; x++) {
			if (getBit(board.board[0], x+y*4)==1)
				cout << "W ";
			else if (getBit(board.board[1], x+y*4)==1)
				cout << "B ";
			else
			  cout << ". ";
		}
		cout << "\n";
	}
	cout << "\n";
}

int main(int argc, char *argv[])
{
	classBoard board;
	int move;
	int i;
	
	board.clear();
	
	//board.putToken44(0);
	//board.putToken44(0);
	//board.putToken44(0);
	//board.putToken44(1);
	//board.putToken44(2);
	//board.putToken44(2);
	//board.putToken44(0);
	//board.actualPlayer = !board.actualPlayer;
	/*displayBoard(board);
	board.removeToken44(2);
	displayBoard(board);
	
	system("pause");
	return 0;*/

	while (1) {
		displayBoard(board);
		for (i = 1; i < 11; i++) {
			board.operationsNumber = 0;
			cout << i << ") evaluate: " << board.miniMax44(15) << " operationsNumber: " << board.operationsNumber << endl;
		}
		/*for (i=0; i<4; i++) {
			short res;
			res = board.putToken44(i);
			if (res == -2) {
				cout << "  ";
				continue;
			}
			if (res == 2) {
				board.actualPlayer=!board.actualPlayer;
				board.removeToken44(i);
				cout << "W!";
				continue;
			}
			switch(board.miniMax44(1)) {
				case 2: cout << "L "; break;
				case 1: cout << "D "; break;
				case -2: cout << "W "; break;
			}
			board.removeToken44(i);
		}*/
		cout << endl << endl;
		cout << board.board[0] << " & " << board.board[1] << endl;
		cin >> move;
		switch (board.putToken44(move)) {
			case -2:
				cout << "busy row\n";
				break;
			case 2:
				cout << "won\n";
				break;
			case 3:
				cout << "#\n";
				break;
			default:
				cout << "nothing\n";
				break;
		}
	}
 	system("PAUSE");
	return EXIT_SUCCESS;
}

/*
inline int classBoard::miniMax44(int depth) {
	register short move;
	register short best = 0;
	register short res;

	if (!depth)
	  return 1;

	for (move=0; move<4; move++) {
		res = putToken44(move);
		if (res == -2)
		  continue;
		if (res == 2) {
			removeToken44(move);
 			actualPlayer = !actualPlayer;
			return 2;
		}
		switch (miniMax44(depth-1)) {
			case -2: // won
				removeToken44(move);
				return 2;
			case 2: // lose
				if (!best)
				  best = -2;
			default: //draw
				best = 1;
		}
		removeToken44(move);
		return best;
	}
}
*/
