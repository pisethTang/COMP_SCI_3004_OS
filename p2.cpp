/* Write a C/C++ POSIX compliant program that prints the POSIX defined configuration options supported on any given 
 *
 * */


#include <iostream>
#include <unistd.h>



int main(){
  using namespace std;
#ifdef _POSIX_JOB_CONTROL
    cout << "System Supports Job Control feature" << std::endl;
  #else 
    cout << "System does not support job control\n" << endl;
#endif 

#ifdef _POSIX_SAVED_IDS
    cout << "System support saved set-UID and saed set-GID" << endl;
#else 
    cout << "System does not support saved set-UID and saed set-GID" << endl;

#endif 

#ifdef _POSIX_CHOWN_RESTRICTED
    cout << "System support change ownership feature" << endl;
#else 
    cout << "System does not support change ownership feature" << endl;
#endif 
    

#ifdef _POSIX_NO_TRUNC
    cout << "System support Path truncation option:" << endl;
#else 
    cout << "System does not support Path truncation option" << endl;
#endif 
  
#ifdef _POSIX_VDISABLE
    cout << "System supports disable character for files:" << endl;
#else 
    cout << "System does not support disable characters" << endl;
#endif 





  return 0;
}
