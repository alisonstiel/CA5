#pragma once
#include <stdlib.h>
#include <unordered_map>
#include <fstream>
#include "Course.h"
class Student{
	public:
		void addRequirement(std::string courseName); 
		void addToSchedule(Course course); 
		void addCourse(Course course);
		std::unordered_map<std::string, bool> getRequirements();
		std::unordered_map<std::string, Course> getSchedule();
		std::unordered_map<std::string, Course> getCourses();
	private:
		std::unordered_map<std::string, bool> requirements; //student's course requirements in order to graduate
		std::unordered_map<std::string, Course> schedule; //student's complete planned schedule
		std::unordered_map<std::string, Course> courses; //all possible courses offered	
		std::string findLackingPrereq(const std::string& courseName);	
};
