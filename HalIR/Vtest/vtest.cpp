#include <iostream>
#include <iomanip>
//#include "voigt.h"
#include "voigt_o.h"

using namespace std;

int main()
{
    int N=50000;
    double *mu = new double[N];
    double xmin=-300;
    double xmax=300;
    double step=(xmax-xmin)/(N-1);
    for(int i=0;i<N;i++)
        mu[i]=xmin+i*step;
    for(int i=0;i<N;i++)
        cout << setw(20) << setprecision(16) <<  mu[i] << " " << voigt(mu[i],0.1) << endl;
    delete [] mu;
    return 0;
}
