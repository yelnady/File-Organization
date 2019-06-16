#pragma once
//#include <all.h>
#include "Author.h"
#include <vector>
using namespace std;
class Books {
	struct book {
		char Book_ID[13];
		char Author_ID[30];
		char Book_Title[50];
		float Book_Price;
	};
	struct PkBook {
		char Book_ID[13];
		int offset = 0;
		bool operator<(const PkBook &obj) {
			return strcmp(Book_ID, obj.Book_ID) < 0;
		}
	};
	struct SkBook {
		char Author_ID[30];
		int ListFilePointer;
		bool operator<(const SkBook &obj) {
			return (strcmp(Author_ID, obj.Author_ID) < 0);
		}
	};
	struct ListFileRec
	{
		int nextPointer;
		char Book_ID[13];
	};
public:
	vector<PkBook> PKIDX;
	vector<SkBook> SKIDX;
	vector<ListFileRec> LINKED;
	void AddBook(Authors& obj);
	void SortBooks();
	int  GetBookIDOffset(char* Book_ID, int &idx);
	void SaveIdx();
	bool PrintBook(char* Book_ID);
	void PrintAll();
	int  AuthorgetAddress(char*ID);
	void AddInSk(char *ID, char*Book_ID);
	void PrintAuthorBooks(char *Author);
	int  AuthorgetAddressseq(char* ID);
	void DeleteBookID(char *Book_ID);
	void DeleteAuthorID(char *Author_ID);

};