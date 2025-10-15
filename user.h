#ifndef USER_H
#define USER_H

#include <string>
#include <vector>

enum class Role { STUDENT, INSTRUCTOR };

class User {
protected:
    std::string id;
    std::string name;
    Role role;
public:
    User(const std::string &id_ = "", const std::string &name_ = "", Role r = Role::STUDENT);
    virtual ~User();
    std::string getId() const;
    std::string getName() const;
    Role getRole() const;
    virtual void display() const;
};

class Student : public User {
    std::vector<std::string> enrolledCourseIds;
public:
    Student(const std::string &id_ = "", const std::string &name_ = "");
    void enroll(const std::string &courseId);
    const std::vector<std::string>& getEnrollments() const;
    void display() const override;
};

class Instructor : public User {
public:
    Instructor(const std::string &id_ = "", const std::string &name_ = "");
    void display() const override;
};

#endif // USER_H
