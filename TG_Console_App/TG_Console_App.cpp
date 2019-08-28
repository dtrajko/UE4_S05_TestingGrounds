// TG_Console_App.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu
//
// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

#include "pch.h"
#include <iostream>
#include <string>

template<class T>
T GetMax(T a, T b)
{
	if (a > b) {
		return a;
	}
	else {
		return b;
	}
}

template<>
std::string GetMax(std::string a, std::string b)
{
	if (a.length() > b.length()) {
		return a;
	}
	else {
		return b;
	}
}

template<class MyT>
MyT AddThree(MyT a, MyT b, MyT c)
{
	return a + b + c;
}

template<class T>
std::string PrettyString(T objectToPrettify)
{
	return "int: " + std::to_string(objectToPrettify);
}

template<>
std::string PrettyString(double objectToPrettify)
{
	return "double: " + std::to_string(objectToPrettify);
}

int main()
{
	std::cout << "GetMax<float> result: " << GetMax(1.3f, 2.6f) << std::endl;
	std::cout << "GetMax<char[]> result: " << GetMax("short", "looooooong") << std::endl;
	std::cout << "GetMax<std::string> result: " << GetMax(std::string("short"), std::string("looooooong")) << std::endl;
	std::cout << "AddThree<double> result: " << AddThree(4.1, 1.3, 3.4) << std::endl;
	std::cout << "AddThree<int> result: " << AddThree(3, 5, 7) << std::endl;
	std::cout << "PrettyString<int> result: " << PrettyString(11) << std::endl;
	std::cout << "PrettyString<double> result: " << PrettyString(3.5) << std::endl;
}
