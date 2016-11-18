#include <iostream>
#include "string.h"

int main() {
	String str = "some text"; 
	str[0] = 'T';    
    std::cout << str.size() << std::endl;

	for (auto it = str.begin();it != str.end();++it) {
		std::cout << *it;
	}

    for(const char& c : str) {
    std::cout << c;
     }
    
    String str1 = "Hello";
    String str2 = "world";
    String str3 = str1 + " " + str2;
    std::cout<<std::endl<<str3<<std::endl;

	return 0;
}
