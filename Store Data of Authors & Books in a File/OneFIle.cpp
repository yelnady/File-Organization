/*
Name : Yusuf Fawzy Elnady  			ID : 201600299
Name : Muhammed Ayman Abdelnabi 	ID : 20160196  
*/

#include "stdafx.h"
#include <all.h>
#include "stdafx.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <strstream>
#include <string>
#include <cstring>
using namespace std ;
struct where{
	string consdition;
	string value;
};

struct book {
	char Book_ID[13];
	char Author_ID[30];
	char Book_Title[50];
	char Book_Price[10];
};

struct author {
	char Author_ID[30];
	char Author_Name[50];
	char Author_Address[50];
	char Author_Mobile[11];
};

class Query
{
public:
	// source.cpp : Defines the entry point for the console application.
	//

	

	where analyze_where_condition(string cond);

	//bool is_deleted(char* buffer) {
	//	return (buffer[0] == '*');
	//}

	void execute(Authors &a , Books &b);

	
};


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
	void AddBook();
	void SortBooks();
	int  GetBookIDOffset(char* Book_ID,int &idx);
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
using namespace std;
void Authors::format(string &a, int len) {
	while (a.size() < len) {
		a += ' ';
	}
	return;
}
void Authors::AddAuthor() {
	fstream f("Author.txt", ios::out | ios::app);
	f.seekp(0, ios::end);
	int nowIdx = (int)f.tellp();
	Author temp;
	cout << "Author ID: ";cin.getline(temp.Author_ID, 29);
	cout << "Author Name: ";cin.getline(temp.Author_Name, 49);
	cout << "Author Address: ";cin.getline(temp.Author_Address, 49);
	cout << "Author Mobile: ";cin.getline(temp.Author_Mobile, 10);

	string authorid(temp.Author_ID), authorname(temp.Author_Name), authordadd(temp.Author_Address), mob(temp.Author_Mobile);
	string all = ' ' + authorid + '|' + authorname + '|' + authordadd + '|' + mob + '|';
	short length = (short)all.size();
	cout << "Your Entered Author is " << length << all << endl;
	f << length << all;

	PkAuthor pk;
	strcpy_s(pk.Author_ID, temp.Author_ID);
	pk.offset = nowIdx;
	PKIDX.push_back(pk);
	AddInSk(temp.Author_ID, temp.Author_Name);
	f.close();
	
}
void Authors::SaveIdx() {
	SortAuthors();
	fstream PKfile("Pkauthors.txt", ios::out);
	fstream SKfile("Skauthors.txt", ios::out);
	fstream LINK("linkedauthor.txt", ios::out);
	for (int i = 0; i < PKIDX.size();i++) {
		string pk1(PKIDX[i].Author_ID);
		format(pk1, 30);
		PKfile << pk1 << " " << PKIDX[i].offset << endl;
	}
	for (int i = 0; i < SKIDX.size();i++) {
		string sk1(SKIDX[i].Author_Name);
		format(sk1, 50);
		SKfile << sk1 << " " << SKIDX[i].ListFilePointer << endl;
	}
	for (int i = 0; i < LINKED.size();i++) {
		string link1(LINKED[i].Author_ID);
		format(link1, 13);
		LINK << link1 << " " << LINKED[i].prev << endl;
	}
	PKfile.close();
	SKfile.close();
	}

void Authors::AddInSk(char *Author_ID, char*Author_Name) {
	int idx = AuthorNamegetAddress(Author_Name);
	ListFileRec linktemp;

	if (idx == -1) {         /*we have a new author*/
		SkAuthor sktemp;
		strcpy_s(linktemp.Author_ID, Author_ID); linktemp.prev = -1;
		LINKED.emplace_back(linktemp);
		strcpy_s(sktemp.Author_Name, Author_Name);sktemp.ListFilePointer = (int)(LINKED.size() - 1);
		SKIDX.emplace_back(sktemp);
	}
	else {
		int rrn = SKIDX[idx].ListFilePointer;
		strcpy_s(linktemp.Author_ID, Author_ID);
		linktemp.prev = rrn;
		LINKED.emplace_back(linktemp);
		int ListFilePointer = (int)(LINKED.size() - 1); /*To update the value of ListFilePointer in SK index*/
		SKIDX[idx].ListFilePointer = ListFilePointer;
	}
}

int  Authors::AuthorNamegetAddress(char* Author_Name) { // return the index in the sk file
	int low = 0, high = (int)(SKIDX.size() - 1);
	while (low <= high) {
		int mid = (low + high) / 2;
		if (strcmp(SKIDX[mid].Author_Name, Author_Name) == 0) { return mid; } //error here cuz we increase the size of ID into albta3a
		if (strcmp(SKIDX[mid].Author_Name, Author_Name) < 0) low = mid + 1;
		else  high = mid - 1;
	}
	return -1;
}

void Authors::SortAuthors() {
	sort(PKIDX.begin(), PKIDX.end());
	sort(SKIDX.begin(), SKIDX.end());
}

int Authors::GetAuthorIDOffset(char* ID, int &idx) { //return the offset in the data file and the index in the pk file 
	strstream stream; string id;
	stream << ID;
	stream >> id;
	int s = id.size();
	id = id.substr(0, s - 1);
	int low = 0, high = (int)(PKIDX.size() - 1);
	while (low <= high) {
		int mid = (low + high) / 2;
		if (strcmp(PKIDX[mid].Author_ID, &id[0]) == 0) { idx = mid; return PKIDX[mid].offset; }
		if (strcmp(PKIDX[mid].Author_ID, ID)<0)low = mid + 1;
		else high = mid - 1;
	}
	return -1;
}
bool Authors::PrintAuthorID(char *Author_ID) { //if we deleted from author ID he won't find it in the search asln
	int n;
	int x = GetAuthorIDOffset(Author_ID, n);
	if (x == -1) { cout << "Author isn't found1\n"; return 0; }
	else {
		fstream f("Author.txt", ios::in);
		f.seekg(x, ios::beg); int n;char c;
		f >> n;
		f.get(c);if (c == '*') {
			cout << "Author isn't found2\n"; return 0;
		}
		Author temp;
		f.getline(temp.Author_ID, 29, '|');f.getline(temp.Author_Name, 49, '|');f.getline(temp.Author_Address, 49, '|');
		f.getline(temp.Author_Mobile, 10, '|');
		cout << "Author ID: " << temp.Author_ID << " /Author Name: " << temp.Author_Name << " /Author Address: " << temp.Author_Address
			<< " /Author Mobile: " << temp.Author_Mobile << endl;
	}
	return 1;
}

