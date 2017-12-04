#include <stdlib.h>
#include <iostream>
#include "Student.h"
#include "Course.h"
#include <fstream>
#include <vector>
#include <sstream>
#include <iterator>
#include <regex>
#include <unordered_map>

void fail(const std::string& reason);
 
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
	std::string course;
	std::string credits;
	std::string offered;
	
	//populate course hash map
	while(!offers.eof()){
		std::getline(offers, line);
		std::istringstream inputString(line);
        std::istream_iterator<std::string> begin(inputString), end;
        std::vector<std::string> arr(begin, end);

		if(arr.size() == 3 || arr.size() == 4){
			std::string courseName = arr.at(0);
			std::string credits = arr.at(1);
			std::string offered = arr.at(2);
			Course::Offered o;
			int credInt;
			bool goodName, goodCredits, goodOffered;

			//check course name format
			if(courseName.length() == 5){
				bool upperCheck = isupper(courseName[0]) && isupper(courseName[1]);
				bool digitCheck = isdigit(courseName[2]) && isdigit(courseName[3]) && isdigit(courseName[4]);
				if(upperCheck && digitCheck){
					goodName = true;
				}else{
					std::cout << courseName << ": Bad course name format" << std::endl;
				}
			}else{
				std::cout << courseName << ": Bad course name length" << std::endl;
			}

			//check credit format
			if(credits.length() == 1){
				credInt = credits[0] - '0';
				if(credInt >=1 && credInt <= 4){
					goodCredits = true;
				}else{
					std::cout << credits << ": Bad number of credits" << std::endl;
				}
			}else{
				std::cout << credits << ": Bad credit format" << std::endl;
			}

			//check offered time format
			if(offered.length() == 1){
				if(offered == "S" || offered == "F" || offered == "E"){
					goodOffered = true;
				}else{
					std::cout << offered << ": Bad semester offered letter" << std::endl;
				}
			}else{
				std::cout << offered << ": Bad semester offered format" << std::endl;
			}

			//create course object if everything passed
			if(goodName && goodCredits && goodOffered){
				Course newCourse(courseName);
				newCourse.setCredits(credInt);
				o = newCourse.stringToOffered(offered);
				newCourse.setOfferedTimes(o);

				//check for tags
				if(arr.size() == 4){
					std::string tags = arr.at(3);
					for(unsigned int i = 0; i<tags.length(); i++){
						if(isalpha(tags[i])){
							newCourse.addTag(tags[i]);
						}else{
							std::cout << tags << ": Bad tag format" << std::endl;
						}
					}
				}
				student.addCourse(newCourse);				
			}
		}else{
			std::cout << "Bad course offerings file format" << std::endl;
		}	
	}
	std::unordered_map<std::string, Course> courses = student.getCourses();
	//set up requirement graph
	while(!reqs.eof()){
		std::string line;
		std::getline(planned,line);
		std::istringstream aLine(line); 
		std::istream_iterator<std::string> word(aLine), end;
		//

	}
	//CHANGED: set up planned schedule to check in next loop
	int num = 1;
	while(!planned.eof()){
		std::string line, semester;
		std::getline(planned,line);
		std::istringstream aLine(line); 
		std::istream_iterator<std::string> word(aLine), end;
		std::regex semester_format("S|F[0-9]{4,}");
		if(std::regex_match(*word, semester_format)){
			semester = *word;
			word++;
			while(word != end){
				if(courses[*word].getCredits() < 0){
					fail("Course " + *word + " is not offered here.");
				}
				if(semester[0] == 'F' && courses[*word].getOfferedTimes() == Course::Offered::Spr){
					fail("Course " + *word + "is not offered in the fall");
				}
				if(semester[0] == 'S' && courses[*word].getOfferedTimes() == Course::Offered::Fall){
					fail("Course " + *word + " is not offered in the spring");
				} else {
					//reworked add to schedule
				}
				word++;
			}

		} else {
			std::cout << "Line " << num << ": Bad semester name " << *word;
		}
		num++;

	}
}

void fail(const std::string& reason){
	std::cout << "Bad plan. Here's why: " << reason << std::endl;
	exit(0);
}
