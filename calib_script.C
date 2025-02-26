#include <iostream>
#include <fstream>
#include <stdio.h>
using namespace std;

float cal_const[14][16];

void calib_init(){
  ifstream calibs;

  char buff[10];
  
  calibs.open("calibration.txt");
  for(int j=0; j<14; j++){
    calibs >> buff; //skips id column
    for(int i=0; i<16; i++){
      calibs >> buff;
      //cout << buff << " ";
      cal_const[j][i] = atof(buff);
      cout << cal_const[j][i] << " ";
    }
    cout << endl;
  }
  calibs.close();
}

void calib_script(int ID, int slot, int chan){
  //t->Print();
  // for (int i=0; i<t->GetEntries(); i++){
  // t->GetEntry(i);
  
   adc[slot][chan] = adc[slot][chan]*cal_const[ID][chan];
    // }
}
