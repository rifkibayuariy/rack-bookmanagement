/*
* Rack Application
* Author: Rifki Bayu Ariyanto and team
* 
*/

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <iomanip>
#include <conio.h>
#include <unordered_map>
#include <limits>
#include <functional>
#include <sstream>

using namespace std;

// >>> STRUCTS DECLARATION <<<

/*
* User Struct
* 
*/
struct User {
	string fullname;
	string username;
	string password;
	int role;
};

/*
* Menu Option Struct
* 
*/
struct MenuOption {
	string menu_name;
	vector<int> access_role;
	
};

/*
* Book Struct
* 
*/
struct Book {
	string category;
	string code;
	string title;
	string author;
	string publisher;
	int release_year;
	int quantity;
	int price;
};

/*
* Columns Table Struct
* 
*/
struct ColumnTable {
	string field_name;
	string align;
};

/*
* Input Field Struct
* 
*/
struct Field {
	string name;
	string key;
	string error_description;
};

// ----------------------------------------------------------------------------------------------------


// >>> GLOBAL VARIABLES DECLARATION <<<

/*
* Login 'Session'
* 
*/
bool login = false;

/*
* Role of User
* 
* untuk menciptakan batas-batasan dalam menggunakan aplikasi ini berdasarkan role user yang digunakan untuk login
* 
*/
map <int, string> role_of_user = {
	{1, "Admin"},
	{2, "Viewer"}
};

/*
* List of User
* 
*/
vector <User> list_of_user = {
	{"Admin","admin","12345", 1},
	{"Stranger", "view", "12345", 2}
};

/*
* User Information
* 
*/
User user_information;

/*
* List of Menu Options
* 
*/
map<int, MenuOption> list_of_menu = {
	{1, {"Add Book", {1}}},
	{2, {"Find Book", {1,2}}},
	{3, {"Update Book", {1}}},
	{4, {"Delete Book", {1}}},
	// {5, {"User", {1}}},
	{99, {"Logout", {1,2}}}
};

/*
* List Of Boook
* 
*/
vector <Book> list_of_books = {
	{"Fiksi","0001","Habis Gelap Terbitlah Terang","Rifki Bayu Ariyanto","Balai Pustaka",2005,1,150000}
};

/*
* List of Table Columns
* 
*/
unordered_map<string, ColumnTable> list_of_tableColumns = {
	{"No",{"-","right"}},
	{"Category",{"category","left"}},
	{"Code",{"code","left"}},
	{"Title",{"title","left"}},
	{"Author",{"author","left"}},
	{"Publisher",{"publisher","left"}},
	{"Release",{"release_year","left"}},
	{"Qty",{"quantity","right"}},
	{"Price (IDR)",{"price","right"}}
};

/*
* List of Input Field
* 
* ascending order
* 
*/
map<int, Field> list_of_fields = {
	{0,{"Title", "title", "Please input the title!"}},
	{1,{"Category", "category", "Please input the category!"}},
	{2,{"Code", "code", "Please input the book code!"}},
	{3,{"Author", "author", "Please input the author!"}},
	{4,{"Publisher", "publisher", "Please input the author!"}},
	{5,{"Release Year", "release_year", "Please input the author!"}},
	{6,{"Quantity", "quantity", "Please input the author!"}},
	{7,{"Price(IDR)", "price", "Please input the author!"}}
};

// ----------------------------------------------------------------------------------------------------


/*
* Functions Declaration
* 
*/
void showHeaderBox(string title = "Rack", char align = 'l', int length = 30);
void authentication();
int showAndSelectMenuOptions();
bool verify(string text);
string getFieldListOfBookByName(const Book& obj, const string& field_name);
void showTableListOfBook(vector <Book> books = list_of_books);
void showBookInformation(Book book, Book oldBook = {});
Book inputBook(Book oldBook = {});
void addBook();
vector<Book> filterListOfBooks(string target, string keyword);
void findBookByParameter();
void updateBook();
void deleteBook();


