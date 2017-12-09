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
#include <unordered_set>
#include <algorithm>
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
		int num = 1;
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
					std::cout << "Line " << num << ": Bad course name format " << courseName << std::endl;
				}
			}else{
				std::cout << "Line " << num << ": Bad course name length " << courseName << std::endl;
			}

			//check credit format
			if(credits.length() == 1){
				credInt = credits[0] - '0';
				if(credInt >=1 && credInt <= 4){
					goodCredits = true;
				}else{
					std::cout << "Line " << num << ": Bad number of credits " << credits << std::endl;
				}
			}else{
				std::cout << "Line " << num << ": Bad credit format " << credits << std::endl;
			}

			//check offered time format
			if(offered.length() == 1){
				if(offered == "S" || offered == "F" || offered == "E"){
					goodOffered = true;
				}else{
					std::cout << "Line " << num << ": Bad semester offered letter " << offered << std::endl;
				}
			}else{
				std::cout << "Line " << num << ": Bad semester offered format " << offered << std::endl;
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
							std::string s(1,tags[i]);
							newCourse.addTag(s);
						}else{
							std::cout << "Line " << num << ": Bad tag format " << tags << std::endl;
						}
					}
				}if(arr.at(0).find("CS")){
					newCourse.addTag("CS"); //courses that don't have tags are CS courses <- not true, see MA304!
				}
				student.addCourse(newCourse);				
			}
		}else{
			std::cout << "Line " << num << ": Bad course offerings file format" << std::endl;
		}
		num++;
	}
	std::unordered_map<std::string, Course>& courses = student.getCourses();
	int linenum = 1;
	int choiceCounter = 1;			
	//set up requirement graph
	std::unordered_set<std::string> required;
	while(!reqs.eof()){
		std::string line;
		std::getline(reqs,line);
		std::istringstream aLine(line); 
		std::istream_iterator<std::string> word(aLine), end;
		//std::cout << *word << std::endl;
		if(*word == "TOTAL"){
			word++;
			if(std::regex_match(*word, std::regex("[0-9]+"))){
				int cred = std::stoi(*word);
				student.addRequiredCredits("CS", cred); 
			} else {
				std::cout << "Bad total number of CS credits needed one line " << linenum << std::endl;
			}
		}

		else if(*word == "CREDIT"){
			word++;
			std::string tag = *word;
			if(!std::regex_match(tag, std::regex("[A-Z]"))){
				std::cout << "CREDIT tag on line " << linenum << " must be exactly one letter long\n";
				linenum++;				
				continue;
			}
			word++;
			if(std::regex_match(*word, std::regex("[0-9]+"))){
				int cred = std::stoi(*word);
				student.addRequiredCredits(tag, cred); 
			} else {
				std::cout << "Bad total number of CS credits needed" << std::endl;
			}
		
		}

		else if(*word == "COURSE"){
			word++;
			if(courses[*word].getCredits() < 0){
				std::cout << "Course on line " << linenum << " is not offered." << std::endl;
				linenum++;				
				continue;
			}
			std::string course = *word;
			word++;
			if(*word == "M"){
				courses[*word].setRequired(Course::Require::Mandatory);
				student.addRequirement(course); 				
			}else if(*word == "R") {
				courses[*word].setRequired(Course::Require::Required);
				required.insert(*word);
				student.addRequirement(course); 								
			}else if(*word == "O"){
				courses[*word].setRequired(Course::Require::Optional);
				//student.addRequirement(course); 								
			}else{
				std::cout << "Requirement option on line " << linenum << " must be M, R, or O! This line will be ignored." << std::endl;
				linenum++;				
				continue;	
			}
			while(word != end){
				//std::cout << "Dong?" << std::endl;
				if(courses[*word].getCredits() > 0){
					courses[course].addPrereq(*word);
					//std::cout << "what.\n";
				}
				word++;
			}
			//std::cout << course << ": ";
			//for(std::string g : courses[course].prereqs){
				//std::cout << g << " ";
			//}
			//std::cout << std::endl;
		}

		else if(*word == "CHOOSE"){
			std::string choiceName = std::to_string(choiceCounter) + "CHOICE"; //choiseNames: 1CHOICE, 2CHOICE, 3CHOICE...
			//std::cout << choiceName << std::endl;
			std::string numOfChoices;
			word++;
			if(std::regex_match(*word, std::regex("[0-9]*[1-9]"))){
				numOfChoices = *word;
			} else {
				std::cout << "Bad number format after CHOOSE. Must be a number greater than 1" << std::endl;
				linenum++;
				continue;
			}
			while(word != end){
				word++;				
				if(courses[*word].getCredits() < 0){
					std::cout << "Course " << *word << "on line " << linenum << " is not offered." << std::endl;
					linenum++;
					continue;
				}else{
					courses[*word].setRequired(Course::Require::Choice);
					student.addChoiceCourse(*word,choiceName);
					student.addChoiceCounter(choiceName,std::stoi(numOfChoices));					
					student.addRequirement(choiceName);
				}
			}
		}
		else{
			std::cout << "Requirements file Line " << linenum << ": Bad first word " << *word << std::endl; 
		}
		choiceCounter++;
		linenum++;		
	}
	
	//CHANGED: set up planned schedule to check in next loop
	int num = 1;
	while(!planned.eof()){
		std::string line, semester;
		std::getline(planned,line);
		std::istringstream aLine(line); 
		std::istream_iterator<std::string> word(aLine), end;
		std::regex semester_format("(S|F)[0-9]{4,}");
		if(std::regex_match(*word, semester_format)){
			semester = *word;
			word++;
			int credits = 0;
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
					//std::cout << semester << " " << *word << std::endl;
					credits += courses[*word].getCredits();
					for(std::string t : courses[*word].getTags()){
						student.addScheduleCredits(t, courses[*word].getCredits()); 
					}
				}
				word++;
			}
			if(credits > 18){
				std::cout << "Note: you are overloading on semester " << semester << ". This may require special approval depending on your GPA." << std::endl;
			} else if(credits < 12){
				std::cout << "Warning: the schedule for " << semester << " is not full-time (at least 12 credits). This may endanger federal financial aid." << std::endl;
			}
		} else {
			std::cout << "Plan file Line " << num << ": Bad semester name " << *word << std::endl;
		}
		num++;

	}



	/*for(auto p : student.getSchedule()){
		std::cout << std::get<0>(p) << ": ";
		for(auto d : std::get<1>(p)){
			std::cout << d << std::endl;
		}
	}*/
