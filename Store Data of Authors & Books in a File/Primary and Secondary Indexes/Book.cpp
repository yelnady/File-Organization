#include "pch.h"
#include "Book.h"
#include "Author.h"
#include "all.h"
//#include <all.h>
void format(string &a, int len) {
	while (a.size() < len) {
		a += ' ';
	}
	return;
}

void Books::AddBook(Authors &a) {
	fstream f("book.txt", ios::out | ios::app);
	f.seekp(0, ios::end);
	int nowIdx = (int)f.tellp(); /*we will write it into offset of pk array*/

	book temp;
	cout << "Book ID: "; cin.getline(temp.Book_ID, 12);
	cout << "Author ID: "; cin.getline(temp.Author_ID, 30);
	cout << "Book Title: "; cin.getline(temp.Book_Title, 50);
	cout << "Book Price: "; cin >> temp.Book_Price;
	int n;
	if (a.GetAuthorIDOffset(temp.Author_ID, n) == -1) {
		cout << "please Enter the Data for this author too" << endl;
		cin.ignore(); a.AddAuthor(); a.SortAuthors();
	}
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
		strcpy_s(sktemp.Author_ID, Author_ID); sktemp.ListFilePointer = LINKED.size() - 1;
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
	string idd(Author_ID);
	while (low <= high) {
		int mid = (low + high) / 2;
		if (strcmp(SKIDX[mid].Author_ID, &idd[0]) == 0) { return mid; } //error here cuz we increase the size of ID into albta3a
		if (strcmp(SKIDX[mid].Author_ID, &idd[0]) < 0) low = mid + 1;
		else  high = mid - 1;
	}
	return -1;
}
int  Books::AuthorgetAddressseq(char* ID) {
	for (int i = 0; i < SKIDX.size(); i++) {
		if (strcmp(SKIDX[i].Author_ID, ID) == 0) return i;
	}
	return -1;
}
int Books::GetBookIDOffset(char* ID, int &idx) { //return the offset in the data file and the index in the pk file 
	string aa(ID);
	int low = 0, high = PKIDX.size() - 1;
	while (low <= high) {
		int mid = (low + high) / 2;
		if (strcmp(PKIDX[mid].Book_ID, &aa[0]) == 0)
		{
			idx = mid;
			return PKIDX[mid].offset;
		}
		if (strcmp(PKIDX[mid].Book_ID, &aa[0]) < 0)low = mid + 1;
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
	for (int i = 0; i < PKIDX.size(); i++) {
		string pk1(PKIDX[i].Book_ID);
		format(pk1, 13);
		PKfile << pk1 << " " << PKIDX[i].offset << endl;
	}
	for (int i = 0; i < SKIDX.size(); i++) {
		string sk1(SKIDX[i].Author_ID);
		format(sk1, 30);
		SKfile << sk1 << " " << SKIDX[i].ListFilePointer << endl;
	}
	for (int i = 0; i < LINKED.size(); i++) {
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
		do {
			PrintBook(LINKED[nxt].Book_ID);
			nxt = LINKED[nxt].nextPointer;
			if (nxt == -1)break;
		} while (1);
	}
}
bool Books::PrintBook(char *Book_ID) {
	int n;
	int x = GetBookIDOffset(Book_ID, n);
	if (x == -1) { cout << "Book isn't found1\n"; return 0; }
	else {
		fstream f("book.txt", ios::in);
		f.seekg(x, ios::beg); int n; char c;
		f >> n;
		f.get(c); if (c == '*') {
			cout << "Book isn't found2\n"; return 0;
		}

		book temp;
		f.getline(temp.Book_ID, 12, '|'); f.getline(temp.Author_ID, 29, '|'); f.getline(temp.Book_Title, 49, '|'); f >> temp.Book_Price;
		cout << "Book ID: " << temp.Book_ID << " /Book Author: " << temp.Author_ID << " /Book Title: " << temp.Book_Title
			<< " /Book Price: " << temp.Book_Price << endl;
	}
	return 1;
}
void Books::PrintAll() {

}
void Books::DeleteBookID(char *Book_ID) {
	int idx;
	int x = GetBookIDOffset(Book_ID, idx);
	if (x == -1) { cout << "Book isn't found\n"; }
	else {
		/*Delete in Data file*/
		fstream f("book.txt", ios::out | ios::in);
		f.seekp(x, ios::beg);
		short n; f >> n;
		book temp;
		f.getline(temp.Book_ID, 12, '|'); f.getline(temp.Author_ID, 29, '|');
		f.seekp(x + sizeof(n), ios::beg);
		string fill(n, '-'); fill = "*" + fill;
		f.write(&fill[0], n);
		/*Delete in PKIDX vector*/
		PKIDX.erase(PKIDX.begin() + idx);
		/*no need to update secondary key as if he searched for this Pk will return not found*/
		idx = AuthorgetAddress(temp.Author_ID); //now we have the idx in SKIDX vector
		int fileptr;
		for (int i = 0; i < LINKED.size(); i++) {
			if (strcmp(LINKED[i].Book_ID, Book_ID) == 0) { fileptr = i; break; }
		}
		int previous;
		if (fileptr != -1) previous = LINKED[fileptr].nextPointer;
		else previous = -1;
		int next = -1;
		for (int i = 0; i < LINKED.size(); i++) {
			if (fileptr == LINKED[i].nextPointer) { next = i; break; }
		}
		LINKED[fileptr].Book_ID[0] = '#';
		LINKED[fileptr].nextPointer = -1;
		if (previous == -1 && next == -1) { // the element is the last in the linked list no next and z==-1 means that we need to modify also SK to be -1 
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
	int  x = AuthorgetAddress(Author_ID);
	if (x == -1) { cout << "This Author has no Books\n"; return; }
	x = SKIDX[x].ListFilePointer;
	vector <char*> books;
	int  n;
	do {
		if (x == -1)break;
		books.push_back(LINKED[x].Book_ID);
		if (LINKED[x].nextPointer == -1) break;
		else x = LINKED[x].nextPointer;

	} while (1);
	for (auto I : books) { cout << I << endl; DeleteBookID(I); }
	cout << "The Author has been deleted\n";
}