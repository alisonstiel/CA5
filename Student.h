#pragma once
#include <stdlib.h>
#include <unordered_map>
#include <fstream>
#include "Course.h"
class Student{
	public:
		Student();
		void addRequirement(std::string courseName); 
		void addToSchedule(Course course); 
		void addCourse(Course course);
		void addRequiredCredits(std::string reqType, int credits);
		void addScheduleCredits(std::string reqType, int credits);
		std::unordered_map<std::string, bool> getRequirements();
		std::unordered_map<std::string, Course> getSchedule();
		std::unordered_map<std::string, Course> getCourses();
		std::unordered_map<std::string, int> getRequiredCredits();	
		std::unordered_map<std::string, int> getScheduleCredits();
	private:
		std::unordered_map<std::string, bool> requirements; //student's course requirements in order to graduate
		std::unordered_map<std::string, Course> schedule; //student's complete planned schedule
		std::unordered_map<std::string, Course> courses; //all possible courses offered
		std::unordered_map<std::string, int> requiredCredits; //map of student's required credits types: total, H, C, F...
		std::unordered_map<std::string, int> scheduleCredits; //map of student's schedule's credits types: total, H, C, F...
		std::string findLackingPrereq(const std::string& courseName);	
};