bool Authors::PrintAuthorName(char *Author_Name) {
	int idx = AuthorNamegetAddress(Author_Name);
	if (idx == -1) {
		cout << "This Author isn't found\n";return 0;
	}
	else {
		int x = SKIDX[idx].ListFilePointer;
		do {
			PrintAuthorID(LINKED[x].Author_ID);
			x = LINKED[x].prev;
			if (x == -1)break;
		} while (1);
	}
	return 1;
}

void Authors::DeleteAuthorID(char *Author_ID) {
	int idx;
	int x = GetAuthorIDOffset(Author_ID, idx);
	if (x == -1) { cout << "Author isn't found\n"; }
	else {
		/*Delete in Data file*/
		fstream f("Author.txt", ios::out | ios::in);
		f.seekp(x, ios::beg);
		short n; f >> n;
		Author temp;

    	f.getline(temp.Author_ID,29, '|');f.getline(temp.Author_Name, 49, '|');
		f.seekp(x + sizeof(n), ios::beg);
		string fill(n, '-');fill = "*" + fill;
		f.write(&fill[0], n);

											/*Delete in PKIDX vector*/
		PKIDX.erase(PKIDX.begin() + idx);
						/*no need to update secondary key as if he searched for this Pk will return not found*/
		idx = AuthorNamegetAddress(temp.Author_Name); //now we have the idx in SKIDX vector
		int fileptr;
		for (int i = 0; i < LINKED.size();i++) {
			if (strcmp(LINKED[i].Author_ID, Author_ID) == 0) { fileptr = i; break; }
		}
		int previous;
		if (fileptr != -1) previous = LINKED[fileptr].prev;
		else previous = -1;
		int next = -1;
		for (int i = 0; i < LINKED.size();i++) {
			if (fileptr == LINKED[i].prev) { next = i; break; }
		}
		LINKED[fileptr].Author_ID[0] = '#';
		LINKED[fileptr].prev = -1;
		if (previous == -1 && next == -1) { // the element is the last in the linked list no next and z==-1 means that we need to modify also SK to be -1 
			SKIDX[idx].ListFilePointer = -1;
		}
		else if (previous == -1 && next != -1) { // hena lw mafy4 7ad 2ablya w mafy4 
			LINKED[next].prev =previous;
		}
		else if (previous != -1 && next != -1) { //fy 7ad b3dya w 7ad 2ablya
			LINKED[next].prev = previous;
		}
		else if (previous != -1 && next == -1) {
			SKIDX[idx].ListFilePointer = previous;
		}
	}
	cout << "This Author has been deleted\n";
}
void Authors::DeleteAuthorName(char *Author_Name) {
	int  x = AuthorNamegetAddress(Author_Name);
	if (x == -1){ cout << "Author Name isn't found\n"; return;}
	x = SKIDX[x].ListFilePointer;
	vector <char*> books;
	int n = 0;
	do {
		if (x == -1)break;
		books.push_back(LINKED[x].Author_ID);
		if (LINKED[x].prev == -1) break;
		else x = LINKED[x].prev;

	} while (1);
	for (auto I : books) { cout << I << endl; DeleteAuthorID(I); }
	cout << "All Authors with name is deleted\n";
}
void format(string &a, int len) {
	while (a.size() < len) {
		a += ' ';
	}
	return;
}

