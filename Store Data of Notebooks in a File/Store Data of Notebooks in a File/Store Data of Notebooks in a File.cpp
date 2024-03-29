#include "pch.h"
#include "all.h"
#define idd cout << "ENTER THE ID :";
#define IN_OUT fstream f("hello.txt", ios::in | ios::out);
#define IN fstream f("hello.txt", ios::in );
#define OUT fstream f("hello.txt",  ios::out);
using namespace std;
class noteBook {
	char ID[21], Brand[21], Proc[51], OS[21];
public:
	void Add() {
		fstream f("hello.txt", ios::out | ios::app);
		f.seekp(0, ios::end);
		cin.ignore();
		cout << "ID: "; cin.getline(ID, 20);
		cout << "Brand: "; cin.getline(Brand, 20);
		cout << "Processor type: "; cin.getline(Proc, 50);
		cout << "Operating system: "; cin.getline(OS, 20);
		string Idd(ID), Brandd(Brand), Procc(Proc), OSS(OS);
		string all = ' ' + Idd + '|' + Brandd + '|' + Procc + '|' + OSS + '|';
		short length = all.size();
		cout << "Your Entered Record is " << length << all << endl;
		f << length; f << all;
		f.close();
	}
	void Delete(char*id) {
		IN_OUT
			int idx = Search(f, id);
		if (idx == -1)return;
		f.seekg(idx, ios::beg);
		short n; f >> n;
		f.seekp(idx + 2, ios::beg);
		string fill(n, '-'); fill = "*" + fill;
		f.write(&fill[0], n);
		f.close();
	}
	int Search(fstream &f, char* id) {
		f.seekg(0); short n = 0; int index = 0; char c;
		while (f) {
			f >> n;
			if (f.eof())return -1;
			c = f.get(); c = f.get();
			stringstream obj; obj << n; string a; obj >> a;
			if (c == '*' || c == '-') {
				while (c == '*' || c == '-') { c = f.get(); }
				f.seekg(-1, ios::cur); index += n + a.size(); continue; // seekg brdo 34an lazm yasta yrgg3 a5r 7rf da msh zyta hya
			}
			f.seekg(-1, ios::cur);//undo the f.get() char ;
			f.getline(this->ID, 20, '|');
			f.getline(this->Brand, 20, '|'); f.getline(this->Proc, 50, '|'); f.getline(this->OS, 20, '|');
			string oldID(id), newID(ID);
			if (oldID == newID) { return index; break; } // return the begin of the length indicator
			index += n + a.size();
		}
		return -1;
	}
	void Update(char* id) {
		IN_OUT	int idx = Search(f, id);
		if (idx != -1) {
			cin.ignore();
			cout << "ID: "; cin.getline(ID, 20);
			cout << "Brand: "; cin.getline(Brand, 20);
			cout << "Processor type: "; cin.getline(Proc, 50);
			cout << "Operating system: "; cin.getline(OS, 20);
			string Idd(ID), Brandd(Brand), Procc(Proc), OSS(OS);
			string all = ' ' + Idd + '|' + Brandd + '|' + Procc + '|' + OSS + '|';
			cout << all << endl;
			short length = all.size(); int len; f.seekg(idx, ios::beg); f >> len;
			if (length == len) {
				f.seekp(idx + 2, ios::beg);
				f.write(&all[0], length);
				f.close();
			}
			else {//if the size of the new char is changed then append at the end;
				Delete(id);
				f.seekp(0, ios::end);
				f << length; f << all;
			}
		}
		else cout << "Not found\n";
	}
	string Visualize() {
		IN f.seekg(0); short n = 0; int index = 0; char c; string visual = "";
		while (1) {
			f >> n;
			if (f.eof())return visual + '\n';
			c = f.get();
			if (c == '*' || c == '-') {
				visual += '*';
				while (c == '*' || c == '-') { c = f.get(); }
				continue;
			}
			visual += '-';
			f.seekg(-1, ios::cur);//undo the f.get() char ;
			f.ignore();
			//ignore the first space so when we compare two strings will be of the same size as the sent one
			f.getline(this->ID, 20, '|');
			f.getline(this->Brand, 20, '|'); f.getline(this->Proc, 50, '|'); f.getline(this->OS, 20, '|');
		}
	}
	void Display() {
		IN char c; short n;
		while (f) {
			f >> n;
			if (f.eof())return;
			c = f.get(); c = f.get();
			stringstream obj; obj << n; string a; obj >> a;
			if (c == '*' || c == '-') {
				while (c == '*' || c == '-') { c = f.get(); if (f.eof())return; }
				f.seekg(-1, ios::cur); continue; // seekg brdo 34an lazm yasta yrgg3 a5r 7rf da msh zyta hya
			}
			f.seekg(-1, ios::cur);//undo the f.get() char ;
			f.getline(this->ID, 20, '|');
			f.getline(this->Brand, 20, '|'); f.getline(this->Proc, 50, '|'); f.getline(this->OS, 20, '|');
			cout << "ID " << ID << ", Brand " << Brand << ", Processor desc. " << Proc << ", Operating System " << OS << endl;
		}
	}
	void Compact() {
		IN char c; short n; string Total = "";
		while (f) {
			f >> n;
			if (f.eof()) { break; }
			c = f.get(); c = f.get();
			stringstream obj; obj << n; string a; obj >> a;
			if (c == '*' || c == '-') {
				while (c == '*' || c == '-') { c = f.get(); }
				f.seekg(-1, ios::cur); continue; // seekg brdo 34an lazm yasta yrgg3 a5r 7rf da msh zyta hya
			}
			f.seekg(-1, ios::cur);//undo the f.get() char ;
			f.getline(this->ID, 20, '|');//getline bt8yyr al get pointer
			f.getline(this->Brand, 20, '|'); f.getline(this->Proc, 50, '|'); f.getline(this->OS, 20, '|');
			string Idd(ID), Brandd(Brand), Procc(Proc), OSS(OS);
			string all = ' ' + Idd + '|' + Brandd + '|' + Procc + '|' + OSS + '|';
			short length = all.size();
			stringstream obj2; obj2 << length; string len; obj2 >> len;
			Total = Total + len + all;
		}
		f.close();
		fstream f2("hello.txt", ios::out);
		f2.seekp(0);
		f2.write(&Total[0], Total.size()); f2.close();
	}
};

int main()
{
	IN_OUT	char c; int x;
	while (1) {
		cout << "(1) for Add a new notebook\n(2) for Delete a notebook\n(3) for Update a record\n"
			<< "(4) Display a notebook\n(5) Compact the FILE \n(6) Visualize the File\n(7) Exit\nPlease Enter your choice\n";
		cin >> c;
		if (c<'8' && c>'0') {
			stringstream obj; obj << c; obj >> x;
		}
		else continue;
		if (x == 1) { noteBook n1; n1.Add(); }
		else if (x == 2) { noteBook n1; char arr[100]; idd cin >> arr; n1.Delete(arr); }
		else if (x == 3) { noteBook n1; char arr[100]; idd cin >> arr; n1.Update(arr); }
		else if (x == 4) { noteBook n1; n1.Display(); }
		else if (x == 5) { noteBook n1; n1.Compact(); }
		else if (x == 6) { noteBook n1; cout << n1.Visualize(); }
		else if (x == 7)break;
	}
	f.close();
	return 0;
}
