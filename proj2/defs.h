struct stats{
	int no_deposits;
	int no_withdrawals;
	int no_rejected;
	int balance;
};	

struct parameters{
	int thread_number;
	int loop_count;
	char* file;
};