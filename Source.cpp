#define _CRT_SECURE_NO_WARNINGS
#include <locale>
#include <string>
#include "CListOfStudents.h"
#include <Windows.h>

int main(void) {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	CListOfStudents CList;
	std::string sFilePath;

	int iAnswer = 1;

	iAnswer = firstMenu();
	switch (iAnswer) {
	case 1:
		CList.openTheFile(sFilePath);
		break;
	case 0:
		return 0;
	}
	while (iAnswer != 0) {
		iAnswer = mainMenu();
		switch (iAnswer) {
		case 1:
			CList.getAndAddStudent();
			break;
		case 2:
			CList.editTheStudentsMark();
			break;
		case 3:
			CList.sortStudentsInMenu();
			break;
		case 4:
			CList.deleteStudentsByCriteria();
			break;
		case 5:
			CList.saveStudentsToFile(sFilePath);
			break;
		case 0:
			CList.saveStudentsToFile(sFilePath);
			return 0;
		}
		system("CLS");
		CList.printListOfStudents();
	}
}
