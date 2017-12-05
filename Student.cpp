#include <stdlib.h>
#include <list>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include "Student.h"

Student::Student(){
	requiredCredits["total"] = 0;
	scheduleCredits["total"] = 0;	
}

void Student::addRequirement(std::string courseName){
    requirements[courseName] = false;
}

void Student::addToSchedule(std::string semester, std::string courseName){
	if(semester.front() == 'S'){
		semester.erase(0,0);
		semester+=".1";
	}else if(semester.front() == 'F'){
		semester.erase(0,0);
		semester+=".2";
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

std::unordered_map<std::string, bool> Student::getRequirements(){
    return requirements;
}

std::map<std::string, std::unordered_set<std::string> > Student::getSchedule(){
    return schedule;
}

std::unordered_map<std::string, Course> Student::getCourses(){
    return courses;
}

std::unordered_map<std::string, int> Student::getRequiredCredits(){
    return requiredCredits;
}

std::unordered_map<std::string, int> Student::getScheduleCredits(){
    return scheduleCredits;
}

std::string Student::findLackingPrereq(const std::string& courseName){
	std::list<std::string> queue;
	for(auto i = courses.begin(); i != courses.end(); i++){
		(std::get<1>(*i)).touched = false;
	}
	if(courses.count(courseName) == 0){
		return "Course does not exist";
	}
	while(queue.size() > 0){
		Course c = courses[*queue.begin()];
		queue.pop_front();
		c.touched = true;
		for(auto pre = c.prereqs.begin(); pre != c.prereqs.end(); pre++){
			courses[*pre].touched = true;
			if(schedule.count(*pre) == 0){
				return *pre;
			}
			if(!courses[*pre].touched) queue.push_back(*pre);
		}
	}
	return "";

}
