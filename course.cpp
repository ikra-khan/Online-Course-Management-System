// course_app.cpp
// Single-file example implementing grading-sheet features:
// - Inheritance, association, file streams, pointers, polymorphism, virtual functions,
// - friend function, operator overloading, STL, templates, enum, multi-file friendly structure.
//
// Compile: g++ -std=c++17 -O2 -o course_app course_app.cpp
// Run: ./course_app

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <memory>   // for smart pointers
#include <sstream>
#include <iomanip>  // for formatting
using namespace std;

// ==============================
// UML (simple textual representation)
// ==============================
// [User] <|-- [Student]
// [User] <|-- [Instructor]
// [Course] "1" <--> "*" [Segment] (composition of segments)
// [EnrollmentManager] associates [Student] and [Course]
// [CourseManager] manages Courses (uses file streams via FileManager)
// [FileManager] handles persistence
//
// This file implements those classes and a CLI.

// ==============================
// Utility: Simple template (user-made template)
// ==============================
template <typename T1, typename T2>
struct SimplePair {
    T1 first;
    T2 second;
    SimplePair() = default;
    SimplePair(const T1 &a, const T2 &b) : first(a), second(b) {}
};

// ==============================
// Enum for user roles
// ==============================
enum class Role { STUDENT, INSTRUCTOR };

// ==============================
// Segment base class with polymorphism
// ==============================
class Segment {
protected:
    string title;
    int durationMinutes;
public:
    Segment(const string &t = "", int d = 0) : title(t), durationMinutes(d) {}
    virtual ~Segment() = default;
    virtual void display() const {
        cout << "Segment: " << title << " (" << durationMinutes << " min)\n";
    }
    virtual string serialize() const {
        // type|title|duration
        ostringstream oss;
        oss << "Segment|" << title << "|" << durationMinutes;
        return oss.str();
    }
    static unique_ptr<Segment> deserialize(const string &line); // forward
};

// VideoSegment derived
class VideoSegment : public Segment {
    string videoUrl;
public:
    VideoSegment(const string &t = "", int d = 0, const string &url = "")
        : Segment(t, d), videoUrl(url) {}
    void display() const override {
        cout << "Video: " << title << " (" << durationMinutes << " min) - URL: " << videoUrl << "\n";
    }
    string serialize() const override {
        ostringstream oss;
        oss << "VideoSegment|" << title << "|" << durationMinutes << "|" << videoUrl;
        return oss.str();
    }
};

// QuizSegment derived
class QuizSegment : public Segment {
    int questions;
public:
    QuizSegment(const string &t = "", int d = 0, int q = 0)
        : Segment(t, d), questions(q) {}
    void display() const override {
        cout << "Quiz: " << title << " (" << durationMinutes << " min) - Qs: " << questions << "\n";
    }
    string serialize() const override {
        ostringstream oss;
        oss << "QuizSegment|" << title << "|" << durationMinutes << "|" << questions;
        return oss.str();
    }
};

unique_ptr<Segment> Segment::deserialize(const string &line) {
    // expected formats:
    // VideoSegment|title|duration|url
    // QuizSegment|title|duration|questions
    // Segment|title|duration
    vector<string> parts;
    string tmp;
    istringstream iss(line);
    while (getline(iss, tmp, '|')) parts.push_back(tmp);
    if (parts.size() < 3) return nullptr;
    string type = parts[0];
    string title = parts[1];
    int duration = stoi(parts[2]);
    if (type == "VideoSegment" && parts.size() >= 4) {
        return make_unique<VideoSegment>(title, duration, parts[3]);
    } else if (type == "QuizSegment" && parts.size() >= 4) {
        int q = stoi(parts[3]);
        return make_unique<QuizSegment>(title, duration, q);
    } else {
        return make_unique<Segment>(title, duration);
    }
}

// ==============================
// Course class (expanded)
// ==============================
class Course {
private:
    string id;
    string title;
    string duration;
    int price;
    string offer;
    string topic;
    string outline;
    string progress;
    bool certificate;
    vector< unique_ptr<Segment> > segments; // composition: Course owns segments
public:
    Course(const string &id_ = "", const string &title_ = "", const string &duration_ = "",
           int price_ = 0, const string &offer_ = "", const string &topic_ = "",
           const string &outline_ = "", const string &progress_ = "", bool certificate_ = false)
        : id(id_), title(title_), duration(duration_), price(price_), offer(offer_),
          topic(topic_), outline(outline_), progress(progress_), certificate(certificate_) {}

