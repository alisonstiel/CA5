#include <stdlib.h>
#include <list>
#include <unordered_set>
#include <unordered_map>
#include "Student.h"

void Student::addRequirement(std::string& courseName){
    requirements.insert(std::make_pair<std::string,bool>(courseName,false));
}
void Student::addToSchedule(Course& course){
    schedule.insert(std::make_pair<std::string,Course>(course.getName(),course));
}
void Student::addCourse(Course& course){
    courses.insert(std::make_pair<std::string,Course>(course.getName(),course));
}
std::unordered_map<std::string, bool> Student::getRequirements(){
    return requirements;
}
std::unordered_map<std::string, Course> Student::getSchedule(){
    return schedule;
}
std::unordered_map<std::string, Course> Student::getCourses(){
    return courses;
}

std::string Student::findLackingPrereq(const std::string& courseName){
	std::list<std::string> queue;
	for(auto i = courses.begin(); i != courses.end(); i++){
		i.touched = false;
	}
	if(courses.count(courseName) == 0){
		return "Course does not exist";
	}
	while(queue.size() > 0){
		Course c = courses[*queue.front()];
		queue.pop_front();
		c.touched = true;
		for(auto pre = c.getPrereqs().begin(); c != c.getPrereqs().end(); c++){
			pre.touched = true;
			if(schedule.count(pre) == 0){
				return pre;
			}
			if(!courses[pre].touched) queue.push_back(pre);
		}
	}
	return "";

}