/*
* Index or Main Function
* 
*	that is called first
* 
*/
int main() {

	cout << "Please fullscreen the window!" << endl;

	while (true) {

		system("cls");

		// checking user authentication
		if (!login) authentication();

		cout << " " << char(254) << " Rack | Book Management" << endl;

		// show Table List of Book
		showTableListOfBook();

		// Select Menu
		int menuSelected = showAndSelectMenuOptions();

		// case disini berdasarkan nilai 'List of Menu Options' bukan berdasarkan angka yang diinput pada saat pilih menu
		switch (menuSelected) {
			case 1:
				addBook();
				break;
			case 2:
				if (list_of_books.size()) findBookByParameter();
				else {
					cout << " > Empty Book List!" << endl;
					system("pause");
				}
				break;
			case 3:
				if (list_of_books.size()) updateBook();
				else {
					cout << " > Empty Book List!" << endl;
					system("pause");
				}
				break;
			case 4:
				if (list_of_books.size()) deleteBook();
				else {
					cout << " > Empty Book List!" << endl;
					system("pause");
				}
				break;
			case 5:
				cout << "Hehe..., masih progress :)" << endl;
				system("pause");
				break;
			case 99:
				if (verify("Logout")) {
					login = false;
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
				}
				break;
			case 0:
				cout << "Good Bye, " << user_information.fullname << "!" << endl;
				return 0;
			default:
				break;
		}

	}

	return 0;
}

/*
* String to Lowercase
* 
* 
*/
string toLowerCase(string input) {
	for (char& c : input) c = tolower(c);
	return input;
}

/*
* Header Box Title
* 
*/
void showHeaderBox(string title, char align, int length) {

	string horizontal_line;
	for (int i = 0; i < (title.length() + 2 > length ? title.length() + 2 : length); i++) horizontal_line += char(196);

	cout << char(218) << horizontal_line << char(191) << endl;
	cout << char(179) << " ";
	cout << setw(title.length() + 2 > length ? title.length() : length - 2) << (tolower(align) == 'r' ? right : left) << title;
	cout << " " << char(179) << endl;
	cout << char(192) << horizontal_line << char(217) << endl;
}

/*
* Authentication
* 
* checking user authentication, sign in or login proccess to access application
* 
*/
void authentication() {

	string username;
	char password[100];

	bool user_found = false;

	// Show Box Header Sign in or Login
	string box_header_title(1, static_cast<char>(254));
	showHeaderBox(box_header_title += " Rack | Login");

	while (true) {

		if (!user_found) cout << " > username: ", getline(cin, username);
		cout << " > password: ";

		// replace password char into '*"
		char char_input;
		int i = 0;
		while (true) {

			char_input = _getch();
			if (char_input == 13) {
				password[i] = '\0';
				break;
			} else if (char_input == '\b') {
				if (i >= 1) {
					cout << "\b \b";
					i--;
				}
			} else {
				password[i] = char_input;
				cout << "*";
				i++;
			}

		}
		cout << endl;

		auto find_user = find_if(list_of_user.begin(), list_of_user.end(), [username](const User& user) {
			return user.username == username;
		});

		if (find_user != list_of_user.end()) {
			user_found = true;

			if (find_user->password == password) {
				login = true;
				user_information = {
					find_user->fullname,
					find_user->username,
					find_user->password,
					find_user->role
				};
				break;
			} else {
				cout << "password don't match" << endl;
			}
		} else {
			user_found = false;
			cout << "username not found" << endl;
		}
	}

	system("cls");
}

/*
* Show Menu Options and also Select the Option
* 
* menu yang ditampilkan berdasarkan hak akses role dari user yang digunakan untuk login
* 
*/
int showAndSelectMenuOptions() {

	vector<int> list_available_menu = { 0 };
	int option_selected;

	// display menu options
	cout << "Actions:" << endl;

	int key = 1;
	for (auto& menu : list_of_menu) {
		for (auto& role : menu.second.access_role) {
			if (role == user_information.role) {
				list_available_menu.push_back(menu.first);
				cout << key << ") " << menu.second.menu_name << " | ";
				key++;
				break;
			}
		}
	}
	cout << "0) Exit" << endl;

	cout << endl;

	while (true) {
		cout << "Choose action: ", cin >> option_selected;

		if (cin.fail()) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}

		for (auto& menu : list_of_menu) {
			if (option_selected < list_available_menu.size()) {
				return list_available_menu[option_selected];
				break;
			}
		}

		cout << "Choose an available action!" << endl;

	}

	system("pause");
}