    // copy not allowed (due to unique_ptr), but support move
    Course(const Course &other) = delete;
    Course& operator=(const Course &other) = delete;
    Course(Course &&) = default;
    Course& operator=(Course &&) = default;

    // setters/getters
    void setTitle(const string &t) { title = t; }
    string getTitle() const { return title; }
    void setId(const string &i) { id = i; }
    string getId() const { return id; }
    void setDurationStr(const string &d) { duration = d; }
    string getDurationStr() const { return duration; }
    void setPrice(int p) { price = p; }
    int getPrice() const { return price; }
    void setOffer(const string &o) { offer = o; }
    string getOffer() const { return offer; }
    void setTopic(const string &t) { topic = t; }
    string getTopic() const { return topic; }
    void setOutline(const string &o) { outline = o; }
    string getOutline() const { return outline; }
    void setProgress(const string &p) { progress = p; }
    string getProgress() const { return progress; }
    void setCertificate(bool c) { certificate = c; }
    bool hasCertificate() const { return certificate; }

    // segments
    void addSegment(unique_ptr<Segment> seg) {
        segments.push_back(move(seg));
    }
    void display() const {
        cout << "\n========== Course Information ==========\n";
        cout << "ID: " << id << "\n";
        cout << "Title: " << title << "\n";
        cout << "Duration: " << duration << "\n";
        cout << "Price: $" << price << "\n";
        cout << "Offer: " << offer << "\n";
        cout << "Topic: " << topic << "\n";
        cout << "Outline: " << outline << "\n";
        cout << "Progress: " << progress << "\n";
        cout << "Certificate: " << (certificate ? "Available ✅" : "Not Available ❌") << "\n";
        cout << "Segments: \n";
        for (const auto &s : segments) s->display();
        cout << "========================================\n";
    }

    // serialization for file storage
    string serialize() const {
        // One-line course header and then segments on following lines. We'll separate sections by newline markers when writing files.
        ostringstream oss;
        oss << "COURSE|" << id << "|" << title << "|" << duration << "|" << price << "|" << offer << "|" << topic << "|"
            << outline << "|" << progress << "|" << certificate << "\n";
        for (const auto &s : segments) {
            oss << s->serialize() << "\n";
        }
        oss << "ENDCOURSE\n";
        return oss.str();
    }
    static Course deserialize(istream &in) {
        // expects the stream positioned at the COURSE line
        string line;
        getline(in, line);
        vector<string> header;
        {
            istringstream iss(line);
            string part;
            while (getline(iss, part, '|')) header.push_back(part);
        }
        if (header.size() < 10) {
            throw runtime_error("Bad course header");
        }
        Course c(header[1], header[2], header[3], stoi(header[4]), header[5], header[6], header[7], header[8], header[9] == "1");
        while (getline(in, line)) {
            if (line == "ENDCOURSE") break;
            auto seg = Segment::deserialize(line);
            if (seg) c.addSegment(move(seg));
        }
        return c;
    }
};

// ==============================
// User base and derived classes
// ==============================
class User {
protected:
    string id;
    string name;
    Role role;
public:
    User(const string &id_ = "", const string &name_ = "", Role r = Role::STUDENT) : id(id_), name(name_), role(r) {}
    virtual ~User() = default;
    string getId() const { return id; }
    string getName() const { return name; }
    Role getRole() const { return role; }
    virtual void display() const {
        cout << "User: " << name << " (ID: " << id << ") Role: " << (role == Role::STUDENT ? "Student" : "Instructor") << "\n";
    }
};

class Student : public User {
    vector<string> enrolledCourseIds;
public:
    Student(const string &id_ = "", const string &name_ = "") : User(id_, name_, Role::STUDENT) {}
    void enroll(const string &courseId) { enrolledCourseIds.push_back(courseId); }
    const vector<string>& getEnrollments() const { return enrolledCourseIds; }
    void display() const override {
        cout << "Student: " << name << " (ID: " << id << ")\n";
        cout << "Enrolled courses: ";
        if (enrolledCourseIds.empty()) cout << "None";
        else {
            for (auto &c : enrolledCourseIds) cout << c << " ";
        }
        cout << "\n";
    }
};

