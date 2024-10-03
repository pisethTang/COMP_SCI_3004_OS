#include <unistd.h>

int balance = 0;


int read_balance(){
    usleep(250000);
    return balance;
}


void write_balance(int new_balance){
    usleep(250000); // Pause the thread 
    // Simulating communication delay that will occur during an ATM or machine or a main network.
    balance = new_balance;
}