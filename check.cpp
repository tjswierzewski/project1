#include <string>
#include <iostream>
#include <fstream>
#include "ap.hpp"

using namespace std;

ap_int<256> evaluate(string txpr);
/**
 * Function for debugging evaluate.cpp
 */
int main(int argc, char const *argv[])
{
    ifstream myfile;
    string problem;
    myfile.open("problem.txt");
    getline(myfile, problem);

    ap_int<1024> answer = evaluate(problem);
    cout << string(answer) << endl;

    return 0;
}