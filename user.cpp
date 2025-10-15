#include "user.h"
#include <iostream>

User::User(const std::string &id_, const std::string &name_, Role r) : id(id_), name(name_), role(r) {}
User::~User() = default;
std::string User::getId() const { return id; }
std::string User::getName() const { return name; }
Role User::getRole() const { return role; }
void User::display() const {
    std::cout << "User: " << name << " (ID: " << id << ") Role: " << (role == Role::STUDENT ? "Student" : "Instructor") << "\n";
}

Student::Student(const std::string &id_, const std::string &name_) : User(id_, name_, Role::STUDENT) {}
void Student::enroll(const std::string &courseId) { enrolledCourseIds.push_back(courseId); }
const std::vector<std::string>& Student::getEnrollments() const { return enrolledCourseIds; }
void Student::display() const {
    std::cout << "Student: " << name << " (ID: " << id << ")\nEnrolled courses: ";
    if (enrolledCourseIds.empty()) std::cout << "None";
    else for (const auto &c : enrolledCourseIds) std::cout << c << " ";
    std::cout << "\n";
}

Instructor::Instructor(const std::string &id_, const std::string &name_) : User(id_, name_, Role::INSTRUCTOR) {}
void Instructor::display() const {
    std::cout << "Instructor: " << name << " (ID: " << id << ")\n";
}
