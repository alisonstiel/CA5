#include <stdlib.h>
#include <list>
#include <unordered_set>
#include "Course.h"
Course::Course(){
	name = "";
	credits = -1;
}
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

Course::Offered Course::stringToOffered(std::string str){
	Offered o;
	if(str == "S"){
		o = Spr;
	}else if(str == "F"){
		o = Fall;
	}else if(str == "E"){
		o = Every;
	}
	return o;
}

void Course::printCourseInfo(){
	std::cout << "Name: " << getName() << std::endl;
	std::cout << "Credits: " << getCredits() << std::endl;
	std::cout << "Offered: " << getOfferedTimes() << std::endl;	
	std::cout << "Num of Tags: " << getTags().size() << std::endl;		
}