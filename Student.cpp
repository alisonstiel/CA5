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