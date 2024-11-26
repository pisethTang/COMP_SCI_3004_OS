#include <iostream>
#include <string.h>

bool checkSimpleCase(std::string s, std::string goal){
  if (s.size() != goal.size()) return false;
  if (s == goal) return true;
  
  // At this stage, s and goal are of the same sizes.
  for (int i=0; i<s.size(); i++){
    if (s[i] != goal[i]) return false;
  }


  return true;
}



bool rotateString(std::string s, std::string goal){
  // Check for simple trivial (edge) cases.
  if (checkSimpleCase(s, goal) == true) return true;
  // else 


}


int main(int argc, char* argv[]){
  std::string s = "abcde";
  std::string goal1 = "bcdea";
  std::string goal2 = "abced";
  





  return 0;
}
