class classBoard {
	public:
	unsigned long long board[2];
	int actualPlayer;
	unsigned long long operationsNumber;

	void clear() {
		board[0] = 0;
		board[1] = 0;
		actualPlayer = 0;
		operationsNumber = 0;
	};
	int putToken44(int column);
	int removeToken44(int column);
	int miniMax44(int depth);
};
