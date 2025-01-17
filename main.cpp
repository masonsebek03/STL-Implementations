#include <iostream>
#include <vector>
#include "vector.h"

int main()
{
	Vector<int> v;
	v.print_data();

	v.push_back(2);
	v.print();
	v.print_data();

	v.pop_back();
	v.pop_back();

	std::vector<int> temp = { 1 };
	temp.pop_back();
	temp.pop_back();

}