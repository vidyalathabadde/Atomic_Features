#include "iostream"
#include "stdlib.h"

int main(){
    int n = 100;
    double * data = (double *)malloc( n * sizeof(double));
    for( int x = 0; x < n; ++x){
	    data[x] = x;	    
    }

    double readSum = 0.0;
    double writeSum = 0.0;
    double captureSum = 0.0;
    double updateSum = 0.0;

    // the atomic construct prevents reading the value while a gang/worker/vector is writing and vice versa
    // this is the read clause read the value of one variable into another variable
#pragma omp target teams loop map(tofrom:data[0:n]) map(from:readSum)
    for(int x = 0; x < n; ++x){
        if(data[x] >= n/2){
#pragma omp atomic read
            readSum = x;
        }
    }

    // the atomic construct prevents reading the value while a gang/worker/vector is writing and vice versa
    // this is the write clause that only allows a direct write to a variable from a expression
#pragma omp target teams loop map(tofrom:data[0:n]) map(from:writeSum)
    for(int x = 0; x < n; ++x){
        if(data[x] >= n/2){
#pragma omp atomic write
            writeSum = x*2 + 1;
        }
    }

    //this is the capture clause that the update to the expression into another variable
#pragma omp target teams loop map(tofrom:data[0:n]) map(from:captureSum)
    for(int x = 0; x < n; ++x){
        if(data[x] >= n/2){
#pragma omp atomic capture
            captureSum = data[x]--;
            }
    }

    std::cout << captureSum << std::endl;

    //this is the update clause which locks the update of a particualar variable from certain operations
#pragma omp target teams loop map(tofrom:data[0:n]) map(from:updateSum)
    for(int x = 0; x < n; ++x){
        if(data[x] >= n/2){
#pragma omp atomic update
    	    updateSum++;
        }
    }
    return 0;
}

