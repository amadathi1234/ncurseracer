#include <iostream>
#include <unordered_map>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include "p3.hpp"

/*
 * P3 cpp file submission for CS368
 * Made by Anand Madathil
 */

//Create a new student with the given name
//DO NOT MODIFY THIS CONSTRUCTOR
Student::Student(std::string name) {
  std::ifstream ifs(name+".txt");
  if(!ifs.is_open()) {
    std::cout << "ERROR: Unable to open student data file " + name + ".txt" << std::endl;
    return;
  }
  this->name = name;
  std::string line,grade;
  int courseNum;
  while(getline(ifs,line)) {
//    std::cout << "Read line: " << line << std::endl;
    std::stringstream stst;
    stst << line;
    stst >> courseNum >> grade;
    grades[courseNum] = grade;
  }
}

//DO NOT MODIFY THIS CONSTRUCTOR
ClassList::ClassList(std::string name) {
  std::ifstream ifs(name+".txt");
  if(!ifs.is_open()) {
    std::cout << "ERROR: Unable to open classlist data file " + name + ".txt" << std::endl;
    return;
  }
  this->name = name;
  std::string line,courseName;
  int courseNum,credits;
  while(getline(ifs,line)) {
//    std::cout << "Read line: " << line << std::endl;
    std::stringstream stst;
    stst << line;
    stst >> courseNum >> credits;
    getline(stst,courseName); //Remainder of the line is course name
    courses[courseNum] = {courseName, credits};
  }
}

/**
 * Prints the class list for the "new" school
 */
void ClassList::printClassList() {
    auto iter = courses.begin();
    while(iter != courses.end()) {
        auto cur = iter->first; // grab the key of the key-value pair
        std::cout << courses[cur].first << " " << cur << " " << courses[cur].second << std::endl;
        iter++; // increment iterator
    }
}

/**
 * Get the course name of a class based on its course number
 */
std::string ClassList::getCourseName(int courseNumber) {
    if(courses.find(courseNumber) != courses.end()) {
        return courses[courseNumber].first; // first member of pair is course name
    }
    else return ""; // course not found in class list
}

/**
 * Empty constructor for outside class list
 */
OutsideClassList::OutsideClassList(std::string name) : ClassList::ClassList(name) {}

void OutsideClassList::printClassList() {
    auto iter = courses.begin();
    for(iter; iter != courses.end(); iter++) {
        auto kvp = (*iter).first;
        std::cout << courses[kvp].first << " " << kvp << std::endl;
    }
}

/**
 * Prints the outside class list courses that are equivalent to some new school course
 */
void OutsideClassList::printClassList(ClassList& c1) {
    auto iter = courses.begin();
    for(iter; iter != courses.end(); iter++) {
        auto kvp = (*iter).first; // grab the key of the current key-value pair
        if(courses[kvp].second) { // check for non-zero course number
            std::cout << courses[kvp].second << " " << c1.getCourseName(courses[kvp].second) 
                << " " << courses[kvp].first << std::endl;
        }
    }
}

/**
 * Get the outside class list equivalent of the new course number
 */
int OutsideClassList::getCourseEquivalent(int courseNumber) {
    auto iter = courses.begin();
    for(iter; iter != courses.end(); iter++) {
        auto outsideNumber = iter->first;
        if(outsideNumber == courseNumber) 
            return iter->second.second; // returned int is course number of the outside class list
    }
    return 0; // no equivalent course
}

/**
 * Print the student's courses with the grades
 */
void Student::printStudent(ClassList& cl) {
    auto iter = grades.begin();
    while(iter != grades.end()) {
        auto num = (*iter).first;
        if(cl.getCourseName(num) != "")
            std::cout << cl.getCourseName(num) << " " << grades[num] << std::endl;
            // print name then grade (space separated)
        iter++;
    }
}

/**
 * Constructor for student with transfer credit
 */
StudentWithTransferCredit::StudentWithTransferCredit(std::string name, std::string schoolName) : Student::Student(name) {
    this->schoolName = schoolName; // set school name
}

/**
 * Accessor for school name
 */
std::string StudentWithTransferCredit::getSchoolName() { return schoolName; }

/**
 * Prints the new course name of an old school course that the student 
 * got transfer credit for, and the associated grade
 */
void StudentWithTransferCredit::printStudent(OutsideClassList& oldSchool, ClassList& newSchool) {
    auto iter = grades.begin();
    for(iter; iter != grades.end(); iter++) {
        auto num = iter->first;
        if(oldSchool.getCourseEquivalent(num)) { // non-zero course equivalent
            std::cout << newSchool.getCourseName(oldSchool.getCourseEquivalent(num)) << " " << grades[num] << std::endl;
            // print course in the same order as the other printStudent()
        }
    }
}

/**
 * Driver method
 */
int main() {

    // new school classlist
    std::string insideName;
    std::cout << "Enter the name of a new school" << std::endl;
    std::cin >> insideName;
    ClassList newCl(insideName);
    newCl.printClassList();

    // old school classlist
    std::string outsideName;
    std::cout << "Enter the name of an outside school" << std::endl;
    std::cin >> outsideName;
    OutsideClassList oldCl(outsideName);
    oldCl.printClassList();
    std::cout << std::endl; 
    oldCl.printClassList(newCl);

    // transfer student
    std::string studentName;
    std::cout << "Enter the name of a student with transfer credit" << std::endl;
    std::cin >> studentName;
    StudentWithTransferCredit student (studentName, outsideName);
    student.printStudent(oldCl, newCl);

}
