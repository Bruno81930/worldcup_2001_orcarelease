#include <iostream>

namespace communication {
    class Base72 {
    public:
	int get72Val(char orcChar);
	char get72Char(int decVal);
	char * decToBase72(long dec);
	long base72ToDec(char * orc);
    };

}
