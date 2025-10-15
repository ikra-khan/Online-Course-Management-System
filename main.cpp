#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include "user.h"
#include "course.h"
#include "content.h"
#include "admin.h"

using namespace std;

int main() {
    CourseManager manager;
    EnrollmentManager enrollMgr;
    Content courseContent;

    const string coursesFile = "courses.db";
    const string enrollFile = "enrollments.db";
    const string contentFile = "content.txt";

    // sample users
    vector< unique_ptr<User> > users;
    users.push_back( make_unique<Student>("s001", "Alice") );
    users.push_back( make_unique<Student>("s002", "Bob") );
    users.push_back( make_unique<Instructor>("i001", "Dr. Smith") );

    // load existing
    if (manager.loadFromFile(coursesFile)) cout << "Loaded courses from " << coursesFile << "\n";
    enrollMgr.load(enrollFile);
    courseContent.loadFromFile(contentFile);

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
        cout << "10. Manage Course Content (lectures/videos/notes)\n";
        cout << "0. Exit\n";
        cout << "Choose: ";
        if (!(cin >> choice)) {
            cin.clear(); string dummy; getline(cin, dummy);
            cout << "Invalid input.\n"; continue;
        }
        cin.ignore();

        if (choice == 1) {
            Course c;
            string id, title, dur, offer, topic, outline, progress;
            int price; bool cert;
            cout << "Course ID: "; getline(cin, id);
            cout << "Title: "; getline(cin, title);
            cout << "Duration (e.g., 4 weeks): "; getline(cin, dur);
            cout << "Price: "; cin >> price; cin.ignore();
            cout << "Offer: "; getline(cin, offer);
            cout << "Topic: "; getline(cin, topic);
            cout << "Outline: "; getline(cin, outline);
            cout << "Progress (e.g., 0%): "; getline(cin, progress);
            cout << "Certificate available? (1=Yes 0=No): "; cin >> cert; cin.ignore();

            c.setId(id); c.setTitle(title); c.setDurationStr(dur); c.setPrice(price);
            c.setOffer(offer); c.setTopic(topic); c.setOutline(outline);
            c.setProgress(progress); c.setCertificate(cert);

            c.addSegment( make_unique<VideoSegment>("Intro Video", 10, "https://example.com/intro") );
            manager += move(c);
            cout << "Course created and added.\n";
        } else if (choice == 2) {
            string cid; cout << "Course ID to add segment to: "; getline(cin, cid);
            Course* cp = manager.getCoursePtr(cid);
            if (!cp) { cout << "Course not found.\n"; continue; }
            cout << "Segment type (1=Video, 2=Quiz, else=Generic): ";
            int t; cin >> t; cin.ignore();
            string title; int dur;
            cout << "Segment title: "; getline(cin, title);
            cout << "Duration minutes: "; cin >> dur; cin.ignore();
            if (t == 1) {
                string url; cout << "Video URL: "; getline(cin, url);
                cp->addSegment( make_unique<VideoSegment>(title, dur, url) );
            } else if (t == 2) {
                int qs; cout << "Number of questions: "; cin >> qs; cin.ignore();
                cp->addSegment( make_unique<QuizSegment>(title, dur, qs) );
            } else cp->addSegment( make_unique<Segment>(title, dur) );
            cout << "Segment added.\n";
        } else if (choice == 3) {
            string cid; cout << "Course ID to display: "; getline(cin, cid);
            Course* cp = manager.getCoursePtr(cid);
            if (!cp) { cout << "Course not found.\n"; continue; }
            cp->display();
        } else if (choice == 4) {
            manager.displayAll();
        } else if (choice == 5) {
            string sid, cid; cout << "Student ID: "; getline(cin, sid); cout << "Course ID: "; getline(cin, cid);
            bool exists = false;
            for (auto &u : users) if (u->getId() == sid) { exists = true; if (u->getRole() != Role::STUDENT) cout << "User exists but not a student.\n"; else dynamic_cast<Student*>(u.get())->enroll(cid); break; }
            if (!exists) {
                Student *raw = new Student(sid, "NewStudent_" + sid); unique_ptr<User> up(raw);
                dynamic_cast<Student*>(up.get())->enroll(cid);
                users.push_back(move(up));
            }
            enrollMgr.enrollStudent(sid, cid);
            cout << "Enrollment recorded.\n";
        } else if (choice == 6) {
            string sid; cout << "Student ID to display: "; getline(cin, sid);
            bool printed = false;
            for (const auto &u : users) {
                if (u->getId() == sid) { u->display(); printed = true; break; }
            }
            if (!printed) {
                cout << "Student not found in memory, checking enrollments:\n";
                for (const auto &p : enrollMgr.getEnrollments()) if (p.first == sid) cout << "- " << p.second << "\n";
            }
        } else if (choice == 7) {
            bool ok1 = manager.saveToFile(coursesFile);
            bool ok2 = enrollMgr.save(enrollFile);
            bool ok3 = courseContent.saveToFile(contentFile);
            cout << "Save courses: " << (ok1 ? "OK" : "Failed") << ", enrollments: " << (ok2 ? "OK" : "Failed")
                 << ", content: " << (ok3 ? "OK" : "Failed") << "\n";
        } else if (choice == 8) {
            if (manager.loadFromFile(coursesFile)) cout << "Courses loaded.\n"; else cout << "Failed to load courses.\n";
            if (enrollMgr.load(enrollFile)) cout << "Enrollments loaded.\n"; else cout << "No enrollments or failed.\n";
            if (courseContent.loadFromFile(contentFile)) cout << "Content loaded.\n"; else cout << "No content file or failed.\n";
        } else if (choice == 9) {
            printSummary(manager);
        } else if (choice == 10) {
            cout << "Content Manager Menu\n1. Add Lecture\n2. Add Video\n3. Add Note\n4. Add Slide\n5. Add Book\n6. Add Assignment\n7. Display Content\nChoose: ";
            int cch; if (!(cin >> cch)) { cin.clear(); string d; getline(cin,d); cout<<"Invalid\n"; continue; }
            cin.ignore();
            string s;
            if (cch == 1) { cout << "Lecture title: "; getline(cin,s); courseContent.addLecture(s); }
            else if (cch == 2) { cout << "Video title/URL: "; getline(cin,s); courseContent.addVideo(s); }
            else if (cch == 3) { cout << "Note: "; getline(cin,s); courseContent.addNote(s); }
            else if (cch == 4) { cout << "Slide: "; getline(cin,s); courseContent.addSlide(s); }
            else if (cch == 5) { cout << "Book: "; getline(cin,s); courseContent.addBook(s); }
            else if (cch == 6) { cout << "Assignment: "; getline(cin,s); courseContent.addAssignment(s); }
            else if (cch == 7) courseContent.displayAll();
            else cout << "Invalid\n";
        } else if (choice == 0) {
            cout << "Exiting program...\n";
        } else cout << "Invalid choice.\n";
    } while (choice != 0);

    // final save
    manager.saveToFile(coursesFile);
    enrollMgr.save(enrollFile);
    courseContent.saveToFile(contentFile);
    return 0;
}
