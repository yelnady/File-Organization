#include "stdafx.h"
#include "all.h"
#include <strstream>
using namespace std;
struct student {
	char ID[9], Name[49], Address[49];
};

struct Tnode {
	char info[9];
	int offset;
	Tnode*right;
	Tnode*left;
	Tnode(char*x,int idx ) {
		//info contains the ID of the user 
		strcpy_s(info, x);right = left = 0;offset = idx;
	}
};
class Tree {
private:
	Tnode * root;
public:
	Tree() {
		root = NULL;
	}
	void insert(Tnode*node, char* studID, int idx) {
		Tnode*current = root; Tnode *prev = root;
		if (root == NULL) {
			root = new Tnode(studID, idx);
		}
		while (current != NULL) {
			if (strcmp(current->info, studID) == 0) {
				break;
			}
			else if (strcmp(root->info, studID) < 0) {
				prev = current;
				current = current->right;
				if (current == NULL) {
					prev->right = new Tnode(studID, idx);
				}
			}
			else {
				prev = current;
				current = current->left;
				if (current == NULL) {
					prev->left = new Tnode(studID, idx);
				}
			}
		}

	}
	void insert(char* studID, int idx) {
		insert(root, studID, idx);
	}
	void printINorder(Tnode*root) {
		if (root != NULL) {
			printINorder(root->left);
			printData(root->info);
			printINorder(root->right);
		}
	}
	void printINorder() {
		printINorder(root);
	}
	void printData(char*c) {
		ifstream f("Students.txt", ios::in);
		int idx = getIndex(c);
		f.seekg(idx, ios::beg);
		int n; f >> n; student Temp;
		f.getline(Temp.ID, 8, '|'); f.getline(Temp.Name, 48, '|'); f.getline(Temp.Address, 48, '|');
		cout << "ID: " << setw(10) << Temp.ID << "\t\tName: " << setw(30) << Temp.Name << "\t\tAddress: " << setw(30) << Temp.Address << endl;
	}
	int  getIndex(char *c) {
		Tnode*current = root;
		while (current != NULL) {
			if (strcmp(current->info, c) == 0) {  return current->offset; }
			else if (strcmp(root->info, c)< 0) {
				current = current->right;
			}
			else {
				current = current->left;
			}
		}
		return -1;
	}
	void Add(const char*id, const char*namee, const char*address) {
		student temp;
		strcpy_s(temp.ID, id);strcpy_s(temp.Name, namee);strcpy_s(temp.Address, address);
		fstream f("Students.txt", ios::app);
		cout << "Enter Your Students Data \n";
		f.seekg(0, ios::end);
		int idx = f.tellg();
		string idd(temp.ID), name(temp.Name), Add(temp.Address);
		string all = ' ' + idd + '|' + name + '|' + Add + '|';
		short length = all.size();
		cout << "Your Entered Student is " << length << all << endl;
		f << length << all;
		insert(temp.ID, idx);
		f.close();
	}
	void Add() {

		student temp;
		fstream f("Students.txt", ios::app);
		cout << "Enter Student's Data \n";
		f.seekg(0, ios::end);
		int idx = f.tellg();
		//cout << "this student will be added and its index is " << idx << endl;;
		cout << "Student ID: ";cin.getline(temp.ID, 8);
		cout << "Student Name: ";cin.getline(temp.Name, 48);
		cout << "Student Address: ";cin.getline(temp.Address, 48);
		string id(temp.ID), name(temp.Name), Add(temp.Address);
		string all = ' ' + id + '|' + name + '|' + Add + '|';
		short length = all.size();
		cout << "Your Entered Student is " << length << all << endl;
		f << length << all;
		insert(temp.ID, idx);
		f.close();
	};
	
	
	void Delete(const char*c ) {

	}
};

int main()
{
	Tree s;
	//cout << "Enter Number of Students " << endl;int n; cin >> n;cin.ignore();
	//for(int i = 0 ;i<n;i++){t.Add();}
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

	s.printINorder();


	system("pause");

}