#define _CRT_SECURE_NO_WARNINGS
#include "CListOfStudents.h"
#include <fstream>
#include <cstdlib>
#include <conio.h>

//================================================================================
//Default constructor, sets everything to zero
CListOfStudents::CListOfStudents() {
    m_pCHead = nullptr;
    m_iCountOfStudents = 0;
    m_aSavedSort[0] = 0;
    m_aSavedSort[1] = 0;
    m_aSavedSort[2] = 0;
}

//================================================================================
//Removes all the students from RAM
CListOfStudents::~CListOfStudents() {
    CNode* pCTemp;
    while (m_pCHead) {
        pCTemp = m_pCHead;
        m_pCHead = m_pCHead->m_pCNext;
        delete pCTemp;
    }
}

//================================================================================
//Reads students from file, puts them to the list
//Takes the name or location of the file as parameter
int CListOfStudents::readStudentsFromFile(std::string sFileLocation) {
    const char* sNewFileLocation = sFileLocation.c_str();
    std::fstream File(sNewFileLocation, std::ios::app | std::ios::binary);
    if (File.fail()) {
        return 0;
    }
    File.close();

    std::ifstream streamFile(sFileLocation, std::ios::binary);

    if (streamFile) {
        CStudent CTempStudent;
        if (!streamFile.read((char*)&(CTempStudent), sizeof(CStudent))) {
            return 1;
        }
        m_pCHead = new(std::nothrow)  CNode;
        CNode* pCTemp;
        //Checking if the memory was allocated
        if (!(m_pCHead)) {
            printf("Couldn't allocate the memory!");
            return 0;
        }

        (m_pCHead)->m_pCNext = nullptr;

        m_pCHead->m_CStudent = CTempStudent;
        //fread(&((*ppHead)->m_studentStudent), sizeof(struct Student), 1, fpOriginal);
        CNode* pCRun = (m_pCHead);
        m_iCountOfStudents++;
        while (streamFile.read((char*)&(CTempStudent), sizeof(CStudent))) {
            pCRun->m_pCNext = new CNode;
            m_iCountOfStudents++;

            //Checking if the memory was allocated
            if (!pCRun->m_pCNext) {
                while ((m_pCHead)) {
                    pCTemp = (m_pCHead);
                    (m_pCHead) = (m_pCHead)->m_pCNext;
                    delete pCTemp;
                }
                (m_pCHead) = NULL;
                std::cout << "Couldn't allocate the memory!";
                return 0;
            }

            pCRun = pCRun->m_pCNext;
            pCRun->m_CStudent = CTempStudent;
            pCRun->m_pCNext = nullptr;
        }
    }
    streamFile.close();
    return 1;
}

//================================================================================
//Gets all the information about student from default input stream
//Adds the student to the end, if the list wasn't sorted before
//Adds the student to the position, so the list remains sorted, if it was sorted before
int CListOfStudents::getAndAddStudent() {

    bool checkerIfWrong = true;

    CStudent studentToAdd;

    do {
        std::cout << "\nEnter student's name: ";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        //std::cin >> studentToAdd.m_sName;
        //std::cin.getline(studentToAdd.m_sName, NAME_LENGTH);
        scanf("%30s", studentToAdd.m_sName);
        if (!isTheRightName(studentToAdd.m_sName)) {
            std::cout << "The name must begin with the upper letter, and all the rest letters must be lower!\n";
        }
    } while (!isTheRightName(studentToAdd.m_sName));


    do {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Enter student's surname: ";
        //std::cin >> studentToAdd.m_sSurname;
        //std::cin.getline(studentToAdd.m_sSurname, NAME_LENGTH);
        scanf("%30s", studentToAdd.m_sSurname);
        if (!isTheRightName(studentToAdd.m_sSurname)) {
            std::cout << "The surname must begin with the upper letter, and all the rest letters must be lower!\n";
        }
    } while (!isTheRightName(studentToAdd.m_sSurname));

    do {
        printf("Enter student's date of birth (DD.MM.YYYY): ");

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> studentToAdd.m_iDay;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cout << "Try again\n";
            continue;
        }
        std::cin.ignore(32767, '.');
        //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> studentToAdd.m_iMonth;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cout << "Try again\n";
            continue;
        }
        std::cin.ignore(32767, '.');
        //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> studentToAdd.m_iYear;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cout << "Try again\n";
            continue;
        }

        if (!isTheRightDate(studentToAdd.m_iDay, studentToAdd.m_iMonth, studentToAdd.m_iYear)) {
            std::cout << "You are lying to me! Enter the correct date!\n";
        }

    } while (!isTheRightDate(studentToAdd.m_iDay, studentToAdd.m_iMonth, studentToAdd.m_iYear));

    bool bCheck = true;
    do {
        bCheck = true;
        printf("Enter student's marks (only %i): ", MARK_COUNT);
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        for (int j = 0; j < MARK_COUNT; j++) {
            std::cin >> studentToAdd.m_aMarks[j];
            if (std::cin.fail()) {
                std::cin.clear();
                //std::cout << "Try again\n";
                bCheck = false;
                break;
            }
        }
        if (!isTheRightMarks(studentToAdd.m_aMarks) || !bCheck) {
            std::cout << "Something wrong, I can feel it... Try again! The marks should be between 0 and 100\n";
        }
    } while (!isTheRightMarks(studentToAdd.m_aMarks) || !bCheck);

    if (!addStudent(studentToAdd)) {
        return 0;
    }
    m_iCountOfStudents++;
    return 1;
}

