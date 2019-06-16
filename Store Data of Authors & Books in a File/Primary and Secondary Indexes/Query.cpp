#include "pch.h"
#include "Query.h"
#include "all.h"
void format(string &a) {
	int i, idx;
	for (i = a.size() - 1; i >= 0; i--) {
		if (a[i] == ' ') continue;
		else { idx = i; break; }
	}
	string b;
	for (int i = 0; i < idx; i++) {
		b = b + a[i];
	}
	a = b;
}
where Query::analyze_where_condition(string cond) {
	where temp;
	int idx;
	stringstream obj; obj << cond;
	for (int i = 0; i < cond.size(); i++) {
		if (cond[i] == '=') {
			idx = i;
			break;
		}
	}
	temp.consdition = cond.substr(0, idx);
	temp.value = cond.substr(idx + 1);
	format(temp.value);
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
				int n;
				int x = b.GetBookIDOffset(&r.value[0], n);
				if (x == -1) { cout << "Book isn't found1\n"; }
				else {
					fstream f("book.txt", ios::in);
					f.seekg(x, ios::beg); int n; char c;
					f >> n;
					f.get(c); if (c == '*') {
						cout << "Book isn't found2\n"; ;
					}

					book temp;
					f.getline(temp.Book_ID, 12, '|'); f.getline(temp.Author_ID, 29, '|'); f.getline(temp.Book_Title, 49, '|'); f >> temp.Book_Price;
					cout << "Book ID: " << temp.Book_ID << " /Book Author: " << temp.Author_ID << " /Book Title: " << temp.Book_Title
						<< " /Book Price: " << temp.Book_Price << endl;
				}
			}
			else if (r.consdition == "author_id") {
				// print all data of the books file with condition r.consdition = r.value [author_id (secondary key ) ==> r.value]
				int idx = b.AuthorgetAddress(&r.value[0]);
				if (idx != -1) {
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
				cout << setw(5) << "Author_ID" << endl;
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
					cout << setw(5) << temp.Book_Price << endl;

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
				while (!f.eof()) {
					f >> n;
					f.get(c);
					author temp;
					f.getline(temp.Author_ID, 29, '|'); f.getline(temp.Author_Name, 49, '|'); f.getline(temp.Author_Address, 49, '|'); f.getline(temp.Author_Mobile, 10, '|');
					if (c == '*')
						continue;
					cout << setw(5) << temp.Author_ID << setw(30) << temp.Author_Name << setw(25) << temp.Author_Address << setw(25) << temp.Author_Mobile << endl;
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
				a.PrintAuthorName(&r.value[0]);
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
				cout << "Author_ID" << endl;
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
				a.PrintAuthorName(&r.value[0]);

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
						cout << setw(5) << temp.Author_Name << endl;
					}

				}
			}
			else if (r.consdition == "author_name") {
				a.PrintAuthorName(&r.value[0]);

			}
		}
		else if (selected == "author_address") {
			if (query == "0") {
				// print Author_Address attribute of the authors file
				cout << "Author_Address" << endl;
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
						cout << setw(5) << temp.Author_Address << endl;
					}

				}
			}
			else if (r.consdition == "author_name") {
				a.PrintAuthorName(&r.value[0]);

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
				a.PrintAuthorName(&r.value[0]);

			}
		}
	}	if ((query.substr(0, 6) == "select") && ((query.find("fromauthorsandbooks", 6) < query.size()) || (query.find("frombooksandauthors", 6) < query.size()))) {
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
				cout << setw(5) << "Author_ID" << setw(20) << "Author_Name" << setw(20) << "Author_Address" << setw(20) << "Author_Mobile" << setw(20) << "Book_ID" << setw(20) << "Book_Title" << setw(20) << "Book_Price" << endl;
				fstream f_A("Author.txt", ios::in); fstream f_B("book.txt", ios::in);
				f_A.seekg(0, ios::beg); f_B.seekg(0, ios::beg); int n; char c;
				do {
					f_A >> n;
					if (f_A.fail())break;
					f_A.get(c);
					if (f_A.fail())break;
					author temp;
					f_A.getline(temp.Author_ID, 29, '|'); f_A.getline(temp.Author_Name, 49, '|'); f_A.getline(temp.Author_Address, 49, '|'); f_A.getline(temp.Author_Mobile, 10, '|');
					if (c == '*')
						continue;
					f_B.seekg(0, ios::beg);
					while (f_B)
					{

						f_B >> n;
						f_B.get(c);
						book temp_B;
						f_B.getline(temp_B.Book_ID, 12, '|'); f_B.getline(temp_B.Author_ID, 29, '|'); f_B.getline(temp_B.Book_Title, 49, '|'); f_B.getline(temp_B.Book_Price, 10, '|');
						if (c == '*') {
							strcmp(temp_B.Book_ID, "NULL");
							strcmp(temp_B.Author_ID, "NULL");
							strcmp(temp_B.Book_Title, "NULL");
							strcmp(temp_B.Book_Price, "NULL");
						}
						if (strcmp(temp.Author_ID, temp_B.Author_ID) == 0) {
							cout << setw(5) << temp.Author_ID << setw(20) << temp.Author_Name << setw(20) << temp.Author_Address << setw(20) << temp.Author_Mobile << setw(20) << temp_B.Book_ID << setw(20) << temp_B.Book_Title << setw(20) << temp_B.Book_Price << endl;
						}

					}
				} while (true);


			}

		}

	}


	// it should creat a new file called "join" to join the current data of both books and authers where (author_id = author_id) in the mode of trunc
	// the attributes of join 
	// Book_ID (primary key) , Author_ID (sec. key )  , Author_Name (sec. key ) , Book _Title , Book_Price  , Author_Address , Author_Mobile
}