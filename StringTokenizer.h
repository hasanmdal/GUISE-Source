/*
 ***********************************************************************
 * Class: StringTokenizer                                              *
 * By Arash Partow - 2000                                              *
 * URL: http://www.partow.net/programming/stringtokenizer/index.html   *
 *                                                                     *
 * Copyright Notice:                                                   *
 * Free use of this library is permitted under the guidelines and      *
 * in accordance with the most current version of the Common Public    *
 * License.                                                            *
 * http://www.opensource.org/licenses/cpl.php                          *
 *                                                                     *
 ***********************************************************************
*/


//! \file StringTokenizer.h - definition of StringTokenizer class
#ifndef INCLUDE_STRINGTOKENIZER_H
#define INCLUDE_STRINGTOKENIZER_H


#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>


class StringTokenizer
{

   public:

    StringTokenizer(const std::string& _str, const std::string& _delim);//!< Constructor
   ~StringTokenizer(){};//!< Destructor

    int         countTokens(); //!< a member function
    bool        hasMoreTokens(); //!< a member function
    std::string nextToken(); //!< a member function
    int         nextIntToken();	//!< a member function
    double      nextFloatToken();//!< a member function
    std::string nextToken(const std::string& delim);//!< a member function
    std::string remainingString();//!< a member function
    std::string filterNextToken(const std::string& filterStr);//!< a member function

   private:

    std::string  token_str;//!< A private member variable
    std::string  delim;//!< A private member variable

};

#endif
