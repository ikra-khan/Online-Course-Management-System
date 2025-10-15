#ifndef ADMIN_H
#define ADMIN_H

#include "course.h"
#include "user.h"
#include <vector>
#include <string>

// EnrollmentManager associates Students and Courses; uses SimplePair from course.h
class EnrollmentManager {
    std::vector< SimplePair<std::string, std::string> > enrollments; // studentId, courseId
public:
    void enrollStudent(const std::string &studentId, const std::string &courseId);
    bool isEnrolled(const std::string &studentId, const std::string &courseId) const;
    const std::vector< SimplePair<std::string, std::string> >& getEnrollments() const;
    bool save(const std::string &filename) const;
    bool load(const std::string &filename);
};

#endif // ADMIN_H