//================================================================================
//Prints the list of students in standart output stream
void CListOfStudents::printListOfStudents() {
    if (!m_pCHead) {
        printf("The list is empty!");
        return;
    }
    CNode* pCRun = m_pCHead;
    printf("\nList of students:\n---------------------------------------------------------------\
-------------\n     Surname\t     Name\t\tDate of birth\tMarks\n-------------------------------------------\
---------------------------------\n");
    int iNumberOfStudent = 1;
    while (pCRun != NULL) {
        printf(" %2i. %-15s %-15s\t%02i.%02i.%04i\t", iNumberOfStudent, pCRun->m_CStudent.m_sSurname, pCRun->m_CStudent.m_sName, \
            pCRun->m_CStudent.m_iDay, pCRun->m_CStudent.m_iMonth, pCRun->m_CStudent.m_iYear);
        for (int j = 0; j < MARK_COUNT; j++) {
            printf("%2i ", pCRun->m_CStudent.m_aMarks[j]);
        }
        printf("\n");
        pCRun = pCRun->m_pCNext;
        iNumberOfStudent++;
    }
    printf("----------------------------------------------------------------------------\n");
}

//================================================================================
//Gets the number of student, as it it sorted at this moment,
//Changes pPrev pointer so that it points on the student, that is before the chosen one
//Changes pThis pointer, so that it points on the student, that is selected with number
//Returns 1 if it selected the student, and returns 0 if it couldn select the student
int CListOfStudents::selectStudent(int iNumberOfStudent, CNode** pCPrev, CNode** pCThis) {
    int iCounter = 1;
    *pCPrev = nullptr;
    *pCThis = m_pCHead;

    while (*pCThis) {
        if (iNumberOfStudent == iCounter) {
            return 1;
        }
        iCounter++;
        *pCPrev = *pCThis;
        *pCThis = (*pCThis)->m_pCNext;
    }
    return 0;
}

//================================================================================
//Deletes the student with the number, that was got by parameter
//Returns 1, if it could delete the student, returns 0, if it couldn't
int CListOfStudents::deleteStudent(int iNumberOfStudent) {
    CNode* pCThis, * pCPrev;
    if (selectStudent(iNumberOfStudent, &pCPrev, &pCThis)) {
        if (pCPrev) {
            pCPrev->m_pCNext = pCThis->m_pCNext;
        }
        else {
            m_pCHead = pCThis->m_pCNext;
        }
        delete pCThis;
        m_iCountOfStudents--;
        return 1;
    }
    return 0;
}

//================================================================================
//Edits the mark of student with number "iNumberOfStudent" of the subject with number
//"iNumberOfSubject" to the mark "iMark"
//Returns 1 if it could edit this student, and 0, if it couldn't
int CListOfStudents::editStudent(int iNumberOfStudent, int iNumberOfSubject, int iMark) {
    CNode* pCThis, * pCPrev;
    if (selectStudent(iNumberOfStudent, &pCPrev, &pCThis)) {
        pCThis->m_CStudent.m_aMarks[iNumberOfSubject - 1] = iMark;
        return 1;
    }
    return 0;
}