class Instructor : public User {
public:
    Instructor(const string &id_ = "", const string &name_ = "") : User(id_, name_, Role::INSTRUCTOR) {}
    void display() const override {
        cout << "Instructor: " << name << " (ID: " << id << ")\n";
    }
};

// ==============================
// CourseManager: manages multiple courses
// Operator overloading: += to add a course
// friend function printSummary to show manager content
// ==============================
class CourseManager; // forward

void printSummary(const CourseManager &mgr); // friend declaration

class CourseManager {
    map<string, Course> courses; // keyed by id
public:
    CourseManager() = default;
    ~CourseManager() = default;

    void addCourse(Course &&c) {
        string id = c.getId();
        courses[id] = move(c);
    }

    // operator overload: add by reference
    CourseManager& operator+=(Course &&c) {
        addCourse(move(c));
        return *this;
    }

    bool hasCourse(const string &id) const {
        return courses.find(id) != courses.end();
    }
    Course* getCoursePtr(const string &id) {
        auto it = courses.find(id);
        if (it == courses.end()) return nullptr;
        return &it->second;
    }
    void displayAll() const {
        cout << "==== All Courses (" << courses.size() << ") ====\n";
        for (const auto &kv : courses) {
            cout << "- " << kv.first << ": " << kv.second.getTitle() << "\n";
        }
    }
    // friend function to print summary
    friend void printSummary(const CourseManager &mgr);

    // persistence helpers: save all
    bool saveToFile(const string &filename) const {
        ofstream ofs(filename, ios::trunc);
        if (!ofs) return false;
        for (const auto &kv : courses) {
            ofs << kv.second.serialize();
        }
        return true;
    }
    bool loadFromFile(const string &filename) {
        ifstream ifs(filename);
        if (!ifs) return false;
        courses.clear();
        string line;
        while (getline(ifs, line)) {
            if (line.rfind("COURSE|", 0) == 0) {
                // Put the line back into a stringstream and call deserialize
                // We'll create an istringstream that contains the header and the subsequent lines
                // But deserialize expects the first line to be COURSE|... so we push back the header and then pass ifs
                // Trick: create a temporary stringstream combining this header and the rest via the file stream reading directly
                // Simpler: we'll create temporary vector of lines.
                // To keep it simple we use the file pointer: move stream back by the length of line + newline? That is messy.
                // Instead, re-open file and parse with while loop that checks for COURSE
                // So break here and reparse entire file properly.
                break;
            }
        }
        ifs.clear();
        ifs.seekg(0);
        while (ifs.peek() != EOF) {
            string header;
            if (!getline(ifs, header)) break;
            if (header.rfind("COURSE|", 0) != 0) continue;
            // Use a stringstream approach: combine header + following lines until ENDCOURSE
            string combined = header + "\n";
            string line2;
            while (getline(ifs, line2)) {
                combined += line2 + "\n";
                if (line2 == "ENDCOURSE") break;
            }
            istringstream iss(combined);
            Course c = Course::deserialize(iss);
            courses[c.getId()] = move(c);
        }
        return true;
    }
};

void printSummary(const CourseManager &mgr) {
    cout << "\n--- CourseManager Summary ---\n";
    cout << "Total courses: " << mgr.courses.size() << "\n";
    for (const auto &kv : mgr.courses) {
        cout << "Course ID: " << kv.first << " Title: " << kv.second.getTitle() << "\n";
    }
    cout << "-----------------------------\n";
}

// ==============================
// FileManager: wraps persistence for courses and enrollments
// ==============================
class FileManager {
public:
    static bool saveCourses(const CourseManager &mgr, const string &filename) {
        return mgr.saveToFile(filename);
    }
    static bool loadCourses(CourseManager &mgr, const string &filename) {
        return mgr.loadFromFile(filename);
    }

