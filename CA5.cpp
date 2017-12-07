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
#include <map>

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
	int linenum = 1;
	//set up requirement graph
	while(!reqs.eof()){
		std::string line;
		std::getline(planned,line);
		std::istringstream aLine(line); 
		std::istream_iterator<std::string> word(aLine), end;
		if(*word == "TOTAL"){
			word++;
			if(std::regex_match(*word, regex("[0-9]+"))){
				int cred = std::stoi(*word);
				student.addRequiredCredits("total", cred); 
			} else {
				std::cout << "Bad total number of CS credits needed one line " << linenum << std::endl;
			}
			
		}
		else if(*word == "CREDIT"){
			word++;
			std::string tag = *word;
			if(!std::regex_match(regex([A-Z]), tag)){
				std::cout << "CREDIT tag on line " << linenum << " must be exactly one letter long\n";
				continue;
			}
			word++
			if(std::regex_match(*word, regex("[0-9]+"))){
				int cred = std::stoi(*word);
				student.addRequiredCredits("total", cred); 
			} else {
				std::cout << "Bad total number of CS credits needed" << std::endl;
			}
		
		}
		else if(*word == "COURSE"){
			word++;
		}
		else if(*word == "CHOOSE"){
			word++;
		}
		else{
			std::cout << "Line " << linenum << ": Bad first word " << *word; 
		}
		linenum++;
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
			int credits;
			while(word != end){
				if(courses[*word].getCredits() < 0){
					fail("Course " + *word + " is not offered here.");
				}
				if(semester[0] == 'F' && courses[*word].getOfferedTimes() == Course::Offered::Spr){
					fail("Course " + *word + " is not offered in the fall");
				}
				if(semester[0] == 'S' && courses[*word].getOfferedTimes() == Course::Offered::Fall){
					fail("Course " + *word + " is not offered in the spring");
				} else {
					student.addToSchedule(semester, *word);
					credits += courses[*word].getCredits();
					for(char t : courses[*word].getTags()){
						std::string s(1,t);
						student.addScheduleCredits(s, courses[*word].getCredits());
					}
					if((*word).find("CS")){
						student.addScheduleCredits("total", courses[*word].getCredits());
					}
				}
				word++;
			}
			if(credits > 18){
				std::cout << "Note: you are overloading on semester " << semester << ". This may require special approval depending on your GPA." << std::endl;
			} else if(credits < 12){
				std::cout << "Warning: the schedule for" << semester << " is not full-time (at least 12 credits). This may endanger federal financial aid." << std::endl;
			}
		} else {
			std::cout << "Line " << num << ": Bad semester name " << *word;
		}
		num++;

	}
}
//Prerequisite checking
for(auto sem = student.getSchedule().begin(); sem != student.getSchedule().end(); sem++){
	for(auto course = std::get<1>(sem).begin(); course != std::get<1>(sem).end(); course++){
		std::string lacking = student.getLackingPrereq(*course);
		if(lacking != ""){
			fail(lacking + " is required to take " + *course);
		}
	}
}

void fail(const std::string& reason){
	std::cout << "Bad plan. Here's why: " << reason << std::endl;
	exit(0);
}