void Books::AddBook() {
	fstream f("book.txt", ios::out | ios::app);
	f.seekp(0,ios::end);
	int nowIdx = (int)f.tellp(); /*we will write it into offset of pk array*/

	book temp;
	cout << "Book ID: ";cin.getline(temp.Book_ID, 12);
	cout << "Author ID: ";cin.getline(temp.Author_ID, 30);
	cout << "Book Title: ";cin.getline(temp.Book_Title, 50);
	cout << "Book Price: ";cin >> temp.Book_Price;

	string bookid(temp.Book_ID), authorid(temp.Author_ID), booktitle(temp.Book_Title), bookprice;
	strstream obj; obj << temp.Book_Price; obj >> bookprice;
	string all = ' ' + bookid + '|' + authorid + '|' + booktitle + '|' + bookprice + '|';
	short length = all.size();
	cout << "Your Entered Book is " << length << all << endl;
	f << length << all;

	PkBook pk;
	strcpy_s(pk.Book_ID, temp.Book_ID);
	pk.offset = nowIdx;
	PKIDX.push_back(pk);
	AddInSk(temp.Author_ID, temp.Book_ID);
	f.close();

}
void Books::AddInSk(char *Author_ID, char*Book_ID) {
	int idx = AuthorgetAddress(Author_ID);
	ListFileRec linktemp;

	if (idx == -1) {         /*we have a new author*/
		SkBook sktemp;
		strcpy_s(linktemp.Book_ID, Book_ID); linktemp.nextPointer = -1;
		LINKED.emplace_back(linktemp);
		strcpy_s(sktemp.Author_ID, Author_ID);sktemp.ListFilePointer = LINKED.size() - 1;
		SKIDX.emplace_back(sktemp);
	}
	else {
		int rrn = SKIDX[idx].ListFilePointer;
		strcpy_s(linktemp.Book_ID, Book_ID);
		linktemp.nextPointer = rrn;
		LINKED.emplace_back(linktemp);
		int ListFilePointer = LINKED.size() - 1; /*To update the value of ListFilePointer in SK index*/
		SKIDX[idx].ListFilePointer = ListFilePointer;
	}
}
int  Books::AuthorgetAddress(char* Author_ID) { // return the index in the sk file
	int low = 0, high = SKIDX.size() - 1;
	strstream stream; string id;
	stream << Author_ID;
	int s = id.size();
	id = id.substr(0, s - 1);
	while (low <= high) {
		int mid = (low + high) / 2;
		if (strcmp(SKIDX[mid].Author_ID, &id[0]) == 0) { return mid; } //error here cuz we increase the size of ID into albta3a
		if (strcmp(SKIDX[mid].Author_ID, &id[0]) < 0) low = mid + 1;
		else  high = mid - 1;
	}
	return -1;
}
int  Books::AuthorgetAddressseq(char* ID) {
	for (int i = 0; i < SKIDX.size();i++) {
		if (strcmp(SKIDX[i].Author_ID, ID) == 0) return i;
	}
	return -1 ;
}
int Books::GetBookIDOffset(char* ID,int &idx ) { //return the offset in the data file and the index in the pk file 
	strstream stream; string id;
	stream << ID;
	stream >> id;
	int s = id.size();
	id = id.substr(0, s-1);
	int low = 0, high = PKIDX.size() - 1;
	while (low <= high) {
		int mid = (low + high) / 2;	
		if (strcmp(PKIDX[mid].Book_ID, &id[0])==0)
		{ 
			idx = mid; 
			return PKIDX[mid].offset; 
		}
		if (strcmp(PKIDX[mid].Book_ID, ID)<0)low = mid + 1;
		else high = mid - 1;
	}
	return -1;
}
void Books::SortBooks() {
	sort(PKIDX.begin(), PKIDX.end());
	sort(SKIDX.begin(), SKIDX.end());
}
void Books::SaveIdx() {

	fstream PKfile("pkbooks.txt", ios::out);
	fstream SKfile("skbooks.txt", ios::out);
	fstream LINK("linked.txt", ios::out);
	for (int i = 0; i < PKIDX.size();i++) {
		string pk1(PKIDX[i].Book_ID);
		format(pk1, 13);
		PKfile << pk1 << " " << PKIDX[i].offset << endl; 
	}
	for (int i = 0; i < SKIDX.size();i++) {
		string sk1(SKIDX[i].Author_ID);
		format(sk1, 30);
		SKfile << sk1 << " " << SKIDX[i].ListFilePointer << endl;
	}
	for (int i = 0; i < LINKED.size();i++) {
		string link1(LINKED[i].Book_ID);
		format(link1, 13);
		LINK << link1 << " " << LINKED[i].nextPointer << endl;
	}
	PKfile.close();
	SKfile.close();
}


void Books::PrintAuthorBooks(char *Author) {
	int idx = AuthorgetAddress(Author);	
	if (idx == -1) {
		cout << "This Author isn't found\n";
	}
	else {
		int nxt = SKIDX[idx].ListFilePointer;
		do{
			PrintBook(LINKED[nxt].Book_ID);
			nxt = LINKED[nxt].nextPointer;
			if (nxt == -1)break;
		} while (1);
	}
}
bool Books::PrintBook(char *Book_ID) {
	int n;
	int x = GetBookIDOffset(Book_ID,n);
	if (x == -1) { cout << "Book isn't found1\n"; return 0; }
	else {
		fstream f("book.txt", ios::in);
		f.seekg(x, ios::beg); int n;char c;
		f >> n;
		f.get(c);if (c == '*') {
			cout << "Book isn't found2\n"; return 0;
		}

		book temp;
		f.getline(temp.Book_ID, 12, '|');f.getline(temp.Author_ID, 29, '|');f.getline(temp.Book_Title, 49, '|');f >> temp.Book_Price;
		cout << "Book ID: " << temp.Book_ID << " /Book Author: " << temp.Author_ID << " /Book Title: " << temp.Book_Title 
			 << " /Book Price: " << temp.Book_Price << endl;
	}
	return 1;
}
void Books::PrintAll() {
	
}
void Books::DeleteBookID(char *Book_ID) {
	int idx;
	int x = GetBookIDOffset(Book_ID,idx);
	if (x == -1) { cout << "Book isn't found\n"; }
	else {
				/*Delete in Data file*/
		fstream f("book.txt", ios::out|ios::in);
		f.seekp(x, ios::beg);
		short n; f >> n;
		book temp;
		f.getline(temp.Book_ID, 12, '|');f.getline(temp.Author_ID, 29, '|');
		f.seekp(x + sizeof(n), ios::beg);
		string fill(n, '-');fill = "*" + fill;
		f.write(&fill[0], n);
				/*Delete in PKIDX vector*/
		PKIDX.erase(PKIDX.begin()+idx);
				/*no need to update secondary key as if he searched for this Pk will return not found*/
		idx = AuthorgetAddress(temp.Author_ID); //now we have the idx in SKIDX vector
		int fileptr;
		for (int i = 0; i < LINKED.size();i++) {
			if (strcmp(LINKED[i].Book_ID, Book_ID) == 0) { fileptr = i; break; }
		}
		int previous;
		if (fileptr!=-1) previous = LINKED[fileptr].nextPointer;
		else previous = -1;
		int next = -1;
		for (int i = 0; i < LINKED.size();i++) {
			if (fileptr == LINKED[i].nextPointer) {next = i; break;} 
		}
		LINKED[fileptr].Book_ID[0] = '#';
		LINKED[fileptr].nextPointer = -1;
		if (previous == -1 && next==-1) { // the element is the last in the linked list no next and z==-1 means that we need to modify also SK to be -1 
			SKIDX[idx].ListFilePointer = -1;
		}
		else if (previous == -1 && next != -1) { // hena lw mafy4 7ad 2ablya w mafy4 
			LINKED[next].nextPointer = previous;
		}
		else if (previous != -1 && next != -1) { //fy 7ad b3dya w 7ad 2ablya
			LINKED[next].nextPointer = previous;
		}
		else if (previous != -1 && next == -1) {
			SKIDX[idx].ListFilePointer = previous;
		}
	}
	cout << "The Book has been deleted\n";
}
void Books::DeleteAuthorID(char *Author_ID) {
	int  x=AuthorgetAddress(Author_ID);
	if (x == -1) { cout << "This Author has no Books\n"; return; }
	x = SKIDX[x].ListFilePointer;
	vector <char*> books;
	int  n;
	do {if (x == -1)break;
		books.push_back(LINKED[x].Book_ID);
		if (LINKED[x].nextPointer == -1) break;
		else x = LINKED[x].nextPointer;
		
	} while (1);
	for (auto I : books) { cout << I << endl; DeleteBookID(I); }
	cout << "The Author has been deleted\n";
}

