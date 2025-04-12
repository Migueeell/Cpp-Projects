/**************************************************************
 * University Entrance Application Tracker - Data Analysis System
 * 
 * Description:
 *   Simulates a university application system with data analysis
 *   capabilities. Processes student applications, preferences, and 
 *   exam scores to assign scholarships and generate insights.
 * 
 * Key Features:
 *   - Manages student/university data using structs and 2D arrays
 *   - Analyzes application patterns and first-choice preferences
 *   - Awards scholarships based on merit and available seats
 *   - Implements file I/O (text + binary) for data persistence
 *   - Includes search/sort functionality for efficient queries
 * 
 * Data Structures:
 *   - Student/University structs
 *   - 2D application matrix (students x universities)
 *   - Sorted student array (by name)
 * 
 * File Handling:
 *   - Input: students.txt, universities.txt, applications.txt  
 *   - Output: final_students.bin (binary student records)
 **************************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

struct Student {
    int id;
    string name;
    int preferences[3];
    int examScore;
    bool gotScholarship = false;
};

struct University {
    int id;
    string name;
    string location;
    int availableSeats;
};

const int NUM_STUDENTS = 10;
const int NUM_UNIS = 5;

void insertStudentSorted(Student students[], int count, Student newStudent) {
    int i = count-1;

    while (i >= 0 && students[i].name>newStudent.name) {
        students[i+1] = students[i];
        i--;
    }
    students[i+1] = newStudent;
}

int loadStudents(Student students[]){
    
    ifstream infile("students.txt");
    if (!infile){
        cout<<"Error opening students.txt"<<endl;
        return 0;
    }

    int count = 0;
    Student s;
    srand(time(0));

    while (infile>>s.id>>s.name>>s.preferences[0]>>s.preferences[1]>>s.preferences[2]){
        s.examScore = rand()%51+50;
        insertStudentSorted(students,count,s);
        count++;
    }
    infile.close();
    cout<<"Loaded "<<count<<" students"<<endl;
    return count;
}

int loadUniversities(University universities[]) {
    ifstream infile("universities.txt");
    if (!infile) {
        cout<<"Error opening universities.txt"<<endl;
        return 0;
    }

    int count = 0;
    while (infile>>universities[count].id>>universities[count].name) {
        infile.ignore(); 
        getline(infile,universities[count].location,' ');
        infile >>universities[count].availableSeats;
        count++;
    }
    infile.close();
    cout<<"Loaded "<<count<<" rows of universities"<<endl;
    return count;
}

int loadApplications(int applications[NUM_STUDENTS][NUM_UNIS]) {
    ifstream infile("applications.txt");
    if (!infile){
        cout<<"Error opening applications.txt"<<endl;
        return 0;
    }
    int i= 0;
    while (i<NUM_STUDENTS) {
        for (int j= 0;j<NUM_UNIS;j++) {
            infile >> applications[i][j];
        }
        i++;
    }
    infile.close();
    cout<<"Loaded "<<i<<" rows of application data"<<endl;
    cout<<endl;
    return i;
}

void displayMatrix(Student students[], University universities[], int applications[][NUM_UNIS]) {
 cout<<"Application Matrix:"<<endl;
 cout<<"Student Name\t";
 for (int j= 0;j<NUM_UNIS;j++){
     cout<<universities[j].name<<"\t";
 }
 cout << endl;

 for (int i= 0;i<NUM_STUDENTS;i++) {
     cout << students[i].name << "\t";
     cout<<"\t";
     cout<<"\t";
     for (int j= 0;j<NUM_UNIS;j++) {
         cout << applications[i][j] << "\t";
         cout<<"\t";
         cout<<"\t";
     }
     cout <<endl;
}
cout<<endl;
}

void countApplications(int applications[][NUM_UNIS], Student students[]) {
    int u_count = 0;
    int s_count = 0;
    cout<<"Applications per university:"<<endl;
        for (int j= 0;j<NUM_UNIS; j++) {
            u_count= 0;
            for (int i= 0;i <NUM_STUDENTS; i++) {
                u_count += applications[i][j];
            }
            cout<<"University "<<j<<": "<<u_count<<" applications"<<endl;
        }
        cout<<"\nApplications per student:"<<endl;
        for (int i= 0;i< NUM_STUDENTS;i++) {
            s_count=0;
            for (int j =0;j< NUM_UNIS;j++) {
                s_count+=applications[i][j];  
            }
            cout<<"Student"<<students[i].name<<": "<<s_count<<" applications"<<endl;
        }
        cout<<endl;
}

void checkFirstPreference(Student students[], int applications[][NUM_UNIS]) {
    int firstpref;
    cout<<"Students who applied to their 1st preference:"<<endl;
    for (int i =0;i<NUM_STUDENTS;i++) {
         firstpref =students[i].preferences[0];
        if (applications[i][firstpref] == 1) { 
            cout<<students[i].name<<endl;
        }
    }
    cout<<endl;
}

void displayStudentInfo(Student students[], int applications[][NUM_UNIS], University universities[]) {
    int num;
    cout<<"Enter the student index (0-9): ";
    cin>>num;
    if (num<0||num>=NUM_STUDENTS) {
        cout<<"Invalid student index!"<<endl;
        return;
    }
    Student* s_pointer=&students[num];
    cout<<"Student ID: "<<s_pointer->id<<endl;
    cout<<"Name: " <<s_pointer->name<<endl;
    cout<<"Exam Score: "<<s_pointer->examScore<<endl;
    
    cout<<"Preferences: ";
    for (int i=0;i<3;i++) {
        cout<<s_pointer->preferences[i]<<" ";
    }
    cout<<endl;

    cout << "Universities applied to: ";
    for (int i= 0; i<NUM_UNIS;i++) {
        if (applications[num][i] == 1) {
            cout<<i<<" ";
        }
    }
    cout<<endl;
    cout<<" "<<endl;
}

void queryByUniversity(Student students[], University universities[], int applications[][NUM_UNIS]) {
    int index;
    cout<<"Enter the University index (0-4): ";
    cin>>index;
    if (index<0 || index>=NUM_UNIS){
        cout << "Invalid index" << endl;
        return;
    }
    bool found = false;
    cout<<"University: "<<universities[index].name<<endl;
    cout<<"Location: "<<universities[index].location<<endl;
    cout << "Students who applied:" << endl;
    for (int i = 0; i< NUM_STUDENTS;i++) {
        if (applications[i][index] == 1){ 
            cout<<students[i].name<<" (Score: "<<students[i].examScore<<")"<<endl;
            found = true;
        }
    }
    if(!found){
        cout<<"No students applied to this university."<<endl;
    }
    cout<<endl;
}

void assignScholarships(Student students[], University universities[], int applications[][NUM_UNIS]) {
    srand(time(0));
    for (int j = 0; j <NUM_UNIS;j++){
        int highest_s = -1;
        int top_student = -1;
        for (int i = 0; i<NUM_STUDENTS;i++) {
            if (applications[i][j] == 1 && students[i].examScore>= 85) {
                if (students[i].examScore >highest_s) {
                    highest_s = students[i].examScore;
                    top_student = i;
                }
            }
        }
        if (top_student!= -1) {
            int scholarshipAmount = rand() % 2001 + 3000;
            students[top_student].gotScholarship = true;
            cout<<"University: "<<universities[j].name<<endl;
            cout<<"Student: "<<students[top_student].name<<endl;
            cout<<"Scholarship Amount: $"<<scholarshipAmount<<endl;
            cout<<"Score: "<<students[top_student].examScore<<endl;
            cout<<endl;
        } else {
            cout<<"University: "<<universities[j].name<<endl;
            cout<<"No eligible applicants"<<endl;
            cout<<endl;
        }
    }
}

void saveToBinary(Student students[]) {
    ofstream outfile("final_students.bin", ios::binary);

    if (!outfile){
        cout<<"Error opening file for writing!"<<endl;
        return;
    }
    for (int i = 0;i< NUM_STUDENTS;i++){
        outfile.write(reinterpret_cast<char*>(&students[i].id), sizeof(int));
        int length = students[i].name.length();
        outfile.write(reinterpret_cast<char*>(&length), sizeof(int));
        outfile.write(students[i].name.c_str(), length);
        outfile.write(reinterpret_cast<char*>(&students[i].preferences), sizeof(students[i].preferences));
        outfile.write(reinterpret_cast<char*>(&students[i].examScore), sizeof(int));
        outfile.write(reinterpret_cast<char*>(&students[i].gotScholarship), sizeof(bool));
    }
    outfile.close();
    cout<<"Saving Student records to binary file."<<endl;
    cout<<endl;
}

void loadFromBinary() {
    ifstream infile("final_students.bin", ios::binary);
    if (!infile) {
        cout<<"Error opening file for reading!"<<endl;
        return;
    }
    Student student;
    cout<<"--- Loaded from Binary ---"<<endl;
    while (infile.read(reinterpret_cast<char*>(&student.id), sizeof(int))) {
        int nameLength;
        infile.read(reinterpret_cast<char*>(&nameLength), sizeof(int));
        char* nameBuffer = new char[nameLength + 1];
        infile.read(nameBuffer, nameLength);
        nameBuffer[nameLength] = '\0'; 
        student.name = string(nameBuffer);
        delete[] nameBuffer;
        infile.read(reinterpret_cast<char*>(&student.preferences), sizeof(student.preferences));
        infile.read(reinterpret_cast<char*>(&student.examScore), sizeof(int));
        infile.read(reinterpret_cast<char*>(&student.gotScholarship), sizeof(bool));
        cout<<student.name<<" | Score: "<<student.examScore<< " | Scholarship: "<<(student.gotScholarship ? "Yes" : "No")<<endl;
    }
    infile.close();
    cout<<endl;
}

int efficientSearchByName(Student students[], int size, string targetName) {
    int low = 0;
    int high = size-1;
    while (low <= high) {
        int mid =low +(high-low)/2;
        if (students[mid].name == targetName) {
            return mid;
        }
        if (students[mid].name>targetName) {
            high=mid-1;
        }
        else {
            low=mid+1;
        }
    }
    return -1;
}

void searchStudentByName(Student students[]) {
    string name;
    cout<<"Enter the student's name to search: ";
    cin.ignore();
    getline(cin, name);
    int index = efficientSearchByName(students, NUM_STUDENTS, name);
    if (index != -1){
        cout<<"Student found:"<<endl;
        cout<<"Name: "<<students[index].name<<endl;
        cout<<"ID: "<<students[index].id<<endl;
        cout<<"Score: "<<students[index].examScore<<endl;
        cout<<"Scholarship Status: "<<(students[index].gotScholarship ? "Yes" : "No")<<endl;
    } else{
        cout<<"Student with the name '"<<name<<"' not found."<<endl;
    }
}

int main() {
    Student students[NUM_STUDENTS];
    University universities[NUM_UNIS];
    int applications[NUM_STUDENTS][NUM_UNIS];

    loadStudents(students);
    loadUniversities(universities);
    loadApplications(applications);
    displayMatrix(students, universities, applications);
    countApplications(applications, students);
    checkFirstPreference(students, applications);
    displayStudentInfo(students, applications , universities);
    queryByUniversity(students, universities, applications);
    assignScholarships(students, universities, applications);
    saveToBinary(students);
    loadFromBinary();
    searchStudentByName(students);
    return 0;
}