/*
* Verify
* 
* verification before continue to main proccess (yes or no question)
* 
*/
bool verify(string text) {
	char verif;
	while (true) {
		cout << text << " ( Y / N ): ";

		if (cin >> verif) {
			if (tolower(verif) == 'y' || tolower(verif) == 'n') {
				break;
			}
		}

		cout << "Please input the correct answer Yes or No ( Y / N )!" << endl;
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}

	return tolower(verif) == 'y' ? true : false;
}

/*
* Get Field List of Book by Name
* 
*/
string getFieldListOfBookByName(const Book& obj, const string& field_name) {
	static const unordered_map<string, function<string(const Book&)>> field_map = {
		{"category", [](const Book& book) { return book.category; }},
		{"code", [](const Book& book) { return book.code; }},
		{"title", [](const Book& book) { return book.title; }},
		{"author", [](const Book& book) { return book.author; }},
		{"publisher", [](const Book& book) { return book.publisher; }},
		{"release_year", [](const Book& book) { return to_string(book.release_year); }},
		{"quantity", [](const Book& book) { return to_string(book.quantity); }},
		{"price", [](const Book& book) { return to_string(book.price); }},
	};

	auto it = field_map.find(field_name);
	if (it != field_map.end())
		return it->second(obj);
	else
		throw invalid_argument("Field name not found");
}

/*
* Show Table List of Book
* 
* 
*/
void showTableListOfBook(vector <Book> books) {

	vector<int> column_width;
	int columns_width = 0;
	int col = 0;

	string top_line(1, static_cast<char>(218));
	string middle_line(1, static_cast<char>(195));
	string middle_line_empty(1, static_cast<char>(195));
	string bottom_line(1, static_cast<char>(192));
	string bottom_line_empty(1, static_cast<char>(192));

	for (auto& column : list_of_tableColumns) {
		int column_length = column.first.length() + 2;

		column_width.push_back(column_length);
		columns_width += column_length;
		columns_width++;
	}

	int columns_data_width = column_width[0] + 1;
	for (auto& row : books) {
		col = 0;
		for (auto& column : list_of_tableColumns) {
			if (col != 0) {
				string field_value = getFieldListOfBookByName(row, column.second.field_name);
				int value_length = static_cast<int>(field_value.length()) + 2;
				column_width[col] = max(column_width[col], value_length);
				columns_data_width += value_length;
				columns_data_width++;
			}
			col++;
		}
	}

	col = 0;
	for (auto& cw : column_width) {
		for (int i = 0; i < cw; i++) {
			top_line += char(196);
			bottom_line += char(196);
			middle_line_empty += char(196);
			middle_line += char(196);
			bottom_line_empty += char(196);
		}

		col++;

		if (col != list_of_tableColumns.size()) {
			top_line += char(194);
			middle_line += char(197);
			middle_line_empty += char(193);
			bottom_line += char(193);
			bottom_line_empty += char(196);
		}
		else if (col == list_of_tableColumns.size()) {
			top_line += char(191);
			middle_line += char(180);
			middle_line_empty += char(180);
			bottom_line += char(217);
			bottom_line_empty += char(217);
		}
	}

	columns_width = max(columns_width, columns_data_width);

	// Display Table Header
	cout << top_line << endl;

	col = 0;
	for (auto& column : list_of_tableColumns) {
		cout << char(179);
		cout << setw(column_width[col]) << left << " " + column.first;

		col++;

		if (col == list_of_tableColumns.size()) cout << char(179);
	}
	cout << endl;

	// Display Table Body
	if (books.size()) {
		cout << middle_line << endl;
		int number = 1;
		for (auto& row : books) {
			col = 0;
			cout << char(179);
			for (auto& column : list_of_tableColumns) {
				cout << setw(column_width[col]) << (column.second.align == "right" ? right : left) << (column.second.align == "right" ? "" : " ") + (col == 0 ? to_string(number) : getFieldListOfBookByName(row, column.second.field_name)) + (column.second.align == "right" ? " " : "");
				col++;
				cout << char(179);
			}
			number++;
			cout << endl;
		}
		cout << bottom_line << endl;
	}
	else {
		cout << middle_line_empty << endl;
		string status = "empty";

		cout << char(179) << setw(columns_width / 2) << right << status << setw(columns_width / 2) << right << char(179) << endl;
		cout << bottom_line_empty << endl;
	}

	cout << endl;

}

