// The latest version of this library is available on GitHub;
//   https://github.com/amir-s/jute/blob/master/jute.cpp

// This is free and unencumbered software released into the public domain.
//
// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.
//
// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors. We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
//
// For more information, please refer to <http://unlicense.org/>

#ifndef __JUTE_H__
#define __JUTE_H__

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <fstream>
#include <cstring>

using namespace std;

namespace jute {
	enum jType { JSTRING, JOBJECT, JARRAY, JBOOLEAN, JNUMBER, JNULL, JUNKNOWN };
	/*!
	* \brief Representing JSON object used by JSON parser.
	*
	* Source: https://github.com/amir-s/jute/blob/master/jute.cpp
	*/
	class jValue {
	private:
		string makesp(int);
		string svalue;
		jType type;
		vector<pair<string, jValue> > properties;
		map<string, int> mpindex;
		vector<jValue> arr;
		string to_string_d(int);
	public:
		jValue();
		jValue(jType);
		string to_string();
		jType get_type();
		void set_type(jType);
		void add_property(string key, jValue v);
		void add_element(jValue v);
		void set_string(string s);
		int as_int();
		double as_double();
		bool as_bool();
		void* as_null();
		string as_string();
		int size();
		jValue operator[](int i);
		jValue operator[](string s);
	};

	/*!
	* \brief JSON parser
	*
	* Source: https://github.com/amir-s/jute/blob/master/jute.cpp
	*/
	class parser {
	private:
		enum token_type { UNKNOWN, STRING, NUMBER, CROUSH_OPEN, CROUSH_CLOSE, BRACKET_OPEN, BRACKET_CLOSE, COMMA, COLON, BOOLEAN, NUL };

		struct token;
		static bool is_whitespace(const char c);
		static int next_whitespace(const string& source, int i);
		static int skip_whitespaces(const string& source, int i);

		static vector<token> tokenize(string source);
		static jValue json_parse(vector<token> v, int i, int& r);
	public:
		static jValue parse(const string& str);
		static jValue parse_file(const string& str);
	};
}
#endif