where Query::analyze_where_condition(string cond) {
	where temp;
	int idx;
	for (int i = 0; i < cond.size(); i++) {
		if (cond[i] == '=') {
			idx = i;
			break;
		}
	}
	temp.consdition = cond.substr(0, idx);
	temp.value = cond.substr(idx + 1);
	return temp;
}

void Query::execute(Authors &a, Books &b)
{

	cout << "Enter a Query in the correct format\n"
		<< "Press CTR-Z to execute the query\n";
	char ch; string query;
	// getting the query
	while (!cin.eof())
	{
		ch = cin.get();
		if (ch == ' ' || ch == '\t' || ch == '\n')
			continue;
		query += (char)tolower(ch);
	}
	cin.clear();

	// the attributes of books
	// Book_ID (primary key) , Author_ID (sec. key ) , Book_Title , Book_Price
	if ((query.substr(0, 6) == "select") && (query.find("frombooks", 6)) < query.size() && (query.find("frombooksand", 6)) > query.size()) {
		int idx; // the end index of "frombooks"
		string selected = ""; where r;
		for (unsigned int i = 6; i < query.size(); i++) {
			if (query.substr(i, 9) == "frombooks") {
				idx = i + 9;
				break;
			}
			selected += query[i]; // the attributes selected
		}

		if (query.size() - 1 != idx) {// if ther is a where statment
			query = query.substr(idx); // the query from "where" statment
			if (query.substr(0, 5) == "where") {
				string str = query.substr(5); // where consdition
				r = analyze_where_condition(str);
			}
		}
		else // there is no where statment
			query = "0";

		if (selected == "all") {
			if (query == "0") {
				// print all data of the books file without conditions
				fstream f("book.txt", ios::in);
				f.seekg(0, ios::beg); int n; char c;
				
				cout << setw(5) << "Book_ID" << setw(20) << "Author_ID" << setw(20) << "Book_Title" << setw(20) << "Book_Price" << endl;
				while (!f.eof())
				{
					f >> n;
					f.get(c);
					book temp;
					f.getline(temp.Book_ID, 12, '|'); f.getline(temp.Author_ID, 29, '|'); f.getline(temp.Book_Title, 49, '|'); f.getline(temp.Book_Price, 10, '|');
					if (c == '*')
						continue;
					cout << setw(5) << temp.Book_ID << setw(20) << temp.Author_ID << setw(20) << temp.Book_Title << setw(20) << temp.Book_Price << endl;

				}
			}
			else if (r.consdition == "book_id") {
				// print all data of the books file with condition r.consdition = r.value [book_id (primary key ) ==> r.value]
				cout << setw(5) << "Book_ID" << setw(20) << "Author_ID" << setw(20) << "Book_Title" << setw(20) << "Book_Price" << endl;
				int n;
				int idx;
				int x = b.GetBookIDOffset(&r.value[0], idx);

				if (x != -1) {

					fstream f("book.txt", ios::in);
					f.seekg(x, ios::beg); int n; char c;
					f >> n;
					f.get(c);
					book temp;
					f.getline(temp.Book_ID, 12, '|'); f.getline(temp.Author_ID, 29, '|'); f.getline(temp.Book_Title, 49, '|'); f.getline(temp.Book_Price, 10, '|');
					if (c != '*'){
						cout << setw(5) << temp.Book_ID << setw(20) << temp.Author_ID << setw(20) << temp.Book_Title << setw(20) << temp.Book_Price << endl;
					}

				}
			}
			else if (r.consdition == "author_id") {
				// print all data of the books file with condition r.consdition = r.value [author_id (secondary key ) ==> r.value]
				int idx = b.AuthorgetAddress(&r.value[0]);
				cout << "befor condition idx\n";
				if (idx != -1) {
					cout << "idx : " << idx << endl;
					int nxt = b.SKIDX[idx].ListFilePointer;
					do {
						b.PrintBook(b.LINKED[nxt].Book_ID);
						nxt = b.LINKED[nxt].nextPointer;
						if (nxt == -1)break;
					} while (1);
				}
			}
		}
		else if (selected == "author_id") {
			if (query == "0") {
				// print Author_ID attribute of the books file
				fstream f("book.txt", ios::in);
				f.seekg(0, ios::beg); int n; char c;

				cout << setw(5) << "Author_ID" << endl;
				while (!f.eof())
				{
					f >> n;
					f.get(c);
					book temp;
					f.getline(temp.Book_ID, 12, '|'); f.getline(temp.Author_ID, 29, '|'); f.getline(temp.Book_Title, 49, '|'); f.getline(temp.Book_Price, 10, '|');
					if (c == '*')
						continue;
					cout << setw(5) << temp.Author_ID << endl;

				}
			}
			else if (r.consdition == "book_id") {
				// print book_id attribute (primary key) of the books file with condition
				cout << setw(5) << "Author_ID"<< endl;
				int n;
				int idx;
				int x = b.GetBookIDOffset(&r.value[0], idx);

				if (x != -1) {

					fstream f("book.txt", ios::in);
					f.seekg(x, ios::beg); int n; char c;
					f >> n;
					f.get(c);
					book temp;
					f.getline(temp.Book_ID, 12, '|'); f.getline(temp.Author_ID, 29, '|'); f.getline(temp.Book_Title, 49, '|'); f.getline(temp.Book_Price, 10, '|');
					if (c != '*') {
						cout << setw(5) << temp.Author_ID << endl;
					}

				}
			}
			else if (r.consdition == "author_id") {
				// print book_id attribute (primary key) of the books file with condition
				/*******************************************
				****************************************
				****************************************/
				cout << "Book_ID with (secondary key) ==> " << r.consdition << " = " << r.value << endl;
			}
		}
		else if (selected == "book_id") {
			if (query == "0") {
				// print book_id attribute (primary key) of the books file
				fstream f("book.txt", ios::in);
				f.seekg(0, ios::beg); int n; char c;

				cout << setw(5) << "Book_ID" << endl;
				while (!f.eof())
				{
					f >> n;
					f.get(c);
					book temp;
					f.getline(temp.Book_ID, 12, '|'); f.getline(temp.Author_ID, 29, '|'); f.getline(temp.Book_Title, 49, '|'); f.getline(temp.Book_Price, 10, '|');
					if (c == '*')
						continue;
					cout << setw(5) << temp.Book_ID << endl;

				}
			}
			else if (r.consdition == "book_id") {
				// print book_id attribute (primary key) of the books file with condition
				cout << setw(5) << "Book_ID" << endl;
				int n;
				int idx;
				int x = b.GetBookIDOffset(&r.value[0], idx);

				if (x != -1) {

					fstream f("book.txt", ios::in);
					f.seekg(x, ios::beg); int n; char c;
					f >> n;
					f.get(c);
					book temp;
					f.getline(temp.Book_ID, 12, '|'); f.getline(temp.Author_ID, 29, '|'); f.getline(temp.Book_Title, 49, '|'); f.getline(temp.Book_Price, 10, '|');
					if (c != '*') {
						cout << setw(5) << temp.Book_ID << endl;
					}

				}
			}
			else if (r.consdition == "author_id") {
				// print book_id attribute (primary key) of the books file with condition
				/*******************************************
				****************************************
				****************************************/
				cout << "Book_ID with (secondary key) ==> " << r.consdition << " = " << r.value << endl;
			}
		}
		else if (selected == "book_title") {
			if (query == "0") {
				// print book_Title attribute of the books file
				fstream f("book.txt", ios::in);
				f.seekg(0, ios::beg); int n; char c;

				cout << setw(5) << "Book_Title" << endl;
				while (!f.eof())
				{
					f >> n;
					f.get(c);
					book temp;
					f.getline(temp.Book_ID, 12, '|'); f.getline(temp.Author_ID, 29, '|'); f.getline(temp.Book_Title, 49, '|'); f.getline(temp.Book_Price, 10, '|');
					if (c == '*')
						continue;
					cout << setw(5) << temp.Book_Title << endl;

				}
			}
			else if (r.consdition == "book_id") {
				// print book_Title attribute of the books file with condition
				cout << setw(5) << "Book_Title" << endl;
				int n;
				int idx;
				int x = b.GetBookIDOffset(&r.value[0], idx);

				if (x != -1) {

					fstream f("book.txt", ios::in);
					f.seekg(x, ios::beg); int n; char c;
					f >> n;
					f.get(c);
					book temp;
					f.getline(temp.Book_ID, 12, '|'); f.getline(temp.Author_ID, 29, '|'); f.getline(temp.Book_Title, 49, '|'); f.getline(temp.Book_Price, 10, '|');
					if (c != '*') {
						cout << setw(5) << temp.Book_Title << endl;
					}

				}
			}
			else if (r.consdition == "author_id") {
				// print book_Title attribute of the books file with condition
				/*******************************************
				****************************************
				****************************************/
				cout << "book_Title with (secondary key) ==> " << r.consdition << " = " << r.value << endl;
			}
		}
		else if (selected == "book_price") {
			if (query == "0") {
				// print Book_Price attribute of the books file
				fstream f("book.txt", ios::in);
				f.seekg(0, ios::beg); int n; char c;

				cout << setw(5) << "Book_Price" << endl;
				while (!f.eof())
				{
					f >> n;
					f.get(c);
					book temp;
					f.getline(temp.Book_ID, 12, '|'); f.getline(temp.Author_ID, 29, '|'); f.getline(temp.Book_Title, 49, '|'); f.getline(temp.Book_Price, 10, '|');
					if (c == '*')
						continue;
					cout << setw(5)<< temp.Book_Price << endl;

				}
			}
			else if (r.consdition == "book_id") {
				// print Book_Price attribute of the books file with condition
				cout << setw(5) << "Book_Price" << endl;
				int n;
				int idx;
				int x = b.GetBookIDOffset(&r.value[0], idx);

				if (x != -1) {

					fstream f("book.txt", ios::in);
					f.seekg(x, ios::beg); int n; char c;
					f >> n;
					f.get(c);
					book temp;
					f.getline(temp.Book_ID, 12, '|'); f.getline(temp.Author_ID, 29, '|'); f.getline(temp.Book_Title, 49, '|'); f.getline(temp.Book_Price, 10, '|');
					if (c != '*') {
						cout << setw(5) << temp.Book_Price << endl;
					}

				}
			}
			else if (r.consdition == "author_id") {
				// print Book_Price attribute of the books file with condition
				/*******************************************
				****************************************
				****************************************/
				cout << "Book_Price with (secondary key) ==> " << r.consdition << " = " << r.value << endl;
			}
		}
	}

	// the attributes of authors
	// Author_ID (primary key) , Author_Name (sec. key ) , Author_Address , Author_Mobile
	if ((query.substr(0, 6) == "select") && (query.find("fromauthors", 6) < query.size()) && (query.find("fromauthorsand", 6) > query.size())) {
		int idx; // the end index of "fromauthors"
		string selected = ""; where r;
		for (unsigned int i = 6; i < query.size(); i++) {
			if (query.substr(i, 11) == "fromauthors") {
				idx = i + 11;
				break;
			}
			selected += query[i]; // the attributes selected
		}

		if (query.size() - 1 != idx) {// if ther is a where statment
			query = query.substr(idx); // the query from "where" statment
			if (query.substr(0, 5) == "where") {
				string str = query.substr(5); // where consdition
				r = analyze_where_condition(str);
			}
		}
		else // there is no where statment
			query = "0";

		if (selected == "all") {
			if (query == "0") {
				// print all data of the authors file
				cout << "Author_ID		Author_Name		Author_Address		Author_Mobile" << endl;
				fstream f("Author.txt", ios::in);
				f.seekg(0, ios::beg); int n; char c;
				while(!f.eof()) {
					f >> n;
					f.get(c); 
					author temp;
					f.getline(temp.Author_ID, 29, '|'); f.getline(temp.Author_Name, 49, '|'); f.getline(temp.Author_Address, 49, '|'); f.getline(temp.Author_Mobile, 10, '|');
					if (c == '*')
						continue;
					cout <<setw(5)<< temp.Author_ID << setw(30) << temp.Author_Name << setw(25) << temp.Author_Address << setw(25) << temp.Author_Mobile << endl;
				}
			}
			else if (r.consdition == "author_id") {
				// print all data of the authors file with condition
				cout << "Author_ID		Author_Name		Author_Address		Author_Mobile" << endl;
				int n;
				int idx;
				int x = a.GetAuthorIDOffset(&r.value[0], idx);

				if (x != -1) {

					fstream f("Author.txt", ios::in);
					f.seekg(x, ios::beg); int n; char c;
					f >> n;
					f.get(c);
					author temp;
					f.getline(temp.Author_ID, 29, '|'); f.getline(temp.Author_Name, 49, '|'); f.getline(temp.Author_Address, 49, '|'); f.getline(temp.Author_Mobile, 10, '|');
					if (c != '*') {
						cout << setw(5) << temp.Author_ID << setw(30) << temp.Author_Name << setw(25) << temp.Author_Address << setw(25) << temp.Author_Mobile << endl;
					}

				}
			}
			else if (r.consdition == "author_name") {
				// print all data of the authors file with condition
				/*******************************************
				****************************************
				****************************************/
				cout << "all with (sacondary key) ==> " << r.consdition << " = " << r.value << endl;
			}
		}
		else if (selected == "author_id") {
			if (query == "0") {
				// print Author_ID attribute (primary key) of the authors file
				cout << "Author_ID" << endl;
				fstream f("Author.txt", ios::in);
				f.seekg(0, ios::beg); int n; char c;
				while (!f.eof()) {
					f >> n;
					f.get(c);
					author temp;
					f.getline(temp.Author_ID, 29, '|'); f.getline(temp.Author_Name, 49, '|'); f.getline(temp.Author_Address, 49, '|'); f.getline(temp.Author_Mobile, 10, '|');
					if (c == '*')
						continue;
					cout << setw(5) << temp.Author_ID << endl;
				}
			}
			else if (r.consdition == "author_id") {
				// print author_id attribute (primary key) of the authors file with condition
				cout << "Author_ID"<< endl;
				int n;
				int idx;
				int x = a.GetAuthorIDOffset(&r.value[0], idx);

				if (x != -1) {

					fstream f("Author.txt", ios::in);
					f.seekg(x, ios::beg); int n; char c;
					f >> n;
					f.get(c);
					author temp;
					f.getline(temp.Author_ID, 29, '|'); f.getline(temp.Author_Name, 49, '|'); f.getline(temp.Author_Address, 49, '|'); f.getline(temp.Author_Mobile, 10, '|');
					if (c != '*') {
						cout << setw(5) << temp.Author_ID << endl;
					}
				}
			}
			else if (r.consdition == "author_name") {
				// print author_id attribute (primary key) of the authors file with condition
				/*******************************************
				****************************************
				****************************************/
				cout << "author_id with (secondary key) ==> " << r.consdition << " = " << r.value << endl;
			}
		}
		else if (selected == "author_name") {
			if (query == "0") {
				// print author_name attribute of the authors file
				cout << "Author_Name" << endl;
				fstream f("Author.txt", ios::in);
				f.seekg(0, ios::beg); int n; char c;
				while (!f.eof()) {
					f >> n;
					f.get(c);
					author temp;
					f.getline(temp.Author_ID, 29, '|'); f.getline(temp.Author_Name, 49, '|'); f.getline(temp.Author_Address, 49, '|'); f.getline(temp.Author_Mobile, 10, '|');
					if (c == '*')
						continue;
					cout << setw(5) << temp.Author_Name << endl;
				}
			}
			else if (r.consdition == "author_id") {
				// print author_name attribute of the authors file with condition
				cout << "Author_Name" << endl;
				int n;
				int idx;
				int x = a.GetAuthorIDOffset(&r.value[0], idx);

				if (x != -1) {

					fstream f("Author.txt", ios::in);
					f.seekg(x, ios::beg); int n; char c;
					f >> n;
					f.get(c);
					author temp;
					f.getline(temp.Author_ID, 29, '|'); f.getline(temp.Author_Name, 49, '|'); f.getline(temp.Author_Address, 49, '|'); f.getline(temp.Author_Mobile, 10, '|');
					if (c != '*') {
						cout << setw(5)<< temp.Author_Name << endl;
					}

				}
			}
			else if (r.consdition == "author_name") {
				// print author_name attribute of the authors file with condition
				/*******************************************
				****************************************
				****************************************/
				cout << "author_name with (secondary key) ==> " << r.consdition << " = " << r.value << endl;
			}
		}
		else if (selected == "author_address") {
			if (query == "0") {
				// print Author_Address attribute of the authors file
				cout << "Author_Address"<< endl;
				fstream f("Author.txt", ios::in);
				f.seekg(0, ios::beg); int n; char c;
				while (!f.eof()) {
					f >> n;
					f.get(c);
					author temp;
					f.getline(temp.Author_ID, 29, '|'); f.getline(temp.Author_Name, 49, '|'); f.getline(temp.Author_Address, 49, '|'); f.getline(temp.Author_Mobile, 10, '|');
					if (c == '*')
						continue;
					cout << setw(5) << temp.Author_Address << endl;
				}
			}
			else if (r.consdition == "author_id") {
				// print Author_Address attribute of the authors file with condition
				cout << "Author_Address" << endl;
				int n;
				int idx;
				int x = a.GetAuthorIDOffset(&r.value[0], idx);

				if (x != -1) {

					fstream f("Author.txt", ios::in);
					f.seekg(x, ios::beg); int n; char c;
					f >> n;
					f.get(c);
					author temp;
					f.getline(temp.Author_ID, 29, '|'); f.getline(temp.Author_Name, 49, '|'); f.getline(temp.Author_Address, 49, '|'); f.getline(temp.Author_Mobile, 10, '|');
					if (c != '*') {
						cout << setw(5)<< temp.Author_Address << endl;
					}

				}
			}
			else if (r.consdition == "author_name") {
				// print Author_Address attribute of the authors file with condition
				/*******************************************
				****************************************
				****************************************/
				cout << "Author_Address with (secondary key) ==> " << r.consdition << " = " << r.value << endl;
			}
		}
		else if (selected == "author_mobile") {
			if (query == "0") {
				// print author_mobile attribute of the authors file
				cout << "Author_Mobile" << endl;
				fstream f("Author.txt", ios::in);
				f.seekg(0, ios::beg); int n; char c;
				while (!f.eof()) {
					f >> n;
					f.get(c);
					author temp;
					f.getline(temp.Author_ID, 29, '|'); f.getline(temp.Author_Name, 49, '|'); f.getline(temp.Author_Address, 49, '|'); f.getline(temp.Author_Mobile, 10, '|');
					if (c == '*')
						continue;
					cout << setw(5) << temp.Author_Mobile << endl;
				}
			}
			else if (r.consdition == "author_id") {
				// print author_mobile attribute of the authors file with condition
				cout << "Author_Mobile" << endl;
				int n;
				int idx;
				int x = a.GetAuthorIDOffset(&r.value[0], idx);

				if (x != -1) {

					fstream f("Author.txt", ios::in);
					f.seekg(x, ios::beg); int n; char c;
					f >> n;
					f.get(c);
					author temp;
					f.getline(temp.Author_ID, 29, '|'); f.getline(temp.Author_Name, 49, '|'); f.getline(temp.Author_Address, 49, '|'); f.getline(temp.Author_Mobile, 10, '|');
					if (c != '*') {
						cout << setw(5) << temp.Author_Mobile << endl;
					}

				}
			}
			else if (r.consdition == "author_name") {
				// print author_mobile attribute of the authors file with condition
				/*******************************************
				****************************************
				****************************************/
				cout << "author_mobile with (secondary key) ==> " << r.consdition << " = " << r.value << endl;
			}
		}
	}

	// it should creat a new file called "join" to join the current data of both books and authers where (author_id = author_id) in the mode of trunc
	// the attributes of join 
	// Book_ID (primary key) , Author_ID (sec. key )  , Author_Name (sec. key ) , Book _Title , Book_Price  , Author_Address , Author_Mobile
	if ((query.substr(0, 6) == "select") && ((query.find("fromauthorsandbooks", 6) < query.size()) || (query.find("frombooksandauthors", 6) < query.size()))) {
		int idx; // the end index of "fromauthors"
		string selected = ""; where r;
		for (unsigned int i = 6; i < query.size(); i++) {
			if (query.substr(i, 19) == "fromauthorsandbooks" || query.substr(i, 19) == "frombooksandauthors") {
				idx = i + 19;
				break;
			}
			selected += query[i]; // the attributes selected
		}

		if (query.size() - 1 != idx) {// if ther is a where statment
			query = query.substr(idx); // the query from "where" statment
			if (query.substr(0, 5) == "where") {
				string str = query.substr(5); // where consdition
				r = analyze_where_condition(str);
			}
		}
		else // there is no where statment
			query = "0";

		if (selected == "all") {  
			if (query == "0") {
				// print all data of the join file without conditions
				cout << setw(5) << "Author_ID"<< setw(20)<<"Author_Name" << setw(20)<< "Author_Address" << setw(20)<< "Author_Mobile" << setw(20) << "Book_ID" << setw(20)<< "Book_Title" << setw(20)<<"Book_Price" << endl;
				fstream f_A("Author.txt", ios::in); fstream f_B("book.txt", ios::in);
				f_A.seekg(0, ios::beg); f_B.seekg(0, ios::beg); int n; char c;
				do {
					f_A >> n;
					if (f_A.fail())
						break;
					f_A.get(c);
					if (f_A.fail())
						break;
					author temp;
					f_A.getline(temp.Author_ID, 29, '|'); f_A.getline(temp.Author_Name, 49, '|'); f_A.getline(temp.Author_Address, 49, '|'); f_A.getline(temp.Author_Mobile, 10, '|');
					if (c == '*')
						continue;
					while (!f_B.eof())
					{
						f_B >> n;
						f_B.get(c);
						book temp_B;
						f_B.getline(temp_B.Book_ID, 12, '|'); f_B.getline(temp_B.Author_ID, 29, '|'); f_B.getline(temp_B.Book_Title, 49, '|'); f_B.getline(temp_B.Book_Price, 10, '|');
						if (c == '*') {
							strcmp(temp_B.Book_ID  , "NULL");
							strcmp(temp_B.Author_ID, "NULL");
							strcmp(temp_B.Book_Title, "NULL");
							strcmp(temp_B.Book_Price, "NULL");
						}
						if (strcmp(temp.Author_ID, temp_B.Author_ID) == 0) {
							cout << setw(5) << temp.Author_ID << setw(20) << temp.Author_Name << setw(20) << temp.Author_Address << setw(20) << temp.Author_Mobile << setw(20) << temp_B.Book_ID << setw(20) << temp_B.Book_Title << setw(20) << temp_B.Book_Price << endl;
						}
						
					}
				}while (true);

				
			}
			//else if (r.consdition == "author_id") {
			//	// print all data of the authors file with condition
			//	/*******************************************
			//	****************************************
			//	****************************************/
			//	cout << "all with (primary key ) ==> " << r.consdition << " = " << r.value << endl;
			//}
			//else if (r.consdition == "author_name") {
			//	// print all data of the authors file with condition
			//	/*******************************************
			//	****************************************
			//	****************************************/
			//	cout << "all with (sacondary key) ==> " << r.consdition << " = " << r.value << endl;
			//}
		}
		//else if (selected == "author_id") {
		//	if (query == "0") {
		//		// print Author_ID attribute (primary key) of the authors file
		//		/*******************************************
		//		****************************************
		//		****************************************/
		//		cout << "Author_ID without conditions......................\n";
		//	}
		//	else if (r.consdition == "author_id") {
		//		// print author_id attribute (primary key) of the authors file with condition
		//		/*******************************************
		//		****************************************
		//		****************************************/
		//		cout << "author_id with (primary key ) ==> " << r.consdition << " = " << r.value << endl;
		//	}
		//	else if (r.consdition == "author_name") {
		//		// print author_id attribute (primary key) of the authors file with condition
		//		/*******************************************
		//		****************************************
		//		****************************************/
		//		cout << "author_id with (secondary key) ==> " << r.consdition << " = " << r.value << endl;
		//	}
		//}
		//else if (selected == "author_name") {
		//	if (query == "0") {
		//		// print author_name attribute of the authors file
		//		/*******************************************
		//		****************************************
		//		****************************************/
		//		cout << "author_name without conditions......................\n";
		//	}
		//	else if (r.consdition == "author_id") {
		//		// print author_name attribute of the authors file with condition
		//		/*******************************************
		//		****************************************
		//		****************************************/
		//		cout << "author_name with (primary key ) ==> " << r.consdition << " = " << r.value << endl;
		//	}
		//	else if (r.consdition == "author_name") {
		//		// print author_name attribute of the authors file with condition
		//		/*******************************************
		//		****************************************
		//		****************************************/
		//		cout << "author_name with (secondary key) ==> " << r.consdition << " = " << r.value << endl;
		//	}
		//}
		//else if (selected == "author_address") {
		//	if (query == "0") {
		//		// print Author_Address attribute of the authors file
		//		/*******************************************
		//		****************************************
		//		****************************************/
		//		cout << "Author_Address without conditions......................\n";
		//	}
		//	else if (r.consdition == "author_id") {
		//		// print Author_Address attribute of the authors file with condition
		//		/*******************************************
		//		****************************************
		//		****************************************/
		//		cout << "Author_Address with (primary key ) ==> " << r.consdition << " = " << r.value << endl;
		//	}
		//	else if (r.consdition == "author_name") {
		//		// print Author_Address attribute of the authors file with condition
		//		/*******************************************
		//		****************************************
		//		****************************************/
		//		cout << "Author_Address with (secondary key) ==> " << r.consdition << " = " << r.value << endl;
		//	}
		//}
		//else if (selected == "author_mobile") {
		//	if (query == "0") {
		//		// print author_mobile attribute of the authors file
		//		/*******************************************
		//		****************************************
		//		****************************************/
		//		cout << "author_mobile without conditions......................\n";
		//	}
		//	else if (r.consdition == "author_id") {
		//		// print author_mobile attribute of the authors file with condition
		//		/*******************************************
		//		****************************************
		//		****************************************/
		//		cout << "author_mobile with (primary key ) ==> " << r.consdition << " = " << r.value << endl;
		//	}
		//	else if (r.consdition == "author_name") {
		//		// print author_mobile attribute of the authors file with condition
		//		/*******************************************
		//		****************************************
		//		****************************************/
		//		cout << "author_mobile with (secondary key) ==> " << r.consdition << " = " << r.value << endl;
		//	}
		//}
	}

}
int main() 
{
	Authors a;fstream fa("Author.txt", ios::out);fstream fb("book.txt", ios::out);
	Books b; 
/*	int n;cout <<  "Enter number of Authors" << endl;;cin >> n;	cin.ignore(); //we should put ignore in the options below when we are to getline

	for (int i = 0; i <n; i++) 
		b.AddAuthor();
	char c[] = "joe";
	b.SortAuthors();
	b.PrintAuthorID(c);*/

string c; int x;  
	while (1) {
		cout << "(1)  Add New Book\n(2)  Add New Author\n(3)  Delete Book (ID)\n"
			<< "(4)  Delete Author (ID)\n(5)  Print Book(ID) \n(6)  Print Book(Author ID)\n(7)  Print Author(ID)\n"
			<< "(8)  Print Author(name)\n(9)  Write a Query\n(10) Exit \n";
		cin >> c;
		if (c[0]<='10' && c[0]>'0') {
			strstream obj; obj << c;obj >> x;
		}
		else continue;
		if (x == 1) { cin.ignore(); b.AddBook();b.SortBooks(); }
		else if (x == 2) { cin.ignore();a.AddAuthor();a.SortAuthors(); }
		else if (x == 3) { cin >> c; b.DeleteBookID(&c[0]); }
		else if (x == 4) { cin >> c;  a.DeleteAuthorID(&c[0]);b.DeleteAuthorID(&c[0]); }
		else if (x == 5) { cin >> c; c +='0'; b.PrintBook(&c[0]); }
		else if (x == 6) { cin >> c;  b.PrintAuthorBooks(&c[0]); }
		else if (x == 7) { cin >> c; c += '0'; a.PrintAuthorID(&c[0]); }
		else if (x==8 ){ cin >> c; a.PrintAuthorName(&c[0]); }
		else if (x == 9) { Query q; q.execute(a , b); }
		else if (x == 10) { a.SaveIdx();b.SaveIdx(); break; }
	} 
	
	//system("pause");
    return 0;
}

