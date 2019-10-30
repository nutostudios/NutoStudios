#include "String.h"

//CONSTRUCTORS
String::String() : string_mem_size(0) {}//Size of the string(0){}

String::String(char* new_string): string_mem_size(strlen(new_string)), string(new char[string_mem_size])
{
	strcpy_s(string, (string_mem_size +1), new_string);
}

String::String(const String &string_class): String(string_class.string) {}

//DESTRUCTOR
String::~String() 
{
	if (string != nullptr)
		delete[] string;
}

//METHODS
bool String::empty() const { return (string_mem_size == 0); }
int String::size() const { return string_mem_size; }
void String::clear() 
{
	if (string != nullptr)
	{
		delete[] string;
		string = nullptr;
		string_mem_size = 0u;
	}
}
const char* String::pointer() const { return string; }

//OPERATORS
String String::operator+(const String &string_class) 
{
	return strcat(string, string_class.string);
}
void String::operator+=(const String &string_class) 
{
	/*string_mem_size += string_class.string_mem_size;
	char* new_string = new char[string_mem_size+1];
	strcpy_s(string, (string_mem_size + 1));*/
}
void String::operator=(const String &string_class) 
{
	clear();
	string_mem_size = string_class.string_mem_size;
	string = new char[string_mem_size + 1];
	strcpy(string,string_class.string);
}
bool String::operator==(const String &string_class) 
{
	return (strcmp(string, string_class.string) == 0 && string_mem_size == string_class.string_mem_size);
}