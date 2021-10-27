
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <string.h>
#include <iostream>
#include <unistd.h>

#include "ctime"
#include "my_const.h"
#include "defs.h"

using namespace std;
//
stats checking_account; // used for race condition
stats savings_account; //used for race condition

stats th_checkings[THREAD_COUNT];
stats th_savings[THREAD_COUNT];



void* transactions(void* params){
	parameters* th_params = (parameters * ) params;
	int amount = 0;
	
	FILE* oFile = fopen(th_params->file,"w");
	

	int reg;
	for(int i = 0;i < th_params->loop_count;i++){
	
		
		
		
		switch(random() % 6){
			case 0: //Deposit Checking Transactions
				amount = (random() % 50) + 50;

				reg = checking_account.balance;
				reg += amount;
				usleep(rand() % 1000);
				checking_account.balance = reg;

				checking_account.no_deposits++;

				reg = th_checkings[th_params->thread_number].balance;
				reg += amount;
				usleep(rand() % 1000);
				th_checkings[th_params->thread_number].balance = reg;

				th_checkings[th_params->thread_number].no_deposits++;
				printf("DEPOSIT CHECKING %i\n",amount);
				fprintf(oFile,"DEPOSIT CHECKING %i\n",amount);
				
				


				break;
			case 1: //Withraw Checking Transactions
				
				amount = (random() % 50)+ 50;

				if(th_checkings[th_params->thread_number].balance < amount){ 
					
					checking_account.no_rejected++;

					th_checkings[th_params->thread_number].no_rejected++;
					printf("WITHDRAWAL CHECKING %i (REJECTED)\n",amount);
					fprintf(oFile,"WITHDRAWAL CHECKING %i (REJECTED)\n",amount);
					
					break;
				}
				// else it succeeds
				reg = checking_account.balance;
				reg -= amount;
				usleep(rand() % 1000);
				checking_account.balance = reg;

				checking_account.no_withdrawals++;

				reg = th_checkings[th_params->thread_number].balance;
				reg -= amount;
				usleep(rand() % 1000);
				th_checkings[th_params->thread_number].balance = reg;

				th_checkings[th_params->thread_number].no_withdrawals++;
				printf("WITHDRAWAL CHECKING %i\n",amount);
				fprintf(oFile,"WITHDRAWAL CHECKING %i\n",amount);
						
				break;
			case 2: // Deposit into Savings
				
				amount = (random() % 50) + 100;

				//update the shared global variables 
				reg = savings_account.balance;
				reg += amount;
				usleep(rand() % 1000);
				savings_account.balance= reg;

				savings_account.no_deposits++;


				// update the global variable for thread #
				reg = th_savings[th_params->thread_number].balance;
				reg += amount;
				usleep(rand() % 1000);
				th_savings[th_params->thread_number].balance = reg;

				th_savings[th_params->thread_number].no_deposits++;
				printf("DEPOSIT SAVINGS %i\n",amount);
				fprintf(oFile,"DEPOSIT SAVINGS %i\n",amount);				

				break;
			case 3: //Withdraw From Savings
				
				amount = (random() % 50) +100;

				if(th_savings[th_params->thread_number].balance < amount){ // Failed due to insufficient funds

					savings_account.no_rejected++;

					th_savings[th_params->thread_number].no_rejected++;
					printf("WITHDRAWAL SAVINGS %i (REJECTED)\n",amount);		
					fprintf(oFile,"WITHDRAWAL SAVINGS %i (REJECTED)\n",amount);					

					break;
				}	
				// else succeeds
				savings_account.no_withdrawals++;
				
				reg = savings_account.balance;
				reg -= amount;
				usleep(rand() % 1000);
				savings_account.balance = reg;
				
				reg = th_savings[th_params->thread_number].balance;
				reg -= amount;
				usleep(rand() % 1000);
				th_savings[th_params->thread_number].balance = reg;
				
				th_savings[th_params->thread_number].no_withdrawals++;
				printf("WITHDRAWAL SAVINGS %i\n",amount);		
				fprintf(oFile,"WITHDRAWAL SAVINGS %i\n",amount);

				break;
			case 4: //Transfer form checking to savings account 
			
				amount = (random() % 100) +100;

				if(th_checkings[th_params->thread_number].balance < amount){ //Transaction failed (REJECTED)

					checking_account.no_rejected++;

					th_checkings[th_params->thread_number].no_rejected++;
					printf("TRANSFER CHECKING TO SAVINGS %i (REJECTED)\n",amount);
					fprintf(oFile,"TRANSFER CHECKING TO SAVINGS %i (REJECTED)\n",amount);									

					break;
				}
				// else Succeeds

				// withdraw from the checking account
				checking_account.no_withdrawals++;
				reg = checking_account.balance;
				reg -= amount;
				usleep(rand() % 1000);
				checking_account.balance = reg;

				th_checkings[th_params->thread_number].no_withdrawals++;
				reg = th_checkings[th_params->thread_number].balance;
				reg -= amount;
				usleep(rand() % 1000);
				th_checkings[th_params->thread_number].balance = reg;

				//deposit into the savings
				th_savings[th_params->thread_number].no_deposits++;
				reg = th_savings[th_params->thread_number].balance;
				reg += amount;
				usleep(rand() % 1000);
				th_savings[th_params->thread_number].balance = reg;
				savings_account.no_deposits++;

				reg = savings_account.balance;
				reg += amount;
				usleep(rand() % 1000);
				savings_account.balance = reg;

				

				printf("TRANSFER CHECKING TO SAVINGS %i\n",amount);
				fprintf(oFile,"TRANSFER CHECKING TO SAVINGS %i\n",amount);	

				break;
			case 5: //Transfer from savings to checking
				
				amount = (random() % 100)+ 100;

				if(th_savings[th_params->thread_number].balance < amount){ //Transaction fails

					savings_account.no_rejected++;
					th_savings[th_params->thread_number].no_rejected++;

					printf("TRANSFER CHECKING TO SAVINGS %i (REJECTED)\n",amount);
					fprintf(oFile,"TRANSFER CHECKING TO SAVINGS %i (REJECTED)\n",amount);	

					break;
				}
					// else transaction succeeds
 
				//withdraw from savings account
				reg = savings_account.balance;
				reg -= amount;
				usleep(rand() % 1000);
				savings_account.balance = reg;

				savings_account.no_withdrawals++;
				
				
				reg = th_savings[th_params->thread_number].balance;
				reg -= amount;
				usleep(rand() % 1000);
				th_savings[th_params->thread_number].balance = reg;

				th_savings[th_params->thread_number].no_withdrawals++;
				
				//deposit into checking

				reg = checking_account.balance;
				reg += amount;
				usleep(rand() % 1000);
				checking_account.balance = reg;

				checking_account.no_deposits++;

				
				reg = th_checkings[th_params->thread_number].balance;
				reg += amount;
				usleep(rand() % 1000);
				th_checkings[th_params->thread_number].balance = reg;
				th_checkings[th_params->thread_number].no_deposits++;

				printf("TRANSFER CHECKING TO SAVINGS %i\n",amount);
				fprintf(oFile,"TRANSFER CHECKING TO SAVINGS %i\n",amount);

				break;
			default:
				//error occured with random() % 6
				break;
		}

	

	}

}

