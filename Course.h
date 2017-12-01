#pragma once
#include <stdlib.h>
#include <list>
#include <iostream>
#include <unordered_set>
class Course{
	public:
		enum Offered{Spr,Fall,Every};
		enum Require{Mandatory, Required, Optional};
		Course(std::string name);
		Course();
		void addTag(const std::string& tag); 
		void addPrereq(const std::string& name);
		Offered getOfferedTimes();
		void setOfferedTimes(Offered o);
		Require getRequired();
		void setRequired(Require r);
		int getCredits();
		void setCredits(int c);
		std::string getName();
		std::unordered_set<std::string> getTags();
		std::list<std::string> prereqs;
		bool touched; //false = white, true = grey or black. Note that we don't need to keep track of distance, and we are currently proccessing the predecessor of the node when we find the student doesn't have the prereq 
	private:
		std::string name;
		int credits;
		std::unordered_set<std::string> tags;
		Offered offered;
		Require req;
};
