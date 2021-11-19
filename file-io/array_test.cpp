#include <iostream>
using std::cout;
using std::endl;


int main() {
  static char RIFF_ID[4] = {'R', 'I', 'F', 'F'};

  int magic = (*(long *)RIFF_ID);
    
  cout << *RIFF_ID <<  magic << (long) 'R' << endl;
  return 0;
} 
