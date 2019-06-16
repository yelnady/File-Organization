#pragma once

#include "pch.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <strstream>
#include "Book.h"
#include "Author.h"
using namespace std;

struct where {
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

	void execute(Authors &a, Books &b);


};

