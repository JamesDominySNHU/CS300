#include <algorithm>
#include <iostream>
#include "ProjectTwo.hpp"

int main(int argc, char* argv[]) {
    commandLoop(); // Start the main program loop
}

void Courses::addCourse(const Course course) {
    if (getSize() == 0) {
        append(course); // If adding the first element, push it to the end
    }
    else if (at(getSize() - 1).getId().compare(course.getId()) < 0) {
        append(course); // If the last element is smaller than the new one, push it to the end
    }
    else {
        size_t insertionPoint;
        insertionPoint = LinkedList(course.getId()); // Find the insertion point using binary search
        if (insertionPoint != SIZE_MAX) {
            insert(insertionPoint, course); // Insert the new course at the appropriate position
        }
    }
}

size_t Courses::LinkedList(const string key) const {
    size_t midPoint = SIZE_MAX;
    size_t lowPoint = 0;
    size_t highPoint = getSize() - 1;

    while (highPoint != SIZE_MAX && highPoint >= lowPoint) {
        midPoint = (highPoint + lowPoint) / 2; // Locate the midpoint for binary search
        Course course = at(midPoint); // Get the value at the midpoint
        if (course.getId() < key) {
            lowPoint = midPoint + 1; // Adjust the search range to the lower half
        }
        else if (course.getId() > key) {
            highPoint = midPoint - 1; // Adjust the search range to the upper half
        }
        else {
            return midPoint; // Found a match
        }
    }

    return midPoint; // Return the closest point for insertion if not found
}

void Courses::printAll() const {
    for (size_t i = 0; i < getSize(); ++i) {
        Course course = at(i);
        cout << course.getId() << " " << course.getTitle() << endl; // Print sorted list of courses by ID and title
    }
}

void Courses::printCourse(const string courseId) const {
    Course course = findCourse(courseId);
    if (course.getId().compare(courseId) == 0) {
        course.print(); // Print details of the found course
    }
    else {
        cout << "Could not find course: " << courseId << endl; // Indicate if the course is not found
    }
}

void Courses::loadFromCSV(const string filename, const string deliminator /* = "," */) {
    string line;
    int lineNumber = 0;
    ifstream inputFile(filename.c_str());

    if (inputFile.is_open()) {
        while (inputFile.good()) {
            getline(inputFile, line);
            if (line != "") {
                ++lineNumber;
                auto tokens = splitString(line, deliminator); // Split the CSV line into tokens
                if (tokens.size() < 2) {
                    cout << "Error: Line format error, expected 2 or more tokens, got ";
                    cout << tokens.size() << " tokens, line number: " << lineNumber << endl;
                    continue;
                }
                Course course;
                course.setId(tokens[0]);
                Course duplicateCourse = findCourse(course.getId()); // Check for duplicate courses
                if (duplicateCourse.getId() != "") {
                    cout << "Warning: Duplicate course detected, line number: ";
                    cout << lineNumber << endl;
                    continue;
                }
                course.setTitle(tokens[1]);
                for (size_t i = 2; i < tokens.size(); ++i) {
                    string prerequisiteId = tokens.at(i);
                    Course courseExists = findCourse(prerequisiteId); // Check if prerequisite course exists
                    if (courseExists.getId() == "") {
                        cout << "Warning: Prerequisite not found, course: " << course.getId();
                        cout << ", prerequisite: " << prerequisiteId << endl;
                        continue;
                    }
                    course.addPrerequisite(courseExists);
                }
                addCourse(course); // Add the course to the data structure
            }
        }
        inputFile.close();
        if (lineNumber == 0)
            throw runtime_error("No Data in " + filename);
    }
    else
        throw runtime_error("Failed to open " + filename);
}

Course Courses::findCourse(const string courseId) const {
    size_t index = LinkedList(courseId); // Find the index of the course
    if (index != SIZE_MAX) {
        Course course = at(index);
        if (course.getId().compare(courseId) == 0) {
            return course; // Return the found course
        }
        else {
            return Course(); // Return an empty course object if not found
        }
    }
    else {
        return Course(); // Return an empty course object if not found
    }
}

vector<string> Courses::splitString(const string input, const string delimiter) const {
    string temp = input;
    size_t pos = 0;
    string token;
    vector<string> returnValue;

    while ((pos = temp.find(delimiter)) != std::string::npos) {
        token = temp.substr(0, pos); // Extract the token from input
        returnValue.push_back(token); // Add the token to the vector
        temp.erase(0, pos + delimiter.length()); // Remove the token from the input for the next call
    }

    if (temp.length() > 0) {
        returnValue.push_back(temp); // Add the last token to the vector
    }

    return returnValue;
}

void Course::print() const {
    cout << id << ", " << title;

    if (prerequisites.size() > 0) {
        cout << endl << "Prerequisites: ";
        for (size_t i = 0; i < prerequisites.size(); ++i) {
            cout << prerequisites.at(i).getId();
            if (i + 1 < prerequisites.size()) {
                cout << ", ";
            }
        }
    }

    cout << endl;
}

void printMenu(void) {
    cout << "Menu:" << endl;
    cout << " 1. Load Data Structure." << endl;
    cout << " 2. Print Course List." << endl;
    cout << " 3. Print Course." << endl;
    cout << " 9. Exit" << endl << endl;
    cout << "What would you like to do? ";
}

void handleChoice(int choice, Courses& courses) {
    switch (choice) {
    case 1:
    {
        string filename;
        auto state = cin.exceptions();
        cin.exceptions(std::istream::failbit | state);
        cout << "Please enter the name of the data file to load." << endl;
        cin.ignore();
        if (courses.getSize() > 0) {
            courses.clear();
        }
        try {
            getline(cin, filename);
            courses.loadFromCSV(filename);
        }
        catch (exception ex) {
            cout << ex.what() << endl;
        }
        cin.exceptions(state);
    }
    break;
    case 2:
        cout << "Here is a sample schedule:" << endl << endl;
        courses.printAll();
        break;
    case 3:
    {
        string courseId;
        cout << "What course do you want to know about? ";
        cin >> courseId;
        courses.printCourse(courseId);
    }
    break;
    case 9:
        cout << "Thank you for using the course planner!" << endl;
        break;
    default:
        cout << choice << " is not a valid option." << endl << endl;
        break;
    }
}

void commandLoop(void) {
    int choice = 0;
    Courses courses;
    cout << "Welcome to the course planner." << endl;
    while (choice != 9) {
        cout << endl;
        printMenu();
        cin >> choice;
        handleChoice(choice, courses);
    }
}