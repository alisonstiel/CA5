#include <stdlib.h>
#include <iostream>
#include "Student.h"
#include "Course.h"
#include <fstream>

int main(int argc, char** argv){
	if(argc < 4){
		std::cout << "Format: <requirements> <offerings> <schedule>\n";
		return 1;
	}
	std::ifstream reqs(argv[1]), offers(argv[2]), planned(argv[3]);
	reqs.peek();
	offers.peek();
	planned.peek(); //Does not take anything off stream, but we need to make sure these files actually exist.
	if(reqs.fail() || offers.fail() || planned.fail()){
		std::cout << "Cannot access one of the files. Please try again!";
		return 2;
	}
	Student student;
	std::string line;
	while(!offers.eof()){
		std::getline(offers, line);
		//populate courses hash map
	}
	while(!reqs.eof()){
		//set up requirement graph
	}
	while(!planned.eof()){
		//Do actual checking
	}
}