/*
* Show Book Information
* 
*/
void showBookInformation(Book book, Book oldBook) {
	int max_width_old_book = 0;
	bool compare = oldBook.title.empty() ? false : true;

	if (compare) {
		for (auto field : list_of_fields) {
			max_width_old_book = max(max_width_old_book, static_cast<int>(getFieldListOfBookByName(oldBook, field.second.key).length()));
		}
	}

	for (auto field : list_of_fields) {
		cout << " > " << setw(15) << field.second.name << ": ";
		if (compare) {
			cout << setw(max_width_old_book) << getFieldListOfBookByName(oldBook, field.second.key);
			cout << "  -> ";
		}
		cout << getFieldListOfBookByName(book, field.second.key);

		cout << endl;
	}
}

/*
* Input Book and return it
* 
*/
Book inputBook(Book oldBook) {
	bool edit = oldBook.title.empty() ? false : true;

	Book input;
	if (edit) input = oldBook;

	if (edit) cout << endl << "( empty the input if using the old value )" << endl << endl;

	for (auto& field : list_of_fields) {
		while (true) {
			string input_temp = "";

			cout << " > " << field.second.name << ": ", getline(cin, input_temp);

			if (edit && input_temp.empty()) break;
			else if (!input_temp.empty()) {
				bool verif = true;

				if (field.second.key == "title") input.title = input_temp;
				else if (field.second.key == "category") input.category = input_temp;
				else if (field.second.key == "code") {
					if (filterListOfBooks("code", input_temp).size()) {
						cout << "Book Code: " << input_temp << " already used!" << endl;
						verif = false;
					} else {
						input.code = input_temp;
					}
				}
				else if (field.second.key == "author") input.author = input_temp;
				else if (field.second.key == "publisher") input.publisher = input_temp;
				else if (field.second.key == "release_year"  || field.second.key == "quantity" || field.second.key == "price") {
					istringstream iss(input_temp);
					int number;

					if (iss >> number) {
						if (iss.eof()) {
							if (field.second.key == "release_year") input.release_year = number;
							else if (field.second.key == "quantity") input.quantity = number;
							else if (field.second.key == "price") input.price = number;
						} else {
							cout << "Invalid input. Extra characters after the integer." << endl;
							verif = false;
						}
					} else {
						cout << "Please input integer!" << endl;
						verif = false;
					}
				}
				
				if (verif) break;
			} else cout << field.second.error_description << endl;
		}
	}

	return input;
}

/*
* Add Book to container List of Book
* 
*/
void addBook() {

	system("cls");

	showHeaderBox("=> Add Book");

	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	Book input = inputBook();

	cout << endl;

	if (verify("Add The Book")) {
		system("cls");
		list_of_books.push_back(input);

		showHeaderBox("The Book saved!");
		showBookInformation(input);
		cout << endl;
	} else {
		cout << "The Book not saved!" << endl;
	}

	system("pause");
}

/*
* Filter List of Book by parameter
* 
*/
vector<Book> filterListOfBooks(string target, string keyword) {
	vector<Book> book_filtered;
	if (list_of_books.size()) {
		for (auto book : list_of_books) {
			string value = getFieldListOfBookByName(book, target);
			if ((target == "code" && keyword == value) || (target != "code" && toLowerCase(value).find(toLowerCase(keyword)) != string::npos)) {
				book_filtered.push_back(book);
			}
		}
	}

	return book_filtered;
}

