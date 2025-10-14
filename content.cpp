#include <iostream>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

class Content {
private:
    vector<string> lectures;
    vector<string> videos;
    vector<string> notes;
    vector<string> slides;
    vector<string> books;
    vector<string> assignments;

public:
    // Add items
    void addLecture(const string &lecture) { lectures.push_back(lecture); }
    void addVideo(const string &video) { videos.push_back(video); }
    void addNote(const string &note) { notes.push_back(note); }
    void addSlide(const string &slide) { slides.push_back(slide); }
    void addBook(const string &book) { books.push_back(book); }
    void addAssignment(const string &assignment) { assignments.push_back(assignment); }

    // Display all content
    void displayAll() const {
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

    // Save to file
    bool saveToFile(const string &filename) const {
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

    // Load from file
    bool loadFromFile(const string &filename) {
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
};

int main() {
    Content c;
    cout << "Welcome to Course Content Manager!\n";
    cout << "---------------------------------\n";

    cout << "Do you want to load existing data from file? (y/n): ";
    char loadChoice;
    cin >> loadChoice;
    cin.ignore();

    if (loadChoice == 'y' || loadChoice == 'Y') {
        if (c.loadFromFile("content.txt"))
            cout << "Data loaded successfully!\n";
        else
            cout << "No file found or failed to load.\n";
    }

    cout << "\nEnter number of lectures: ";
    int n;
    cin >> n;
    cin.ignore();
    for (int i = 0; i < n; i++) {
        string lec;
        cout << "Enter lecture " << i + 1 << ": ";
        getline(cin, lec);
        c.addLecture(lec);
    }

    cout << "\nEnter number of videos: ";
    cin >> n;
    cin.ignore();
    for (int i = 0; i < n; i++) {
        string vid;
        cout << "Enter video " << i + 1 << ": ";
        getline(cin, vid);
        c.addVideo(vid);
    }

    cout << "\nEnter number of notes: ";
    cin >> n;
    cin.ignore();
    for (int i = 0; i < n; i++) {
        string note;
        cout << "Enter note " << i + 1 << ": ";
        getline(cin, note);
        c.addNote(note);
    }

    cout << "\nEnter number of slides: ";
    cin >> n;
    cin.ignore();
    for (int i = 0; i < n; i++) {
        string slide;
        cout << "Enter slide " << i + 1 << ": ";
        getline(cin, slide);
        c.addSlide(slide);
    }

    cout << "\nEnter number of books: ";
    cin >> n;
    cin.ignore();
    for (int i = 0; i < n; i++) {
        string book;
        cout << "Enter book " << i + 1 << ": ";
        getline(cin, book);
        c.addBook(book);
    }

    cout << "\nEnter number of assignments: ";
    cin >> n;
    cin.ignore();
    for (int i = 0; i < n; i++) {
        string assign;
        cout << "Enter assignment " << i + 1 << ": ";
        getline(cin, assign);
        c.addAssignment(assign);
    }

    cout << "\nDo you want to save to file? (y/n): ";
    char ch;
    cin >> ch;
    if (ch == 'y' || ch == 'Y') {
        if (c.saveToFile("content.txt"))
            cout << "Saved to content.txt successfully!\n";
        else
            cout << "Error saving to file!\n";
    }

    cout << "\nDisplaying all content:\n";
    c.displayAll();

    return 0;
}
