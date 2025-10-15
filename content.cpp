#include "content.h"
#include <fstream>
#include <iostream>

using namespace std;

void Content::addLecture(const std::string &lecture) { lectures.push_back(lecture); }
void Content::addVideo(const std::string &video) { videos.push_back(video); }
void Content::addNote(const std::string &note) { notes.push_back(note); }
void Content::addSlide(const std::string &slide) { slides.push_back(slide); }
void Content::addBook(const std::string &book) { books.push_back(book); }
void Content::addAssignment(const std::string &assignment) { assignments.push_back(assignment); }

void Content::displayAll() const {
    cout << "\n===== COURSE CONTENT DETAILS =====\n";
    auto printSection = [&](const string &name, const vector<string> &vec) {
        cout << "\n" << name << ":\n";
        if (vec.empty()) cout << "  (none)\n";
        else for (const auto &s : vec) cout << "  - " << s << "\n";
    };
    printSection("Lectures", lectures);
    printSection("Videos", videos);
    printSection("Notes", notes);
    printSection("Slides", slides);
    printSection("Books", books);
    printSection("Assignments", assignments);
}

bool Content::saveToFile(const std::string &filename) const {
    ofstream ofs(filename);
    if (!ofs) return false;
    auto writeSection = [&](const string &tag, const vector<string> &vec) {
        ofs << "#" << tag << '\n';
        for (const auto &s : vec) ofs << s << '\n';
    };
    writeSection("LECTURES", lectures);
    writeSection("VIDEOS", videos);
    writeSection("NOTES", notes);
    writeSection("SLIDES", slides);
    writeSection("BOOKS", books);
    writeSection("ASSIGNMENTS", assignments);
    ofs << "#END\n";
    return true;
}

bool Content::loadFromFile(const std::string &filename) {
    ifstream ifs(filename);
    if (!ifs) return false;
    vector<string> lines;
    string line;
    while (getline(ifs, line)) lines.push_back(line);
    auto getBlock = [&](const string &tag) {
        vector<string> out;
        string header = "#" + tag;
        bool inBlock = false;
        for (const auto &L : lines) {
            if (L == header) { inBlock = true; continue; }
            if (inBlock) {
                if (!L.empty() && L[0] == '#') break;
                out.push_back(L);
            }
        }
        return out;
    };
    lectures = getBlock("LECTURES");
    videos = getBlock("VIDEOS");
    notes = getBlock("NOTES");
    slides = getBlock("SLIDES");
    books = getBlock("BOOKS");
    assignments = getBlock("ASSIGNMENTS");
    return true;
}
