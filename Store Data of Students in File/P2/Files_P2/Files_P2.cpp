#include "stdafx.h"
#include "all.h"
#define GOLDEN  (sqrt(5) - 1) / 2
#define m 997
using namespace std;
struct student {
	char ID[9], Name[49], Address[49];
	student() {
		strcpy_s(ID, "//");
	}
};
struct PrimaryDataArea {
	student data[2];
	int start,used;
	PrimaryDataArea() { start = -1; used = 0; }
};
struct OverflowArea {
	student data[2];
	int next,used;
	OverflowArea() { next = -1; used = 0; }
};
class Students {
private :
	PrimaryDataArea p[m];OverflowArea o[m];
	int Hash(const char*id) {
		string a(id);
		stringstream obj; obj << a;int k; obj >> k;
		/*int k = 0; 
		for (auto i : a)k += (int)i;*/
		return (m*((k*GOLDEN)-floor((k*GOLDEN))));

	}
	void WriteToP(int level, int idx, student temp) {//each hash address will have specific 110 * 2 , as we store two buckets 
		fstream f("Primary.txt", ios::out | ios::binary | ios::in);
		f.seekp(level * 110 + 110 * idx, ios::beg); // as we write in the hash 0 at the second index
		string id(temp.ID), name(temp.Name), Add(temp.Address), bookprice;
		string all = ' ' + id + '|' + name + '|' + Add + '|';
		f << all;
		f.close();
	}
	void WriteToO(int level, int idx, student temp) {
		fstream f("Overflow.txt", ios::out | ios::binary | ios::in);
		f.seekp(level * 110 + 110 * idx, ios::beg);
		string id(temp.ID), name(temp.Name), Add(temp.Address), bookprice;
		string all = ' ' + id + '|' + name + '|' + Add + '|';
		f << all;
		f.close();
	}
	int AvailO;
public :
	Students() { AvailO = 0;
	fstream f("Primary.txt", ios::out);f.close();
	fstream f2("overflow.txt", ios::out);f2.close();
	}
	void Add(const char*id, const char*name, const char*address) {
		student temp;
		strcpy_s(temp.ID, id);strcpy_s(temp.Name, name);strcpy_s(temp.Address, address);
		int h = Hash(temp.ID);
		//cout << h << endl;
		if (p[h].used<2) {

			p[h].data[p[h].used] = temp;
			WriteToP(h, p[h].used, temp);
			p[h].used++;

		}
		else {
			int start = p[h].start;
			if (start == -1) { //if primary area is full, Start now is the pointer to Data in Separate overflow .. if it was -1 then assign it one
				start = AvailO; AvailO++;
				o[start].data[0] = temp;
				p[h].start = AvailO; //It isn't -1 again we have a start now 
				WriteToO(start, o[start].used, temp);
				o[start].used++;
			}
			else if (o[start].used<2) {
				o[start].data[o[start].used] = temp;
				WriteToO(start, o[start].used, temp);
				o[start].used++;
			}
			else { // if the bucket also in the separete overflow area is full then we need a new space to insert our data 
				int val;
				while (1) {
					val = o[start].next;
					if (val == -1) { AvailO = o[start].next; o[AvailO].data[0] = temp; }
					else if (o[val].used < 2) { o[val].data[1] = temp; break; } //there is avilable space at next then break we have finished
					else { //also val isn't free
						start = val;
					}
				}
			}

		}

	}
	void Add() {
		student temp;
		cout << "Enter Your Students Data \n";
		cout << "Student ID (MAX 8 Characters): ";cin.getline(temp.ID, 8);
		cout << "Student Name (MAX 48 Characters): ";cin.getline(temp.Name, 48);
		cout << "Student Address (MAX 48 Characters): ";cin.getline(temp.Address, 48);
		int h = Hash(temp.ID);
		//cout << h << endl;
		if (p[h].used<2) {

			p[h].data[p[h].used] = temp;
			WriteToP(h, p[h].used,temp);
			p[h].used++;
		
			} 
		else {
			int start = p[h].start;
			if (start == -1) { //if primary area is full, Start now is the pointer to Data in Separate overflow .. if it was -1 then assign it one
				start = AvailO; AvailO++;
				o[start].data[0] = temp;
				p[h].start = AvailO; //It isn't -1 again we have a start now 
				WriteToO(start, o[start].used, temp);
				o[start].used++;
			}
			else if(o[start].used<2){
				 o[start].data[o[start].used] = temp; 
				 WriteToO(start, o[start].used, temp);
				 o[start].used++;
			}
			else  { // if the bucket also in the separete overflow area is full then we need a new space to insert our data 
				int val;
				while (1) {
					val = o[start].next;
					if (val == -1) { AvailO = o[start].next; o[AvailO].data[0] = temp; }
					else if (o[val].used < 2) { o[val].data[1] = temp; break; } //there is avilable space at next then break we have finished
					else { //also val isn't free
						start = val;
					}
				}
			}

			}
		
	}
	void Print() {
		student Temp;
		{
			cout << "Primary araeaaaaaaa\n";
			fstream f("Primary.txt", ios::binary | ios::in);
			for (int i = 0; i < m; i++) {
			if (p[i].used != 0) {
				if (strcmp(p[i].data[0].ID, "//") != 0) {
					f.seekg(i * 110, ios::beg);
					f.getline(Temp.ID, 8, '|');f.getline(Temp.Name, 48, '|');f.getline(Temp.Address, 48, '|');
					cout << "ID: " << setw(10) << Temp.ID << "\t\tName: " << setw(30) << Temp.Name << "\t\tAddress: " << setw(30) << Temp.Address << endl;
				}
				if (strcmp(p[i].data[1].ID, "//") != 0) {
					f.seekg(i * 110 + 110, ios::beg);
					f.getline(Temp.ID, 8, '|');f.getline(Temp.Name, 48, '|');f.getline(Temp.Address, 48, '|');
					cout << "ID: " <<setw(10)<< Temp.ID << "\t\tName: " <<setw(30)<<Temp.Name << "\t\tAddress: " << setw(30) << Temp.Address << endl;
				}
			}
		}
		}
		{
			fstream f("overflow.txt", ios::binary | ios::in);
			cout << "Overflow areaaaaaaaaaa\n";
			for (int i = 0; i < m; i++) {
			if (o[i].used != 0) {
				if (strcmp(o[i].data[0].ID, "//") != 0) {
					f.seekg(i * 110, ios::beg);
					f.getline(Temp.ID, 8, '|');f.getline(Temp.Name, 48, '|');f.getline(Temp.Address, 48, '|');
					cout << "ID: " << setw(10) << Temp.ID << "\t\tName: " << setw(30) << Temp.Name << "\t\tAddress: " << setw(30) << Temp.Address << endl;
				}
				if (strcmp(o[i].data[1].ID, "//") != 0) {
					f.seekg(i * 110 + 110, ios::beg);
					f.getline(Temp.ID, 8, '|');f.getline(Temp.Name, 48, '|');f.getline(Temp.Address, 48, '|');
					cout << "ID: " << setw(10) << Temp.ID << "\t\tName: " << setw(30) << Temp.Name << "\t\tAddress: " << setw(30) << Temp.Address << endl;
				}
			}
		}
	}
	}
	void Delete(const char*id) {
		int h = Hash(id);
		if (strcmp(p[h].data[0].ID, id) == 0) {
			strcpy_s(p[h].data[0].ID, "//");p[h].used--;
		}
		else if (strcmp(p[h].data[1].ID, id) == 0) {
			strcpy_s(p[h].data[1].ID, "//");p[h].used--;
		}
		else if (p[h].start != -1) {//then it will be at overflow file, we need to search there 
			int val = p[h].start;
			while (1) {
			if (strcmp(o[val].data[0].ID, id) == 0) {
				strcpy_s(o[val].data[0].ID, "//");o[val].used--;break;
			}
			else if (strcmp(o[val].data[1].ID, id) == 0) {
				strcpy_s(o[val].data[1].ID, "//");o[val].used--;break;
			}
			else if (o[val].next == -1)break;
			else {val=o[val].next; }

		}
		}
	}
	
};
int main() {
	Students s;	
	/*
	cout << "Enter Number of Records you want to add\n";
	int n; cin >> n;cin.ignore();
	while (n) { s.Add();n--; }
	cout << "Enter Number of Records you want to Delete\n";
	cin >> n;char *c = new char;
	while (n) { cin >> c;s.Delete(c);n--; }
	*/
	s.Add("23", "Yusuf", "giza");
	s.Add("324", "mido", "giza");
	s.Add("2341", "joe", "giza");
	s.Add("234", "ahmed", "giza");
	s.Add("1", "ali", "giza");
	s.Add("111", "Nermine TEMPLATE", "giza");
	s.Add("3423", "farah", "giza");
	s.Add("23444", "shaymaa", "giza");
	s.Add("499", "menna", "giza");
	s.Add("678", "arwa", "giza");
	s.Add("6977", "assem", "giza");
	s.Add("654", "khaled", "giza");
	s.Add("8", "doaa", "giza");
	s.Add("331", "rodaina", "giza");
	s.Add("5675", "laura", "giza");
	s.Add("456", "lara", "giza");
	s.Add("64", "John", "giza");
	s.Add("4", "Ali", "giza");
	s.Add("870", "Haha", "giza");
	s.Add("453", "Sondos", "giza");

	cout << endl;
	s.Print();
	s.Delete("453");
	s.Delete("5675");
	s.Delete("456");
	s.Delete("23444");
	s.Delete("870");
	cout << endl;
	s.Print();
	system("pause");
}

