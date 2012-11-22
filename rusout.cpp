#include "stdafx.h"

/************************************************************************/
/* Русские буквы в консоли                                              */
/************************************************************************/
std::basic_ostream< char, std::char_traits<char> >& operator << ( std::basic_ostream< char, std::char_traits<char> >& out, const char* str )
{
	std::vector<char> temp ( strlen ( str ) + 1 );
	if ( ::CharToOemA ( str, &temp[0] ) )
	{
		std::string dest ( &temp[0] );
		out << dest;
	}
	return out;
}