//================================================================================
//Delete students by criteria "iElementToDeleteBy", if this equals to
//1 - delete by age
//2 - delete by rating
//3 - delete by mark of specific subject
//if "iMoreOrLess" equals to 0 - then it deletes students that have less, if 1 - have more
//"iThanWhat" is a parameter to compare with
//"iSubject" is used if it is chosen to delete by specific subject
void CListOfStudents::deleteStudents(int iElementToDeleteBy, int iMoreOrLess, int iThanWhat, int iSubject) {
    int iCheck = 1;
    CNode* pCRun = m_pCHead, * pCSave;
    while (iCheck) {
        iCheck = 0;
        pCRun = m_pCHead;
        if (!pCRun) {
            break;
        }
        if (defineDelete(iElementToDeleteBy, iMoreOrLess, iThanWhat, pCRun->m_CStudent, iSubject)) {
            pCSave = m_pCHead;
            m_pCHead = pCRun->m_pCNext;
            if (pCSave->m_pCNext == NULL) {
                m_pCHead = NULL;
            }
            delete pCSave;
            --m_iCountOfStudents;
            iCheck = 1;
            continue;
        }
        while (pCRun->m_pCNext != NULL) {
            if (defineDelete(iElementToDeleteBy, iMoreOrLess, iThanWhat, pCRun->m_pCNext->m_CStudent, iSubject)) {
                pCSave = pCRun->m_pCNext->m_pCNext;
                delete pCRun->m_pCNext;
                pCRun->m_pCNext = pCSave;
                iCheck = 1;
                --m_iCountOfStudents;
                break;
            }
            pCRun = pCRun->m_pCNext;
        }
    }
}

//================================================================================
//Adds a student to the end of the list, if it wasn't sorted before
//and to the place, that the list remains sorted, if it was sorted before
//"studentToAdd" - is a CStudent type parameter with all filled fields, that the student will have
int CListOfStudents::addStudent(CStudent studentToAdd) {
    CNode* pCNode = new(std::nothrow) CNode;
    if (!pCNode) {    //If the memory wasn't allocated
        CNode* pCTemp;
        while (m_pCHead) {
            pCTemp = (m_pCHead);
            (m_pCHead) = (m_pCHead)->m_pCNext;
            delete pCTemp;;
        }
        (m_pCHead) = NULL;
        printf("Couldn't allocate the memory!");
        return 0;
    }
    pCNode->m_CStudent = studentToAdd;
    pCNode->m_pCNext = NULL;

    if (!m_pCHead) {   //If the list is empty
        m_pCHead = pCNode;
        return 1;
    }

    CNode* pCRun = m_pCHead;
    while (pCRun->m_pCNext) {
        pCRun = pCRun->m_pCNext;
    }
    pCRun->m_pCNext = pCNode;
    sortStudents(m_aSavedSort[0], m_aSavedSort[1], m_aSavedSort[2]);
    return 1;
}

//================================================================================
//Checks if the name or the surname is in correct format
bool CListOfStudents::isTheRightName(char sName[]) {
    bool bCheckerIfWrong = false;
    if (!isupper(sName[0])) {
        bCheckerIfWrong = true;
    }
    int iter = 1; //iterator
    while (sName[iter] != '\0') {
        if (!islower(sName[iter]) && !((isupper(sName[iter + 1]) && sName[iter] == '-') || (isupper(sName[iter]) && sName[iter - 1] == '-'))) {
            bCheckerIfWrong = true;
        }
        iter++;
    }
    return !bCheckerIfWrong;
}

