#include "pch.h"
#include "all.h"
#include "Book.h" 
#include "Author.h"
#include "Query.h"

using namespace std;
int main()
{
	Authors a; fstream fa("Author.txt", ios::out); fstream fb("book.txt", ios::out);
	Books b;
	string c; int x;
	while (1) {
		cout << "(1)  Add New Book\n(2)  Add New Author\n(3)  Delete Book (ID)\n"
			<< "(4)  Delete Author (ID)\n(5)  Print Book(ID) \n(6)  Print Book(Author ID)\n(7)  Print Author(ID)\n"
			<< "(8)  Print Author(name)\n(9)  Write a Query\n(10) Exit \n";
		cin >> c;
		if (c[0] <= '10' && c[0] > '0') {
			strstream obj; obj << c; obj >> x;
		}
		else continue;
		if (x == 1) { cin.ignore(); b.AddBook(a); b.SortBooks(); }
		else if (x == 2) { cin.ignore(); a.AddAuthor(); a.SortAuthors(); }
		else if (x == 3) { cin >> c; b.DeleteBookID(&c[0]); }
		else if (x == 4) { cin >> c;  a.DeleteAuthorID(&c[0]); b.DeleteAuthorID(&c[0]); }
		else if (x == 5) { cin >> c; b.PrintBook(&c[0]); }
		else if (x == 6) { cin >> c;  b.PrintAuthorBooks(&c[0]); }
		else if (x == 7) { cin >> c;  a.PrintAuthorID(&c[0]); }
		else if (x == 8) { cin >> c; a.PrintAuthorName(&c[0]); }
		else if (x == 9) {
			Query q; q.execute(a, b);
		}
		else if (x == 10) { a.SaveIdx(); b.SaveIdx(); break; }
	}

	//system("pause");
	return 0;
}

