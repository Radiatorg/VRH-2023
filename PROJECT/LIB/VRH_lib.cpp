#include <cstdlib>
#include <iostream>
#include <string.h>

extern "C" {

	void __stdcall _printN(int a)
	{
		std::cout << a << std::endl;
	}

	void __stdcall _printS(char* str)
	{
		if (str!= NULL)
		std::cout << str << std::endl;
	}

	int  __stdcall _pow(int num, int exponent)
	{
		if (exponent < 0) {
			std::cout << "The language does not support fractional numbers";
			return num;
		}
		long long accum;
		accum = pow(num,exponent);

			if (accum > INT_MAX|| accum < INT_MIN)
			{
				std::cout << "Overflow in pow()" << std::endl;
				accum = INT_MIN;
			}
			return accum;
	}
	int __stdcall _abs(int num) {
		if (num < 0 && num != INT_MIN)
			num = num * (-1);
		return num;
	}

	void __stdcall _pause() {
		system("pause");
	}
}