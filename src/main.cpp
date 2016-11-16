#include <iostream>
#include "NMF.hpp"
using namespace std;


// TEST


int main() {

    //Creating Instance
    NMF<double> nmf;

    //Initializing
    nmf._init(2,4,3);

    //Creating and Setting base matrix
    double** data = new double*[2];
    for(int i = 0;i < 2;i++)
    {
        data[i] = new double[4];
        for(int j = 0;j < 4;j++)
        {
            data[i][j] = double(i+j+1);
            cout << data[i][j] << ",";

        }
        cout << "" << endl;
    }
    nmf._set_baseMatrix(data);

    //Creating and Setting template data
    double* temp_data = new double[2];
    temp_data[0] = 0.0;
    temp_data[1] = 2.0;
    nmf._setDictionary(0,temp_data);
    temp_data[0] = 1.0;
    temp_data[1] = 6.0;
    nmf._setDictionary(1,temp_data);
    temp_data[0] = 11.0;
    temp_data[1] = 10.0;
    nmf._setDictionary(2,temp_data);

    //Starting Separation.
    nmf._separate_with_template(2000000000,nmf.EUC,1.0e-9);
    // nmf._separate_without_template(2000000000,nmf.EUC,1.0e-9);

    //Printing Approximate Matrix
    nmf._print_approx();

    return 0;

}
