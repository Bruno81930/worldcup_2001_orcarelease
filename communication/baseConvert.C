#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "baseConvert.h"

#define ulong unsigned long

using namespace communication;

int Base72::get72Val(char orcChar)
{

    int addValue = 0;

    if(((orcChar -'0') < 10)&&(orcChar >= '0'))
	return (orcChar - '0');
    addValue += 10;
    
    if(((orcChar -'a') < 26)&&(orcChar >= 'a'))
	return (orcChar - 'a' + addValue);
    addValue += 26;
    
    if(((orcChar -'A') < 26)&&(orcChar >= 'A'))
	return (orcChar - 'A' + addValue);
    addValue += 26;
    

    switch(orcChar) {

    case '(':
	return (addValue + 0);

    case ')':
	return (addValue + 1);

    case '<':
	return (addValue + 2);

    case '>':
	return (addValue + 3);

    case '.':
	return (addValue + 4);

    case '+':
	return (addValue + 5);

    case '*':
	return (addValue + 6);

    case '_':
	return (addValue + 7);

    case '/':
	return (addValue + 8);

    case '?':
	return (addValue + 9);

    }
    
    
}



char Base72::get72Char(int decVal)
{
    
    if(decVal < 10)
	return (decVal + '0');
    decVal -= 10;
    if(decVal < 26)
	return (decVal + 'a');
    decVal -= 26;
    if(decVal < 26)
	return (decVal + 'A');
    decVal -= 26;

    switch(decVal) {
	
    case 0:
	return '(';
	
    case 1:
	return ')';
	
    case 2:
	return '<';
	
    case 3:
	return '>';
	
    case 4:
	return '.';
	
    case 5:
	return '+';
	
    case 6:
	return '*';
	
    case 7:
	return '_';
	
    case 8:
	return '/';
	
    case 9:
	return '?';
    }

}


char * Base72::decToBase72(long dec)
{
    ulong longVal = dec;
    char *orc=new char[5];
    for (int i=4; i>=0; i--)
    {
	orc[i] = get72Char(longVal % 62);
	longVal /= 62;
    }
    return orc;
}

long Base72::base72ToDec(char * orc)
{
    ulong dec = 0;
    long mult = 1;
    for (int i=4; i>=0; i--)
    {
	dec += get72Val(orc[i]) *mult;
	mult *= 62;
    }
    return dec;
}