//Prerequisite checking
	for(auto sem : student.getSchedule()){
		//std::cout << std::get<0>(sem);
		for(auto course : std::get<1>(sem)){
			//std::cout << "lol";
			std::string lacking = student.findLackingPrereq(course, std::get<0>(sem));
			if(lacking != ""){
				fail(lacking + " is required to take " + course);

			//more checks to make sure course/semester is valid

			}else if(courses[course].getRequired() == Course::Require::Choice){ //this will take care of CHOOSE
				std::string choiceName = student.getChoiceCourses()[course];
				student.getChoiceCounters()[choiceName]--;
				if(student.getChoiceCounters()[choiceName] == 0) student.getRequirements().erase(choiceName); //choice requirements met
			}else{
				student.getRequirements().erase(course);
			}
		}
	}

	//final check
	bool reqsFulfilled = student.getRequirements().size() == 0;
	bool creditsFulfilled = true; //todo
	std::vector<std::string> neededTags;
	std::unordered_map<std::string, int> requiredCreds = student.getRequiredCredits();
	std::unordered_map<std::string, int> fufilled = student.getScheduleCredits();
	for(auto requirement = requiredCreds.begin(); requirement != requiredCreds.end(); requirement++){
		std::string type = std::get<0>(*requirement);
		int needed = std::get<1>(*requirement);
		if(fufilled[type] < needed){
			creditsFulfilled = false;
			neededTags.push_back(type); 	
		}
	}
	if(reqsFulfilled && creditsFulfilled){
		std::cout << "Good plan. Get to work." << std::endl;
	}
	else if(!reqsFulfilled){
		std::cout << "Needed courses: ";
		for(std::string r: student.getRequirements()){
			//std::cout << "***" << r << "***" << std::endl;
			if(std::regex_match(r, std::regex("CHOICE"))){
				std::cout << student.getChoiceCounters()[r] << " more of " << "( ";
				for(auto cou : student.getChoiceCourses()){
					if(std::get<1>(cou) == r){
						std::cout << std::get<0>(cou) << " ";
					}			
				}	
				std::cout << ") ";
			} else {
				std::cout << r << " ";
			}
		}
		std::cout << std::endl;
	}
	else{
		std::cout << "You don't have enough ";
		for(std::string c : neededTags){
			std::cout << c ;// << " (" << student.getScheduleCredits()[c] << ") "; 
		}
		std::cout << "credits to graduate.\n";
	}
}	

void fail(const std::string& reason){
	std::cout << "Bad plan. Here's why: " << reason << std::endl;
	exit(0);
}