//================================================================================
//Checks if the date is right
bool CListOfStudents::isTheRightDate(int iDay, int iMonth, int iYear) {
    const int iYearToBegin = 1940;
    const int iYearToEnd = 2010;

    bool bCheckerTheRightDate = true;
    bool bCheckerTheLeapYear = false;

    if (iYear > iYearToEnd || iYear < iYearToBegin) {
        return false;
    }

    if (iYear % 4 == 0 && (iYear % 100 != 0 || iYear % 400 == 0)) {
        bCheckerTheLeapYear = true;
    }

    if (iMonth < 1 || iMonth > 12) {
        return false;
    }

    if (iMonth == 1 || iMonth == 3 || iMonth == 5 || iMonth == 7 || iMonth == 8 || iMonth == 10 || iMonth == 12) {
        if (iDay < 1 || iDay > 31) {
            return false;
        }
    }
    if (iMonth == 4 || iMonth == 6 || iMonth == 9 || iMonth == 11) {
        if (iDay < 1 || iDay > 30) {
            return false;
        }
    }
    if (bCheckerTheLeapYear) {
        if (iMonth == 2 && (iDay > 29 || iDay < 1)) {
            return false;
        }
    }
    else {
        if (iMonth == 2 && (iDay > 28 || iDay < 1)) {
            return false;
        }
    }
    return bCheckerTheRightDate;
}

//================================================================================
//Checks if array of marks are correct
bool CListOfStudents::isTheRightMarks(int aMarks[]) {
    for (int i = 0; i < MARK_COUNT; i++) {
        if (aMarks[i] > 100 || aMarks[i] < 0) {
            return false;
        }
    }

    return true;
}

//================================================================================
//Checks if the name of the file is right
bool CListOfStudents::isTheRightFile(std::string sFilePath) {
    if (sFilePath.length() < 5) {
        return false;
    }
    if (sFilePath.at(sFilePath.length() - 4) != '.') {
        return false;
    }
    if (sFilePath.at(sFilePath.length() - 3) != 'b') {
        return false;
    }
    if (sFilePath.at(sFilePath.length() - 2) != 'i') {
        return false;
    }
    if (sFilePath.at(sFilePath.length() - 1) != 'n') {
        return false;
    }
    return true;
}

//================================================================================
//Sorts students in the list
//"iElementToSort" - the parameter, that students have to be sorted by, where
//1 - by surname
//2 - by age
//3 - by rating
//4 - by mark of specific subject
//"iAscendingOrDescending" is the parameter, that students have to sort how, 1 - Asc, 2 - Desc
//"iSubject" is used when it is chosen 4th parameter of sorting, and it represents the number of subject,
//by of which marks the list will be sorted
void CListOfStudents::sortStudents(int iElementToSort, int iAscendingOrDescending, int iSubject = 0) {
    m_aSavedSort[0] = iElementToSort;
    m_aSavedSort[1] = iAscendingOrDescending;
    m_aSavedSort[2] = iSubject;
    if (m_pCHead) {
        CNode* pCRun = m_pCHead, * pCBeforeRun = NULL;
        int iCountOfIterations = this->m_iCountOfStudents;
        for (int i = 0; i < iCountOfIterations; i++) {
            pCBeforeRun = NULL;
            pCRun = m_pCHead;
            while (pCRun->m_pCNext) {
                if (defineSort(iElementToSort, iAscendingOrDescending, &(pCRun->m_CStudent), &(pCRun->m_pCNext->m_CStudent), iSubject)) {
                    /*switch pRun and pRun->pNext*/
                    CNode* pCRunNext = pCRun->m_pCNext;
                    if (pCBeforeRun) {
                        pCBeforeRun->m_pCNext = pCRunNext;
                    }
                    else {
                        m_pCHead = pCRun->m_pCNext;
                    }
                    pCRun->m_pCNext = pCRun->m_pCNext->m_pCNext;
                    pCRunNext->m_pCNext = pCRun;
                    pCRun = pCRunNext; //Changing pRun because we switched it before
                }
                pCBeforeRun = pCRun;

                pCRun = pCRun->m_pCNext;
            }
        }
    }
}

//================================================================================
//Saves all the students to the file with name or location "sFileLocation"
int CListOfStudents::saveStudentsToFile(std::string sFileLocation) {
    const char* sNewFileLocation = sFileLocation.c_str();
    std::ofstream File(sNewFileLocation, std::ios::out | std::ios::binary);

    if (File) {
        if (m_pCHead) {
            CNode* pCRun = m_pCHead;
            while (pCRun) {
                File.write((char*)&(pCRun->m_CStudent), sizeof(CStudent));
                pCRun = pCRun->m_pCNext;
            }
            return 1;
        }
        return 0;
    }
    return 0;
}

