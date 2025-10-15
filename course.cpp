#include "course.h"
#include <sstream>
#include <fstream>
#include <iostream>

using namespace std;

// Segment implementation
Segment::Segment(const std::string &t, int d) : title(t), durationMinutes(d) {}
Segment::~Segment() = default;
void Segment::display() const {
    cout << "Segment: " << title << " (" << durationMinutes << " min)\n";
}
std::string Segment::serialize() const {
    ostringstream oss;
    oss << "Segment|" << title << "|" << durationMinutes;
    return oss.str();
}

std::unique_ptr<Segment> Segment::deserialize(const std::string &line) {
    vector<string> parts;
    string tmp;
    istringstream iss(line);
    while (getline(iss, tmp, '|')) parts.push_back(tmp);
    if (parts.size() < 3) return nullptr;
    string type = parts[0];
    string t = parts[1];
    int d = stoi(parts[2]);
    if (type == "VideoSegment" && parts.size() >= 4) return make_unique<VideoSegment>(t, d, parts[3]);
    if (type == "QuizSegment" && parts.size() >= 4) return make_unique<QuizSegment>(t, d, stoi(parts[3]));
    return make_unique<Segment>(t, d);
}

// VideoSegment
VideoSegment::VideoSegment(const std::string &t, int d, const std::string &url) : Segment(t, d), videoUrl(url) {}
void VideoSegment::display() const { cout << "Video: " << title << " (" << durationMinutes << " min) - URL: " << videoUrl << "\n"; }
std::string VideoSegment::serialize() const {
    ostringstream oss;
    oss << "VideoSegment|" << title << "|" << durationMinutes << "|" << videoUrl;
    return oss.str();
}

// QuizSegment
QuizSegment::QuizSegment(const std::string &t, int d, int q) : Segment(t, d), questions(q) {}
void QuizSegment::display() const { cout << "Quiz: " << title << " (" << durationMinutes << " min) - Qs: " << questions << "\n"; }
std::string QuizSegment::serialize() const {
    ostringstream oss;
    oss << "QuizSegment|" << title << "|" << durationMinutes << "|" << questions;
    return oss.str();
}

// Course
Course::Course(const std::string &id_, const std::string &title_, const std::string &duration_,
               int price_, const std::string &offer_, const std::string &topic_,
               const std::string &outline_, const std::string &progress_, bool certificate_)
    : id(id_), title(title_), duration(duration_), price(price_), offer(offer_), topic(topic_),
      outline(outline_), progress(progress_), certificate(certificate_) {}

void Course::setId(const std::string &i) { id = i; }
std::string Course::getId() const { return id; }
void Course::setTitle(const std::string &t) { title = t; }
std::string Course::getTitle() const { return title; }
void Course::setDurationStr(const std::string &d) { duration = d; }
std::string Course::getDurationStr() const { return duration; }
void Course::setPrice(int p) { price = p; }
int Course::getPrice() const { return price; }
void Course::setOffer(const std::string &o) { offer = o; }
std::string Course::getOffer() const { return offer; }
void Course::setTopic(const std::string &t) { topic = t; }
std::string Course::getTopic() const { return topic; }
void Course::setOutline(const std::string &o) { outline = o; }
std::string Course::getOutline() const { return outline; }
void Course::setProgress(const std::string &p) { progress = p; }
std::string Course::getProgress() const { return progress; }
void Course::setCertificate(bool c) { certificate = c; }
bool Course::hasCertificate() const { return certificate; }

void Course::addSegment(std::unique_ptr<Segment> seg) { segments.push_back(move(seg)); }
void Course::display() const {
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
    cout << "Segments:\n";
    for (const auto &s : segments) s->display();
    cout << "========================================\n";
}

std::string Course::serialize() const {
    ostringstream oss;
    oss << "COURSE|" << id << "|" << title << "|" << duration << "|" << price << "|" << offer << "|" << topic << "|"
        << outline << "|" << progress << "|" << (certificate ? "1" : "0") << "\n";
    for (const auto &s : segments) oss << s->serialize() << "\n";
    oss << "ENDCOURSE\n";
    return oss.str();
}

Course Course::deserialize(std::istream &in) {
    string header;
    getline(in, header);
    vector<string> parts;
    string tmp;
    istringstream hh(header);
    while (getline(hh, tmp, '|')) parts.push_back(tmp);
    if (parts.size() < 10) throw runtime_error("Bad course header");
    Course c(parts[1], parts[2], parts[3], stoi(parts[4]), parts[5], parts[6], parts[7], parts[8], parts[9] == "1");
    string line;
    while (getline(in, line)) {
        if (line == "ENDCOURSE") break;
        auto seg = Segment::deserialize(line);
        if (seg) c.addSegment(move(seg));
    }
    return c;
}

// CourseManager

void CourseManager::addCourse(Course &&c) {
    string id = c.getId();
    courses[id] = move(c);
}

CourseManager& CourseManager::operator+=(Course &&c) {
    addCourse(move(c));
    return *this;
}

bool CourseManager::hasCourse(const std::string &id) const {
    return courses.find(id) != courses.end();
}

Course* CourseManager::getCoursePtr(const std::string &id) {
    auto it = courses.find(id);
    if (it == courses.end()) return nullptr;
    return &it->second;
}

void CourseManager::displayAll() const {
    cout << "==== All Courses (" << courses.size() << ") ====\n";
    for (const auto &kv : courses) {
        cout << "- " << kv.first << ": " << kv.second.getTitle() << "\n";
    }
}

bool CourseManager::saveToFile(const std::string &filename) const {
    ofstream ofs(filename, ios::trunc);
    if (!ofs) return false;
    for (const auto &kv : courses) ofs << kv.second.serialize();
    return true;
}

bool CourseManager::loadFromFile(const std::string &filename) {
    ifstream ifs(filename);
    if (!ifs) return false;
    courses.clear();
    string line;
    while (getline(ifs, line)) {
        if (line.rfind("COURSE|", 0) != 0) continue;
        // collect course block
        string combined = line + "\n";
        while (getline(ifs, line)) {
            combined += line + "\n";
            if (line == "ENDCOURSE") break;
        }
        istringstream iss(combined);
        Course c = Course::deserialize(iss);
        courses[c.getId()] = move(c);
    }
    return true;
}

void printSummary(const CourseManager &mgr) {
    cout << "\n--- CourseManager Summary ---\n";
    cout << "Total courses: " << mgr.courses.size() << "\n";
    for (const auto &kv : mgr.courses) cout << "Course ID: " << kv.first << " Title: " << kv.second.getTitle() << "\n";
    cout << "-----------------------------\n";
}
