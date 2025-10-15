#include "admin.h"
#include <fstream>

using namespace std;

void EnrollmentManager::enrollStudent(const string &studentId, const string &courseId) {
    enrollments.emplace_back(studentId, courseId);
}

bool EnrollmentManager::isEnrolled(const string &studentId, const string &courseId) const {
    for (const auto &p : enrollments) if (p.first == studentId && p.second == courseId) return true;
    return false;
}

const vector< SimplePair<string,string> >& EnrollmentManager::getEnrollments() const { return enrollments; }

bool EnrollmentManager::save(const string &filename) const {
    ofstream ofs(filename, ios::trunc);
    if (!ofs) return false;
    for (const auto &p : enrollments) ofs << p.first << "|" << p.second << "\n";
    return true;
}

bool EnrollmentManager::load(const string &filename) {
    ifstream ifs(filename);
    if (!ifs) return false;
    enrollments.clear();
    string line;
    while (getline(ifs, line)) {
        auto pos = line.find('|');
        if (pos == string::npos) continue;
        enrollments.emplace_back(line.substr(0,pos), line.substr(pos+1));
    }
    return true;
}
