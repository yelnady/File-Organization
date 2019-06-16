#include "pch.h"
#include "Author.h"
#include "all.h"
//#include <all.h>
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
	cout << "Author ID: "; cin.getline(temp.Author_ID, 29);
	cout << "Author Name: "; cin.getline(temp.Author_Name, 49);
	cout << "Author Address: "; cin.getline(temp.Author_Address, 49);
	cout << "Author Mobile: "; cin.getline(temp.Author_Mobile, 10);

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
	for (int i = 0; i < PKIDX.size(); i++) {
		string pk1(PKIDX[i].Author_ID);
		format(pk1, 30);
		PKfile << pk1 << " " << PKIDX[i].offset << endl;
	}
	for (int i = 0; i < SKIDX.size(); i++) {
		string sk1(SKIDX[i].Author_Name);
		format(sk1, 50);
		SKfile << sk1 << " " << SKIDX[i].ListFilePointer << endl;
	}
	for (int i = 0; i < LINKED.size(); i++) {
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
		strcpy_s(sktemp.Author_Name, Author_Name); sktemp.ListFilePointer = (int)(LINKED.size() - 1);
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
	string aa(Author_Name);
	int low = 0, high = (int)(SKIDX.size() - 1);
	while (low <= high) {
		int mid = (low + high) / 2;
		if (strcmp(SKIDX[mid].Author_Name, &aa[0]) == 0) { return mid; } //error here cuz we increase the size of ID into albta3a
		if (strcmp(SKIDX[mid].Author_Name, &aa[0]) < 0) low = mid + 1;
		else  high = mid - 1;
	}
	return -1;
}

void Authors::SortAuthors() {
	sort(PKIDX.begin(), PKIDX.end());
	sort(SKIDX.begin(), SKIDX.end());
}

int Authors::GetAuthorIDOffset(char* ID, int &idx) { //return the offset in the data file and the index in the pk file 
	string aa(ID);
	int low = 0, high = (int)(PKIDX.size() - 1);
	while (low <= high) {
		int mid = (low + high) / 2;
		if (strcmp(PKIDX[mid].Author_ID, &aa[0]) == 0) { idx = mid; return PKIDX[mid].offset; }
		if (strcmp(PKIDX[mid].Author_ID, &aa[0]) < 0)low = mid + 1;
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
		f.seekg(x, ios::beg); int n; char c;
		f >> n;
		f.get(c); if (c == '*') {
			cout << "Author isn't found2\n"; return 0;
		}
		Author temp;
		f.getline(temp.Author_ID, 29, '|'); f.getline(temp.Author_Name, 49, '|'); f.getline(temp.Author_Address, 49, '|');
		f.getline(temp.Author_Mobile, 10, '|');
		cout << "Author ID: " << temp.Author_ID << " /Author Name: " << temp.Author_Name << " /Author Address: " << temp.Author_Address
			<< " /Author Mobile: " << temp.Author_Mobile << endl;
	}
	return 1;
}

bool Authors::PrintAuthorName(char *Author_Name) {
	int idx = AuthorNamegetAddress(Author_Name);
	if (idx == -1) {
		cout << "This Author isn't found\n"; return 0;
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

		f.getline(temp.Author_ID, 29, '|'); f.getline(temp.Author_Name, 49, '|');
		f.seekp(x + sizeof(n), ios::beg);
		string fill(n, '-'); fill = "*" + fill;
		f.write(&fill[0], n);

		/*Delete in PKIDX vector*/
		PKIDX.erase(PKIDX.begin() + idx);
		/*no need to update secondary key as if he searched for this Pk will return not found*/
		idx = AuthorNamegetAddress(temp.Author_Name); //now we have the idx in SKIDX vector
		int fileptr;
		for (int i = 0; i < LINKED.size(); i++) {
			if (strcmp(LINKED[i].Author_ID, Author_ID) == 0) { fileptr = i; break; }
		}
		int previous;
		if (fileptr != -1) previous = LINKED[fileptr].prev;
		else previous = -1;
		int next = -1;
		for (int i = 0; i < LINKED.size(); i++) {
			if (fileptr == LINKED[i].prev) { next = i; break; }
		}
		LINKED[fileptr].Author_ID[0] = '#';
		LINKED[fileptr].prev = -1;
		if (previous == -1 && next == -1) { // the element is the last in the linked list no next and z==-1 means that we need to modify also SK to be -1 
			SKIDX[idx].ListFilePointer = -1;
		}
		else if (previous == -1 && next != -1) { // hena lw mafy4 7ad 2ablya w mafy4 
			LINKED[next].prev = previous;
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
	if (x == -1) { cout << "Author Name isn't found\n"; return; }
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