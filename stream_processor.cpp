#include <iostream>
#include <unordered_map>
#include <string>
#include <sstream>
using namespace std;


int main(){
  int bx, by, bz;
  int px, py, pz;
  char comma;
  cin >> bx >> comma >> by >> bz >> comma >> px >> py >> comma >> pz >> ws;
  string line;
  char symbol;
  string label;
  
  unordered_map<char, string> tag_table;

  while(true){
    getline(cin, line);
    if (line.empty()) break; 
    stringstream line_stream(line);
    line_stream >> symbol >> comma >> label;
    tag_table[symbol] = label;
  }

  for (int k=0; k < bz; k++){
    for (int j=0; j<by; j++){
      getline(cin, line);
      for(int i=0; i<bz; i++){
        cout << i << "," << j << "," << k << ",1,1,1," << tag_table[line[i]] << "\n";
      }
    }

    getline(cin, line);
  }
  return 0;
}
