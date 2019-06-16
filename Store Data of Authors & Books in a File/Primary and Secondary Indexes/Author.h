#pragma once
//#include <all.h>
#include <strstream>
#include <vector>
using namespace std;
class Authors {
	struct Author {

		char Author_ID[30];
		char Author_Name[50];
		char Author_Address[50];
		char Author_Mobile[11];

	};
	struct PkAuthor {
		char Author_ID[30];
		int offset = 0;
		bool operator<(const PkAuthor &obj) {
			return strcmp(Author_ID, obj.Author_ID) < 0;
		}
	};
	struct SkAuthor {
		int ListFilePointer;
		char Author_Name[50];
		bool operator<(const SkAuthor &obj) {
			return strcmp(Author_Name, obj.Author_Name) < 0;
		}
	};
	struct ListFileRec
	{
		int prev;
		char Author_ID[30];
	};
public:
	vector<PkAuthor> PKIDX;
	vector<SkAuthor> SKIDX;
	vector<ListFileRec> LINKED;

public:

	void AddAuthor();
	void SortAuthors();
	int  GetAuthorIDOffset(char* ID, int &idx);
	void SaveIdx();
	void format(string &a, int len);
	void AddInSk(char *Author_ID, char*Author_Name);
	int  AuthorNamegetAddress(char* Author_Name); // return idx in Sk vector
	bool PrintAuthorID(char *Author_ID);
	void DeleteAuthorID(char *Author_ID);
	bool PrintAuthorName(char *Author_Name);
	void DeleteAuthorName(char *Author_Name);







};
