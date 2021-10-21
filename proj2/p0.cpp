#include "my_const.h"
#include "defs.h"
#include "cstdlib"
#include "ctime"
#include "pthread.h"


//
stats checking_account;
stats savings_account;

stats th_checkings[THREAD_COUNT];
stats th_savings[THREAD_COUNT];


void* actionToTake(void*){

	switch(random() % 6){
		case 0:
			
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		default:
			//error occured with random() % 6
			break;
	}


}

int main(int argc,char* argv[]){
	srand(time(NULL));


	int loop_count = atoi(argv[1]);
	pthread_t threads[THREAD_COUNT];

	pthread_attr_t attr;
	pthread_attr_init(&attr);


	for(int i = 0; i < THREAD_COUNT;i++)
		pthread_create(&threads[i],&attr,actionToTake,(void*)loop_count);

	




	return 0;
}