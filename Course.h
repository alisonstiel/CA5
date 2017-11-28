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
	private:
		std::list<std::string> prereqs;
		std::string name;
		int credits;
		std::unordered_set<std::string> tags;
		Offered offered;
		Require req;
};
