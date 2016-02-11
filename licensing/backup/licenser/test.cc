
#include <string>
#include <iostream>
#include <cstdio>

#include "Coder.hpp"

using namespace std;
using namespace Licenser;


int main(int argc, char **argv) {


	string line("U2FsdGVkX190gtFTJgL4KZ3Vr25I307usMINWZj8HY4L4BLCyjgNelpk/dQDwyDXQ8VZL0Vg+4T0R0oEA7aLZA==");
	string output;
	string encoded;

	try {
		cout << "input: " << line << endl;
		Coder::decodeBF(line, output);
		cout << "decoded output: " << output << endl;
		Coder::encodeBF(output, encoded);
		cout << "encoded output: " << encoded << endl;
		Coder::decodeBF(encoded, output);
		cout << "again decoded output: " << output << endl;

	} catch (const exception &ex) {
	
		cerr << "exception: " << ex.what() << endl;
	}

}



