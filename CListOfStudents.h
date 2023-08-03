#pragma once

#include <iostream>
#include <fstream>
#include <string>

#define MARK_COUNT 6
#define NAME_LENGTH 31

class CStudent {
public:
    char m_sSurname[NAME_LENGTH];
    char m_sName[NAME_LENGTH];
    int m_iDay;
    int m_iMonth;
    int m_iYear;
    int m_aMarks[MARK_COUNT];
};

class CNode {
public:
    CStudent m_CStudent;
    CNode* m_pCNext;
};

class CListOfStudents {
    CNode* m_pCHead;
    int m_iCountOfStudents;
    int m_aSavedSort[3];
public:
    CListOfStudents();
    ~CListOfStudents();
    int readStudentsFromFile(std::string sFileLocation);
    int getAndAddStudent();
    void printListOfStudents();
    int selectStudent(int iNumberOfStudent, CNode** pCPrev, CNode** pCThis);
    int deleteStudent(int iNumberOfStudent);
    int editStudent(int iNumberOfStudent, int iNumberOfSubject, int iMark);
    void deleteStudents(int iElementToDeleteBy, int iMoreOrLess, int iThanWhat, int iSubject);
    void sortStudents(int iElementToSort, int iAscendingOrDescending, int iSubject);
    int saveStudentsToFile(std::string sFileLocation);

    void openTheFile(std::string& sFilePath);
    void editTheStudentsMark();
    void sortStudentsInMenu();
    void deleteStudentsByCriteria();
private:
    bool defineSort(int iElementToSort, int iAscendingOrDescending, CStudent* pCFirst, CStudent* pCSecond, int iSubject);
    bool defineDelete(int iElementToDeleteBy, int iMoreOrLess, int iThanWhat, CStudent CStudentToDelete, int iSubject);
    int addStudent(CStudent CStudentToAdd);

    bool isTheRightName(char sName[]);
    bool isTheRightDate(int iDay, int iMonth, int iYear);
    bool isTheRightMarks(int aMarks[]);
    bool isTheRightFile(std::string sFilePath);
};

int mainMenu();
int firstMenu();