    // Simple enrollments file
    static bool saveEnrollments(const vector< SimplePair<string,string> > &enrs, const string &filename) {
        ofstream ofs(filename, ios::trunc);
        if (!ofs) return false;
        for (auto &p : enrs) {
            ofs << p.first << "|" << p.second << "\n"; // studentId|courseId
        }
        return true;
    }
    static bool loadEnrollments(vector< SimplePair<string,string> > &enrs, const string &filename) {
        ifstream ifs(filename);
        if (!ifs) return false;
        enrs.clear();
        string line;
        while (getline(ifs, line)) {
            auto pos = line.find('|');
            if (pos == string::npos) continue;
            enrs.emplace_back(line.substr(0,pos), line.substr(pos+1));
        }
        return true;
    }
};

// ==============================
// EnrollmentManager (associates students with courses)
// Demonstrates pointers, vector usage, and association
// ==============================
class EnrollmentManager {
    vector< SimplePair<string, string> > enrollments; // studentId, courseId
public:
    void enrollStudent(const string &studentId, const string &courseId) {
        enrollments.emplace_back(studentId, courseId);
    }
    bool isEnrolled(const string &studentId, const string &courseId) const {
        for (const auto &p : enrollments) if (p.first == studentId && p.second == courseId) return true;
        return false;
    }
    const vector< SimplePair<string,string> >& getEnrollments() const { return enrollments; }

    // persist
    bool save(const string &filename) const {
        return FileManager::saveEnrollments(enrollments, filename);
    }
    bool load(const string &filename) {
        return FileManager::loadEnrollments(const_cast<vector<SimplePair<string,string>>&>(enrollments), filename);
    }
};

// ==============================
// Friend function example outside class
// (we already used printSummary as friend of CourseManager)
// ==============================