//================================================================================
//This method is used to define, if the "pFirst" student has to be placed before or after the "pSecond" student
//"iElementToSort" - the parameter, that students have to be sorted by, where
//1 - by surname
//2 - by age
//3 - by rating
//4 - by mark of specific subject
//"iAscendingOrDescending" is the parameter, that students have to sort how, 1 - Asc, 2 - Desc
//"iSubject" is used when it is chosen 4th parameter of sorting, and it represents the number of subject,
//by of which marks the list will be sorted
bool CListOfStudents::defineSort(int iElementToSort, int iAscendingOrDescending, CStudent* pCFirst, CStudent* pCSecond, int iSubject = 0) {
    //Ascending - 1
    //Descending - 0
    int iSum1 = 0, iSum2 = 0;
    switch (iElementToSort) {
    case 1: //By Surname
        if (strcmp(pCFirst->m_sSurname, pCSecond->m_sSurname) > 0) {
            if (iAscendingOrDescending) {
                return false;
            }
            else return true;
        }
        else {
            if (iAscendingOrDescending) {
                return true;
            }
            else return false;
        }
    case 2: //By Age
        int iTempAge1, iTempAge2;
        iTempAge1 = pCFirst->m_iYear * 10'000 + pCFirst->m_iMonth * 100 + pCFirst->m_iDay;
        iTempAge2 = pCSecond->m_iYear * 10'000 + pCSecond->m_iMonth * 100 + pCSecond->m_iDay;
        if (iTempAge1 < iTempAge2) {
            if (iAscendingOrDescending) {
                return false;
            }
            else return true;
        }
        else {
            if (iAscendingOrDescending) {
                return true;
            }
            else return false;
        }
    case 3: //By rating

        for (int i = 0; i < MARK_COUNT; i++) {
            iSum1 += pCFirst->m_aMarks[i];
            iSum2 += pCSecond->m_aMarks[i];
        }
        if (iSum2 > iSum1) {
            if (iAscendingOrDescending) {
                return false;
            }
            else return true;
        }
        else {
            if (iAscendingOrDescending) {
                return true;
            }
            else return false;
        }
    case 4: //By subject
        if (pCFirst->m_aMarks[iSubject - 1] < pCSecond->m_aMarks[iSubject - 1]) {
            if (iAscendingOrDescending) {
                return false;
            }
            else return true;
        }
        else {
            if (iAscendingOrDescending) {
                return true;
            }
            else return false;
        }
    default:
        return false;
    }
}

//================================================================================
//This method is used to define, if the "pStudent" has to be deleted or not
//returns true, if it has to, returns false, if it doesn't
bool CListOfStudents::defineDelete(int iElementToDeleteBy, int iMoreOrLess, int iThanWhat, CStudent pCStudent, int iSubject = 0) {
    //More - 1
    //Less - 0
    int iSum = 0;
    switch (iElementToDeleteBy) {
    case 1: //By Age
        int iTempAge;
        iTempAge = pCStudent.m_iYear * 10'000 + pCStudent.m_iMonth * 100 + pCStudent.m_iDay;
        if (iTempAge < iThanWhat) {
            if (iMoreOrLess) {
                return false;
            }
            else return true;
        }
        else {
            if (iMoreOrLess) {
                return true;
            }
            else return false;
        }
    case 2: //By rating

        for (int i = 0; i < MARK_COUNT; i++) {
            iSum += pCStudent.m_aMarks[i];
        }
        if (iSum / MARK_COUNT < iThanWhat) {
            if (iMoreOrLess) {
                return false;
            }
            else return true;
        }
        else {
            if (iMoreOrLess) {
                return true;
            }
            else return false;
        }
    case 3: //By subject
        if (pCStudent.m_aMarks[iSubject - 1] < iThanWhat) {
            if (iMoreOrLess) {
                return false;
            }
            else return true;
        }
        else {
            if (iMoreOrLess) {
                return true;
            }
            else return false;
        }
    default:
        return false;
    }
}

