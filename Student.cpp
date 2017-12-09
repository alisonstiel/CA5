#include <stdlib.h>
#include <list>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include "Student.h"


void Student::addRequirement(std::string courseName){
    requirements.insert(courseName);
}

void Student::addToSchedule(std::string semester, std::string courseName){
	if(semester.front() == 'S'){
		semester.erase(0,1);
		semester+=".1";
		//std::cout << semester << std::endl;
	}else if(semester.front() == 'F'){
		semester.erase(0,1);
		semester+=".2";
		//std::cout << semester << std::endl;
	}else{
		std::cout << "Bad semester format" << std::endl;
	}
	schedule[semester].insert(courseName);
}

void Student::addCourse(Course course){
    courses[course.getName()] = course;
}

void Student::addRequiredCredits(std::string reqType, int credits){
	if(credits > 0){
		requiredCredits[reqType] += credits; //creates a new key if reqType doesn't yet exist, or increments it's value if it does exist 	
	}
}

void Student::addScheduleCredits(std::string reqType, int credits){
	if(credits > 0){
		scheduleCredits[reqType] += credits;	
	}
}

void Student::addChoiceCourse(std::string courseName, std::string choiceName){
	choiceCourses[courseName] = choiceName;
}

void Student::addChoiceCounter(std::string choiceName, int counter){
	choiceCounters[choiceName] = counter;
}			

std::unordered_set<std::string>& Student::getRequirements(){
    return requirements;
}

std::map<std::string, std::unordered_set<std::string> >& Student::getSchedule(){
    return schedule;
}

std::unordered_map<std::string, Course>& Student::getCourses(){
    return courses;
}

std::unordered_map<std::string, int>& Student::getRequiredCredits(){
    return requiredCredits;
}

std::unordered_map<std::string, int>& Student::getScheduleCredits(){
    return scheduleCredits;
}

std::unordered_map<std::string, std::string>& Student::getChoiceCourses(){
    return choiceCourses;
}

std::unordered_map<std::string, int>& Student::getChoiceCounters(){
    return choiceCounters;
}

std::string Student::findLackingPrereq(const std::string& courseName, std::string semester){
	std::list<std::string> queue;
	//std::cout << courseName << std::endl;
	//std::cout << "Dong!" << std::endl;
	for(auto i = courses.begin(); i != courses.end(); i++){
		(std::get<1>(*i)).touched = false;
	}
	if(courses[courseName].getCredits() < 0){
		return "Course does not exist";
	}
	queue.push_back(courseName);
	while(queue.size() > 0){
		//std::cout << "Ding!" << std::endl;
		Course c = courses[*queue.begin()];
		//std::cout << '\t' << *queue.begin() << std::endl;
		queue.pop_front();
		c.touched = true;
		//std::cout << '\t' << c.getCredits() << std::endl;
		//std::cout << c.prereqs.size() << std::endl;
		for(std::string pre : c.prereqs){
			//std::cout << "AAAIIIIEEEE!!!!" << std::endl;
			if(!courses[pre].touched) queue.push_back(pre);
			courses[pre].touched = true;
			//std::cout << "\t\t" << pre << std::endl;
			if(!hasTaken(pre, semester)){
				return pre;
			}
			
		}
	}
	return "";

}
//Check to see if student has taken the course before the given semester
bool Student::hasTaken(const std::string& course, std::string semester){
	if(semester.front() == 'S'){
		semester.erase(0,1);
		semester+=".1";
		//std::cout << semester << std::endl;
	}else if(semester.front() == 'F'){
		semester.erase(0,1);
		semester+=".2";
		//std::cout << semester << std::endl;
	}
	//std::cout << semester << std::endl;
	for(auto sem : schedule){ //It only works with this version of iteration, somehow
		if(std::get<0>(sem) >=  semester) return false;
		if(std::get<1>(sem).count(course)) return true;
	}
	return false;
}