// ==============================
// CLI / Main
// ==============================
int main() {
    CourseManager manager;
    EnrollmentManager enrollMgr;

    // sample users created via pointers to demonstrate pointer usage and polymorphism
    vector< unique_ptr<User> > users;
    users.push_back( make_unique<Student>("s001", "Alice") );
    users.push_back( make_unique<Student>("s002", "Bob") );
    users.push_back( make_unique<Instructor>("i001", "Dr. Smith") );

    // load existing data if present
    const string coursesFile = "courses.db";
    const string enrollFile = "enrollments.db";
    if (FileManager::loadCourses(manager, coursesFile)) {
        cout << "Loaded courses from " << coursesFile << "\n";
    }
    enrollMgr.load(enrollFile); // ignore return

    int choice;
    do {
        cout << "\n====== Online Course Management ======\n";
        cout << "1. Create Course (Instructor)\n";
        cout << "2. Add Segment to Course\n";
        cout << "3. Display Course\n";
        cout << "4. List All Courses\n";
        cout << "5. Enroll Student in Course\n";
        cout << "6. Display Student Enrollment\n";
        cout << "7. Save Data\n";
        cout << "8. Load Data\n";
        cout << "9. Print Manager Summary (friend func)\n";
        cout << "0. Exit\n";
        cout << "Choose: ";
        if (!(cin >> choice)) {
            cin.clear();
            string dummy;
            getline(cin, dummy);
            cout << "Invalid input.\n";
            continue;
        }
        cin.ignore();

        if (choice == 1) {
            // Create new course
            Course c;
            string id, title, dur, offer, topic, outline, progress;
            int price;
            bool cert;
            cout << "Course ID: ";
            getline(cin, id);
            cout << "Title: ";
            getline(cin, title);
            cout << "Duration (e.g., 4 weeks): ";
            getline(cin, dur);
            cout << "Price: ";
            cin >> price;
            cin.ignore();
            cout << "Offer: ";
            getline(cin, offer);
            cout << "Topic: ";
            getline(cin, topic);
            cout << "Outline: ";
            getline(cin, outline);
            cout << "Progress (e.g., 0%): ";
            getline(cin, progress);
            cout << "Certificate available? (1=Yes 0=No): ";
            cin >> cert;
            cin.ignore();
            c.setId(id);
            c.setTitle(title);
            c.setDurationStr(dur);
            c.setPrice(price);
            c.setOffer(offer);
            c.setTopic(topic);
            c.setOutline(outline);
            c.setProgress(progress);
            c.setCertificate(cert);

            // add a default segment to show composition
            auto seg = make_unique<VideoSegment>("Intro Video", 10, "https://example.com/intro");
            c.addSegment(move(seg));

            // use operator+=
            manager += move(c);
            cout << "Course created and added.\n";
        } else if (choice == 2) {
            string cid;
            cout << "Course ID to add segment to: ";
            getline(cin, cid);
            Course* cp = manager.getCoursePtr(cid);
            if (!cp) { cout << "Course not found.\n"; continue; }

            cout << "Segment type (1=Video, 2=Quiz, else=Generic): ";
            int t; cin >> t; cin.ignore();
            string title;
            int dur;
            cout << "Segment title: ";
            getline(cin, title);
            cout << "Duration minutes: ";
            cin >> dur;
            cin.ignore();
            if (t == 1) {
                string url;
                cout << "Video URL: "; getline(cin, url);
                cp->addSegment( make_unique<VideoSegment>(title, dur, url) );
            } else if (t == 2) {
                int qs;
                cout << "Number of questions: "; cin >> qs; cin.ignore();
                cp->addSegment( make_unique<QuizSegment>(title, dur, qs) );
            } else {
                cp->addSegment( make_unique<Segment>(title, dur) );
            }
            cout << "Segment added.\n";
        } else if (choice == 3) {
            string cid;
            cout << "Course ID to display: ";
            getline(cin, cid);
            Course* cp = manager.getCoursePtr(cid);
            if (!cp) { cout << "Course not found.\n"; continue; }
            cp->display();
        } else if (choice == 4) {
            manager.displayAll();
        } else if (choice == 5) {
            string sid, cid;
            cout << "Student ID: "; getline(cin, sid);
            cout << "Course ID: "; getline(cin, cid);
            // assume student exists in users vector; else create and add
            bool found = false;
            for (auto &u : users) {
                if (u->getId() == sid) {
                    if (u->getRole() != Role::STUDENT) {
                        cout << "User exists but is not a student.\n";
                        found = true;
                        break;
                    }
                    // enroll pointer cast
                    Student *sp = dynamic_cast<Student*>(u.get());
                    if (sp) sp->enroll(cid);
                    break;
                }
            }
            // if not found, create student pointer (demonstrate new pointer usage)
            bool studentExists = false;
            for (auto &u : users) if (u->getId() == sid) studentExists = true;
            if (!studentExists) {
                // raw pointer example (educational) then wrap into unique_ptr
                Student *raw = new Student(sid, "NewStudent_" + sid);
                unique_ptr<User> up(raw);
                up->display();
                // enroll
                Student *sp = dynamic_cast<Student*>(up.get());
                if (sp) sp->enroll(cid);
                users.push_back(move(up));
            }
            // record in enrollment manager
            enrollMgr.enrollStudent(sid, cid);
            cout << "Enrollment recorded.\n";
        } else if (choice == 6) {
            string sid;
            cout << "Student ID to display: ";
            getline(cin, sid);
            bool printed = false;
            for (const auto &u : users) {
                if (u->getId() == sid) {
                    u->display();
                    printed = true;
                    break;
                }
            }
            if (!printed) {
                cout << "Student not found in memory, checking enrollments file...\n";
                // list enrollments for this student
                for (const auto &p : enrollMgr.getEnrollments()) {
                    if (p.first == sid) cout << "- Enrolled in: " << p.second << "\n";
                }
            }
        } else if (choice == 7) {
            bool ok1 = FileManager::saveCourses(manager, coursesFile);
            bool ok2 = enrollMgr.save(enrollFile);
            cout << "Save courses: " << (ok1 ? "OK" : "Failed") << ", enrollments: " << (ok2 ? "OK" : "Failed") << "\n";
        } else if (choice == 8) {
            if (FileManager::loadCourses(manager, coursesFile)) {
                cout << "Courses loaded.\n";
            } else {
                cout << "Failed to load courses.\n";
            }
            if (enrollMgr.load(enrollFile)) {
                cout << "Enrollments loaded.\n";
            } else {
                cout << "No enrollments file or failed to load.\n";
            }
        } else if (choice == 9) {
            // friend function example
            printSummary(manager);
        } else if (choice == 0) {
            cout << "Exiting program...\n";
        } else {
            cout << "Invalid choice.\n";
        }

    } while (choice != 0);

    // final save on exit
    FileManager::saveCourses(manager, coursesFile);
    enrollMgr.save(enrollFile);

    return 0;
}