//================================================================================
//This function gives user a menu on the screen, it returns the value, that the user has chosen
int mainMenu() {
    int iAnswer = -1;
    bool bCheck = false;
    static bool bFirst = true;
    while (iAnswer < 0 || iAnswer > 5 || bCheck) {
        std::cout << "\nThere are your possibilities:\nAdd new student - 1\nEdit a student's mark - 2\nSort students - 3\nDelete students by criteria - 4\nSave file - 5\nSave and close the program - 0\nYour answer is: ";
        /*std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');*/
        if (bFirst) {
            std::cin.putback('\n');
            std::cin.putback('1'); 
            //std::cin.clear();
            //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            bFirst = false;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> iAnswer;
        if (std::cin.fail()) {
            bCheck = true;
            continue;
        }
        bCheck = false;
    }
    return iAnswer;
}

//================================================================================
//This function gives user a menu, that is used at the start of the program
//it returns the value, that the user has chosen
int firstMenu() {
    int iAnswer = -1;
    bool bCheck = true;
    while (iAnswer < 0 || iAnswer > 1 || bCheck) {
        std::cout << "\nThere are your possibilities:\nOpen the file - 1\nClose the program - 0\nYour answer is: ";
        //std::cin.clear();
        /*std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.clear();*/
        std::cin >> iAnswer;

        /*if (std::cin.fail()) {
            bCheck = true;
            continue;
        }*/
        bCheck = false;
        //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    return iAnswer;
}

//================================================================================
//This method is used in a menu
//It provides user a possibility to enter the name of the file to open
//It gets "sFilePath" as a parameter, so that it may be used later
void CListOfStudents::openTheFile(std::string& sFilePath) {
    system("CLS");
    int iCheck = 0;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    do {
        iCheck = 0;
        std::cout << "\nPlease, enter the correct path to the file and/or its name: ";
        char sFilePathTemp[512];
        
        std::cin.getline(sFilePathTemp, 511);
        //std::cin >> sFilePath;
        sFilePath = sFilePathTemp;
        //std::cin.putback('1');
        std::cin.clear();
        //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (!isTheRightFile(sFilePath)) {
            std::cout << "The file must end with \".bin\" and has to have name\n";
            iCheck = 0;
            std::cin.clear();
            //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        if (this->readStudentsFromFile(sFilePath) == 1) {
            iCheck = 1;
        }
    } while (iCheck == 0);
    std::cout << "Your file was read or created successfully! Press any key to continue...";
    _getch();
    system("CLS");
    this->printListOfStudents();
}

//================================================================================
//This method is used in menu
//It provides user an opportunity to input all the information about editing the student
//And then it edits him by previosly gotten parameters
void CListOfStudents::editTheStudentsMark() {
    int iNumber, iSubject, iNewMark;
    bool bCheck = true;
    if (m_iCountOfStudents == 0) {
        return;
    }
    do {
        std::cout << "Enter the number of which student you would like to edit mark: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> iNumber;
        if (iNumber < 1 || iNumber > this->m_iCountOfStudents) {
            std::cout << "Why are you bully me?\n";
        }
    } while (iNumber < 1 || iNumber > this->m_iCountOfStudents);

    do {
        std::cout << "Enter the number of subject, the mark of which you want to edit: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> iSubject;
        if (iSubject < 1 || iSubject > MARK_COUNT) {
            std::cout << "Why are you bully me?\n";
        }
    } while (iSubject < 1 || iSubject > MARK_COUNT);

    do {
        std::cout << "Enter the new mark of a student: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> iNewMark;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cout << "Why are you bully me?\n";
            bCheck = true;
            continue;
        }
        if (iNewMark < 0 || iNewMark > 100) {
            std::cout << "Why are you bully me?\n";
        }
        bCheck = false;
    } while (iNewMark < 0 || iNewMark > 100 || bCheck);
    editStudent(iNumber, iSubject, iNewMark);
}