/*
* Find Book by Parameter Book Code
* 
*/
void findBookByParameter() {

	system("cls");

	map<int, Field> parameters = {
		{1, {"Book Code","code",""}},
		{2, {"Book Title","title",""}},
		{3, {"Author","author",""}},
		{4, {"Publisher","publisher",""}},
	};

	showHeaderBox("Find Book");

	cout << " Parameter:" << endl;
	for (auto p : parameters) {
		cout << "  " << p.first << ") " + p.second.name << endl;
	}
	cout << "  0) Back" << endl << endl;

	int parameter;
	while (true) {
		cout << "Choose parameter: ", cin >> parameter;
		if (parameter == 0) break;

		bool found = false;
		for (auto p : parameters) {
			if (p.first == parameter) {
				found = true;
				break;
			}
		}

		if (found) break;

		cout << "Choose an available parameter" << endl;
	}

	if (parameter != 0) {
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		string keyword;
		while (true) {
			cout << " > " << parameters[parameter].name << ": ";
			getline(cin, keyword);

			if (keyword.empty()) {
				cout << "Please input keyword for " << parameters[parameter].name << endl;
			} else break;
		}

		vector<Book> filtered_book = filterListOfBooks(parameters[parameter].key, keyword);

		if (filtered_book.size()) {
			system("cls");

			cout << " Parameter: " << parameters[parameter].name << endl;
			cout << " Keyword: " << keyword << endl;
			cout << " Number of Books: " << filtered_book.size() << endl << endl;

			int key = 1;
			for (Book book : filtered_book) {
				showHeaderBox(to_string(key) + ". " + book.title, 'l', 60);
				showBookInformation(book);
				cout << endl;
				key++;
			}

		} else {
			cout << "Book not found!" << endl;
		}

		system("pause");
	}
}

/*
* Update Book
* 
* parameter: Book Code
* 
*/
void updateBook() {
	string book_code;
	cout << " > Book Code: ", cin >> book_code;

	vector<Book> filtered_book = filterListOfBooks("code", book_code);

	if (filtered_book.size()) {
		system("cls");

		Book target_book = filtered_book[0];

		showHeaderBox("Update Book | Book Code: " + target_book.code);
		showBookInformation(target_book);

		cout << endl << " Update Book" << endl;

		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		Book updated_book = inputBook(target_book);

		cout << endl;

		if (verify("Update The Book")) {
			for (auto& book : list_of_books) {
				if (book.code == book_code) {
					book = updated_book;
					break;
				}
			}

			system("cls");
			showHeaderBox("The Book Updated!");
			showBookInformation(updated_book, target_book);

		} else {
			cout << "The Book not updated!" << endl;
		}

		cout << endl, system("pause");

	}
	else {
		cout << "Book not found!" << endl;
		system("pause");
	}

}

/*
* Delete Book
* 
* parameter: Book Code
* 
*/
void deleteBook() {
	string book_code;
	cout << " > Book Code: ", cin >> book_code;

	vector<Book> filtered_book = filterListOfBooks("code", book_code);

	if (filtered_book.size()) {
		system("cls");

		Book target_book = filtered_book[0];

		showHeaderBox("Update Book | Book Code: " + target_book.code);
		showBookInformation(target_book);

		cout << endl;

		if (verify("Delete The Book")) {

			list_of_books.erase(remove_if(list_of_books.begin(), list_of_books.end(),
				[book_code](const Book& book) {
					return book.code == book_code;
				}),
				list_of_books.end());

			cout << "The Book (code: " << book_code << ") deleted!" << endl;
		}
		else {
			cout << "The Book not deleted!" << endl;
		}

		cout << endl, system("pause");

	}
	else {
		cout << "Book not found!" << endl;
		system("pause");
	}
}

// Thank you, I'll say goodbye soon
