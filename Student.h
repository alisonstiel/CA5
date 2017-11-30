#pragma once
#include <stdlib.h>
#include <unordered_map>
#include <fstream>
#include "Course.h"
class Student{
	public:
		void addRequirement(Course& req); 
		void addToSchedule(Course& course); 
		void addCourse(Course& course);
		std::unordered_map<Course, bool> getRequirements();
		std::unordered_set<Course> getSchedule();
		std::unordered_set<Course> getCourses();
	private:
		std::unordered_map<Course, bool> requirements; //student's course requirements in order to graduate
		std::unordered_set<Course> schedule; //student's complete planned schedule
		std::unordered_set<Course> courses; //all possible courses offered		
};
