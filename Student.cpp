#include <stdlib.h>
#include <list>
#include <unordered_set>
#include <unordered_map>
#include "Student.h"

void Student::addRequirement(Course& req){
    requirements.insert(std::make_pair<Course,bool>(req,false));
}
void Student::addToSchedule(Course& course){
    schedule.insert(course);
}
void Student::addCourse(Course& course){
    courses.insert(course);
}
std::unordered_map<Course, bool> Student::getRequirements(){
    return requirements;
}
std::unordered_set<Course> Student::getSchedule(){
    return schedule;
}
std::unordered_set<Course> Student::getCourses(){
    return courses;
}