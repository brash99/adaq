#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <iomanip>
using namespace std;

int main() {

  ifstream delta;
  ofstream calibs;

  char buff[5];
  float value;

  delta.open("delta.txt");
  calibs.open("calibration.txt");

  string firstline;
  getline(delta, firstline);
  //calibs << firstline << endl;

  for(int j=0; j<14; j++){
  // while (delta.good()){
  for (int i=0; i<17; i++){
    delta >> buff;
    value = atoi(buff) + 0.0;

    if (i>0){
      if (value > 0) {value = 200.0/value;} else {value=0;}
      calibs << left << setw(9) << value;
    } 
    else { 
      calibs << left << setw(3) << value;
    }
    }
  calibs << endl;
  delta.ignore(256,'\n');
  }

  delta.close();
  calibs.close();
}

