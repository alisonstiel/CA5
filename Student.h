#pragma once
#include <stdlib.h>
#include <unordered_map>
#include <map>
#include <fstream>
#include "Course.h"
class Student{
	public:
		void addRequirement(std::string courseName); 
		void addToSchedule(std::string semester, std::string courseName); 
		void addCourse(Course course);
		void addRequiredCredits(std::string reqType, int credits);
		void addScheduleCredits(std::string reqType, int credits);
		void addChoiceCourse(std::string courseName, std::string choiceName);
		void addChoiceCounter(std::string choiceName, int counter);				
		std::unordered_set<std::string> getRequirements();
		std::map<std::string, std::unordered_set<std::string> > getSchedule();
		std::unordered_map<std::string, Course> getCourses();
		std::unordered_map<std::string, int> getRequiredCredits();	
		std::unordered_map<std::string, int> getScheduleCredits();
		std::unordered_map<std::string, std::string> getChoiceCourses();
		std::unordered_map<std::string, int> getChoiceCounters();				
		bool hasTaken(const std::string& course, const std::string& semester);
		std::string findLackingPrereq(const std::string& courseName, const std::string& semester);
	private:
		std::unordered_set<std::string> requirements; //student's course requirements in order to graduate
		std::map<std::string, std::unordered_set<std::string> > schedule; //student's complete planned schedule, ordered by semester
		std::unordered_map<std::string, Course> courses; //all possible courses offered
		std::unordered_map<std::string, int> requiredCredits; //map of student's required credits types: total, H, C, F...
		std::unordered_map<std::string, int> scheduleCredits; //map of student's schedule's credits types: total, H, C, F...
		std::unordered_map<std::string, std::string> choiceCourses;
		std::unordered_map<std::string, int> choiceCounters;
		
			
};
