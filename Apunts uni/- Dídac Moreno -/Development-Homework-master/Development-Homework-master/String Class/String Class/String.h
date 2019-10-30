#ifndef _STRING__H_
#define _STRING__H_

#define _CRT_SECURE_NO_WARNINGS

#include <cstring>
class String
{
public:

	String();
	String(char* string);
	String(const String &string_class);

	~String();

	//METHODS
	bool empty() const;
	int size() const;
	void clear();
	const char* pointer() const;

	//OPERATORS  // +,+=,==, =

	String operator+(const String &string_class);
	void operator+= (const String &string_class);
	void operator= (const String &string_class);
	bool operator== (const String &string_class);


private:

	char* string = nullptr; //Pointer to the array of chars
	unsigned int string_mem_size; //Size of the string
};


#endif //