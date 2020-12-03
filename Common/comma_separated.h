#include <iostream>
#include <string>

template<class T, char sep = ','>
struct comma_separated
{
	T value;
	operator const T& () const { return value; }
};

template<class T, char sep>
std::istream& operator>>(std::istream& in, comma_separated<T, sep>& t)
{
	if (!(in >> t.value)) //if we failed to read the int
		return in;          //return failure state
	if (in.peek() == sep) //if next character is a comma
		in.ignore();        //extract it from the stream and we're done
	else                  //if the next character is anything else
		in.clear();         //clear the EOF state, read was successful
	return in;
}

template<char sep>
std::istream& operator>>(std::istream& in, comma_separated<std::string, sep>& t)
{
	std::getline(in, t.value, sep);
	return in;
}
