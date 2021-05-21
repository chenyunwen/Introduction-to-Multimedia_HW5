# include <stdio.h>
# include <iostream>
# include <cstdlib>
# include <ctime>
# include <random>

using namespace std;

// P00 P01
// P10 P11
double probability[2][2];
bool state = false;         // state now
float error_burst = 0;      // temporary save the number of continuous packet lost each time
float avg_error_burst = 0;  // the average length of error burst
int error_times = 0;        // the total times of the continuous packet lost

int main(){

    probability[0][0] = 0.2;    // P00 Bad -> Bad
    probability[0][1] = 0.8;    // P01 Bad -> Good
    probability[1][0] = 0.3;    // P10 Good -> Bad
    probability[1][1] = 0.7;    // P11 Good -> Good

    double P0 = probability[1][0] / (probability[0][1] + probability[1][0]);
    double P1 = probability[0][1] / (probability[0][1] + probability[1][0]);

    srand( time(NULL) % 100 * time(NULL));
    double x = (double) rand() / (RAND_MAX + 1.0); // get random number between [0,1]

    if(x > P0) state = true;    // start from the Good state
    else state = false;         // start from the Bad state

    for (int i = 0; i < 1000; i++){     // set the total number of packets to be 1000
        if (state){
            // Good now
            srand(rand());
            x = (double) rand() / (RAND_MAX + 1.0);

            if(x > probability[1][0]) state = true; // Good -> Good
            else {
                // Good -> Bad
                state = false;
                error_burst++;
            }
        } else {
            // Bad now
            srand(rand());
            x = (double) rand() / (RAND_MAX + 1.0);

            if(x > probability[0][0]) {
                // Bad -> Good
                state = true;
                if (i == 0) continue;   // if start from the Bad state
                // calculate the average length of error burst now
                avg_error_burst = (avg_error_burst * (float)error_times + (float)error_burst) / (float)(error_times + 1);
                // the total times of the continuous packet lost
                error_times++;
                // cout << "error_burst[" << error_times <<"]: " << error_burst << endl;
                error_burst = 0;
            }
            else {
                // Bad -> Bad
                state = false;
                error_burst ++;
            } 
        }
    }

    // Check if end at the Bad state
    if(state == false){
        avg_error_burst = (avg_error_burst * (float)error_times + (float)error_burst) / (float)(error_times + 1);
        error_times++;
        error_burst = 0;
    }
    cout << "error times: " << error_times << endl;
    cout << "average error burst: " << avg_error_burst << endl; 
    return 0;
}