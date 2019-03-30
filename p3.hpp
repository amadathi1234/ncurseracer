/*
 * Author: Alexi Brooks
 * Date: March 2019
 */

#include <iostream>
#include <unordered_map>
#include <map>
#include <string>
#include <fstream>

class ClassList {
  protected:
    std::string name; //Name of the school
    std::unordered_map<int,std::pair<std::string,int>> courses; //Maps course number to name and credits
  public:
    ClassList(std::string name); //Load classlist info from file
    void printClassList(); //Print the entire classlist
    std::string getCourseName(int courseNumber); //Get the course name given its number
};

class OutsideClassList : public ClassList {
  public:
    OutsideClassList(std::string name); //Load classlist info from file
    void printClassList(); //Print the entire classlist
    void printClassList(ClassList& cl); //Print the classes that transfer with their equivalents
    int getCourseEquivalent(int courseNumber);
};

class Student {
  protected:
    std::string name;
    std::map<int,std::string> grades; //Maps course number to letter grade
  public:
    Student(std::string name); //Load student info from file
    std::string getStudentName(); //Return student's name
    void printStudent(ClassList& cl); //Print student data
};

class StudentWithTransferCredit : public Student {
  protected:
    std::string schoolName; //Use this school's classlist for reference
  public:
    StudentWithTransferCredit(std::string name, std::string schoolName); //Load student info from file
    std::string getSchoolName(); //Return name of student's former school
    void printStudent(OutsideClassList& oldSchool, ClassList& newSchool); //Print student data
};
