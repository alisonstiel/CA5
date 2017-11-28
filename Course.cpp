#pragma once
#include <stdlib.h>
#include <list>
#include <unordered_set>
class Course{
	private:
		std::list<std::string> prereqs;
		std::string name;
		int credits;
		std::unordred_set<std::string> tags;
	public:
		enum Offered{Spr,Fall,Every};
		Course(const std::string& name, const std::string, 
		addPrereq(const std::string& name);
		std::string getName();
		std::string getTags();
};
