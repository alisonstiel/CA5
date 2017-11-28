#include <stdlib.h>
#include <list>
#include <unordered_set>
#include "Course.h"

Course::Course(std::string name){
	this -> name = name;
}

int Course::getCredits(){
	return credits;
}

void Course::setCredits(int c){
	credits = c;
}

void Course::addPrereq(const std::string& name){
	prereqs.push_back(name);
}

std::string Course::getName(){
	return name;
}

std::unordered_set<std::string> Course::getTags(){
	return tags;
}
void Course::addTag(const std::string& tag){
	tags.insert(tag);
}

Course::Offered Course::getOfferedTimes(){
	return offered;
}

void Course::setOfferedTimes(Course::Offered o){
	offered = o;
}

Course::Require Course::getRequired(){
	return req;
}

void Course::setRequired(Course::Require r){
	req = r;
}




