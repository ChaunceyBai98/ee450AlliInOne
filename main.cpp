#include <arpa/inet.h>
#include <iostream>

using namespace std;
int main() {
    std::cout << "Hello, World!" << std::endl;
    cout<< htons(32)<<endl<<htons(htons(32))<<endl;
    return 0;
}