//================================================================================
//This method is used in menu
//It provide user an opportunity to enter the parameters, by which the list will be sorted
//And the it sorts them by those parameters
void CListOfStudents::sortStudentsInMenu() {
    if (m_iCountOfStudents == 0) {
        return;
    }
    bool bCheck = true;
    int iCriteria = 0, iAscendingOrDescending = -1, iSubject = -1;
    std::cout << "Choose the critera you would like to sort students by:\nBy surname - 1\nBy birth - 2\nBy rating - 3\nBy mark of subject - 4\n";
    while (iCriteria < 1 || iCriteria > 4 || bCheck) {
        std::cout << "Enter an option: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> iCriteria;
        if (std::cin.fail()) {
            bCheck = true;
            continue;
        }
        bCheck = false;
    }
    bCheck = true;
    std::cout << "Choose ascending or descending:\nAscending - 1\nDescending - 0\n";
    while (iAscendingOrDescending < 0 || iAscendingOrDescending > 1 || bCheck) {
        std::cout << "Enter an option: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> iAscendingOrDescending;
        if (std::cin.fail()) {
            bCheck = true;
            continue;
        }
        bCheck = false;
    }
    bCheck = true;
    if (iCriteria == 4) {
        std::cout << "Choose the subject, you would like to sort by:\n";
        while (iSubject < 1 || iSubject > MARK_COUNT || bCheck) {
            std::cout << "Enter an option: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin >> iSubject;
            if (std::cin.fail()) {
                bCheck = true;
                continue;
            }
            bCheck = false;
        }
    }
    else {
        iSubject = 0;
    }
    sortStudents(iCriteria, iAscendingOrDescending, iSubject);

}

//================================================================================
//This method is used in menu
//It provides user an opportunity to enter criterias, by which the students will be deleted
//And then it deletes them by those criterias
void CListOfStudents::deleteStudentsByCriteria() {
    if (m_iCountOfStudents == 0) {
        return;
    }
    int iCriteria = 0, iLessOrMore = -1, iSubject = -1;
    int iYear = -1, iMonth = -1, iDay = -1, iMark = -1;
    bool bCheck = true;
    std::cout << "Choose the criteria you would like to delete students by:\nBy date of birth - 1\nBy rating - 2\nBy mark of subject - 3\n";
    while (iCriteria < 1 || iCriteria > 3 || bCheck) {
        std::cout << "Enter an option: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> iCriteria;
        if (std::cin.fail()) {
            bCheck = true;
            continue;
        }
        bCheck = false;
    }
    bCheck = true;
    std::cout << "Choose do you want to delete students that are less ... or more ...:\nMore - 1\nLess - 0\n";
    while (iLessOrMore < 0 || iLessOrMore > 1 || bCheck) {
        std::cout << "Enter an option: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> iLessOrMore;
        if (std::cin.fail()) {
            bCheck = true;
            continue;
        }
        bCheck = false;
    }
    switch (iCriteria) {
    case 1:
        do {
            printf("Enter student's date of birth in format (DD.MM.YYYY): ");

            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin >> iDay;
            if (std::cin.fail()) {
                std::cin.clear();
                std::cout << "Try again\n";
                continue;
            }
            std::cin.ignore(32767, '.');
            //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin >> iMonth;
            if (std::cin.fail()) {
                std::cin.clear();
                std::cout << "Try again\n";
                continue;
            }
            std::cin.ignore(32767, '.');
            //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin >> iYear;
            if (std::cin.fail()) {
                std::cin.clear();
                std::cout << "Try again\n";
                continue;
            }

            if (!isTheRightDate(iDay, iMonth, iYear)) {
                std::cout << "You are lying to me! Enter the correct date!\n";
            }

        } while (!isTheRightDate(iDay, iMonth, iYear));
        break;
    case 2:

        while ((iMark < 0 || iMark > 100) || bCheck) {
            std::cout << "Enter the mark, by which you want to delete: ";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.clear();
            std::cin >> iMark;
            if (std::cin.fail()) {
                bCheck = true;
                continue;
            }
            bCheck = false;
        }
        break;
    case 3:
        bCheck = true;
        while (iSubject < 1 || iSubject > MARK_COUNT || bCheck) {
            std::cout << "Enter the subject, by which mark you want to delete: ";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.clear();
            std::cin >> iSubject;
            if (std::cin.fail()) {
                bCheck = true;
                continue;
            }
            bCheck = false;
        }
        while (iMark < 0 || iMark > 100 || bCheck) {
            std::cout << "Enter the mark, by which you want to delete: ";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.clear();
            std::cin >> iMark;
            if (std::cin.fail()) {
                bCheck = true;
                continue;
            }
            bCheck = false;
        }
        break;
    }
    int iFinal = 0;
    if (iMark != -1) {
        iFinal = iMark;
    }
    if (iMonth != -1) {
        iFinal = iYear * 10'000 + iMonth * 100 + iDay;
    }
    deleteStudents(iCriteria, iLessOrMore, iFinal, iSubject);
}