int main(int argc,char* argv[]){

	if(argc != 2){
		printf("To run: %s loop_count\n",argv[0]);
		exit(1);
	}

	


	srand(time(NULL));


	int loop_count = atoi(argv[1]);
	if(loop_count == 0){
		printf("To run: %s loop_count\n", argv[0]);
		exit(1);
	}

	parameters* th_params[THREAD_COUNT]; 
	for(int i =0;i<THREAD_COUNT;i++){
		th_checkings[i].balance = 0;
		th_savings[i].balance = 0;
		th_params[i] = new parameters;
	}


	pthread_t threads[THREAD_COUNT];

	pthread_attr_t attr;
	pthread_attr_init(&attr);


	for(int i = 0; i < THREAD_COUNT;i++){
		th_params[i]->loop_count = loop_count;
		th_params[i]->file = new char[1024];
		th_params[i]->thread_number = i;
		string fileName = "unsynchronized_th" + to_string(i) + "output.txt";
		cout << "th" + to_string(i) << endl;
		
		strcpy(th_params[i]->file,fileName.c_str());
		pthread_create(&threads[i], &attr, transactions, (void*) th_params[i]);
		
	}

	

	
		


	//waits for all threads to be done.
		

		pthread_join(threads[0],NULL);
		pthread_join(threads[1],NULL); 
		pthread_join(threads[2],NULL); 
		pthread_join(threads[3],NULL); 
		pthread_join(threads[4],NULL); 
		pthread_join(threads[5],NULL); 
		pthread_join(threads[6],NULL); 
		pthread_join(threads[7],NULL); 
		pthread_join(threads[8],NULL); 
		pthread_join(threads[9],NULL); 

	cout << endl << "Balance of checking_account: $" << checking_account.balance << endl;
	cout << "Balance of savings_account: $" << savings_account.balance << endl;


	int saving_thSum = 0;
	int checking_thSum = 0;
	for(int i = 0;i<THREAD_COUNT;i++){
		checking_thSum += th_checkings[i].balance;
		saving_thSum += th_savings[i].balance;
	}	

		if(checking_account.balance != checking_thSum || saving_thSum != savings_account.balance){
			cout << "The race condition occured" << endl;
			cout << "Sum of all th_checking: $" << checking_thSum << endl;
			cout << "Sum of all th_saving: $" << saving_thSum << endl;
		}else{
			cout << "The race condition did not occur." << endl;
			cout << "Sum of all th_checking: $" << checking_thSum << endl;
			cout << "Sum of all th_saving: $" << saving_thSum << endl;	
		}






		for(int i = 0;i<THREAD_COUNT;i++){
		delete[] th_params[i]->file;
		delete th_params[i];
		}
		
	return 0;
}