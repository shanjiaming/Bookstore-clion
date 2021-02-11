//#define debug
//--------------------------------------------------

#include <regex>
#include <cstring>
#include <string>
#include <set>
#include <unordered_map>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <vector>
#include <stack>
#include <cassert>
#include <sstream>
#include <utility>
#include <tuple>
#include <algorithm>
#include <unordered_map>
#include "BPlusTree.h"
using namespace std;

//--------------------------------------------------

enum BookInfoType {
    t_ISBN, t_Book_name, t_Author, t_Keyword
};
const vector<BookInfoType> c_BookInfoTypes{t_ISBN, t_Book_name, t_Author, t_Keyword};
typedef StringType ISBN, Book_name, Author, Keyword;
typedef double Price;
typedef int Quantity;

struct Book {
    ISBN isbn;
    Book_name book_name;
    Author author;
    Keyword keyword;
    Price price = -1;
    Quantity quantity = 0;

    ISBN key() const {return isbn;}

    Book() = default;

    Book(ISBN _isbn) : isbn(_isbn) {}


};


typedef int Time;


typedef StringType User_id, Passwd, User_name;
typedef int Authority;

struct CoreUser {
    User_id user_id;
    Authority authority;
    ISBN selected_book;

    User_id key() const {return user_id;}

    CoreUser() = default;

    CoreUser(const User_id &_user_id, Authority _authority = 0) : user_id(_user_id),
                                                                  authority(
                                                                          _authority) {}

};

struct User : CoreUser {
    Passwd passwd;
    User_name user_name;

    User() = default;

    User(const User_id &_user_id, const Passwd &_passwd, const User_name &_user_name, Authority _authority = 0)
            : CoreUser(_user_id, _authority),
              user_name(
                      _user_name),
              passwd(
                      _passwd) {}
};




//-------------------------------------------------

BPlusTree<User> user_tree("");
BPlusTree<Book> book_tree("");
const map<BookInfoType, FileName> c_bookShowFileMap{{t_ISBN, ""}, {t_Book_name, ""}, {t_Author, ""}, {t_Keyword, ""}};


stack<CoreUser> user_stack;
//stack<ISBN> book_stack;

//-------------------------------------------------
//private Macro

#define ErrorOccur {\
    cout << "Invalid" << endl;\
    return;\
}

#define checkAuthority(x) { \
    if (user_stack.empty() || user_stack.top().authority < x) {\
        ErrorOccur;\
    }\
}

#define FindAndPopSelectedBook  \
    if (user_stack.empty() || user_stack.top().selected_book == "") {\
        ErrorOccur;\
    }                  \
    ISBN& usb = user_stack.top().selected_book; \
    Book tbook = book_tree.find(usb);   \
    book_tree.erase(usb);

//private functions
void addFinance(char, Price);

//-------------------------------------------------

void initialize();

void function_chooser();

namespace user {

    void su(User_id, Passwd);

    void suDirect(User_id);

    void logout();

    void useradd(User_id, Passwd, Authority, User_name);

    void registerAccount(User_id, Passwd, User_name);

    void deleteAccount(User_id);

    void passwd(User_id, Passwd, Passwd);

    void passwdDirect(User_id, Passwd);

}

namespace book {

    void select(ISBN);

    void modify(ISBN, Book_name, Author, Keyword, Price);

    void import(Quantity, Price);

    void show(BookInfoType, string);

    void showFinance();

    void showFinanceTime(Time);

    void buy(ISBN, Quantity);

}

namespace sys {

    void reportFinance();

    void reportEmployee();

    void reportMyself();

    void log();

}


int main() {
#ifdef debug
    freopen("D:\\Programming\\BigHomework\\Bookstore\\Bookstore-2021-main\\Data\\BasicDataSet\\testcase3.txt", "r", stdin);
    freopen("myout.txt", "w", stdout);
#endif
    initialize();
    while (true) {
        function_chooser();
    }
    return 0;
}

void initialize() {
    auto firstOpen = []()->bool{
        //TODO
    };
    auto openMark = [](){
        //TODO
    };
    if (firstOpen()) {
        user_tree.insert(User("root", "sjtu", "root", 7));
        openMark();
    }
    //TODO?
}

void function_chooser() {

    string input;
    smatch parameter;

    //management
    static const string
            user_id = " (\\w{1,30})",
            passwd = " (\\w{1,30})",
            user_name = " ([^ ]{1,30})",
            permission = " ([731])";
    static const regex
            rule_su("su" + user_id + passwd),
            rule_su_direct("su" + user_id),
            rule_logout("logout"),
            rule_useradd("useradd" + user_id + passwd + permission + user_name),
            rule_register("register" + user_id + passwd + user_name),
            rule_delete("delete" + user_id),
            rule_passwd("passwd" + user_id + passwd + passwd),
            rule_passwd_direct("passwd" + user_id + passwd);
    //book
    static const string
            _ISBN = "(.{1,20})", csISBN = " " + _ISBN, e_ISBN = " -ISBN=" + _ISBN,
            _book_name = "(\".{1,60}\")", book_name = " " + _book_name, e_book_name = " -name=" + _book_name,
            _author = _book_name, author = book_name, e_author = e_book_name,
            _keyword = "(\"[^ ]{1,60}\")", keyword = " " + _keyword, e_keyword = " -keyword=" + _keyword,
            _price = "(\\d+(?:\\.\\d+)?)", price = " " + _price, e_price = " -price=" + _price,
            cost_price = price,
            quantity = " (\\d+)",
            time = " (\\d+)";
    static const regex
            rule_select("select" + csISBN),
            rule_modify("modify.*"),
            rule_modify_ISBN("modify.*" + e_ISBN + ".*"),
            rule_modify_name("modify.*" + e_book_name + ".*"),
            rule_modify_author("modify.*" + e_author + ".*"),
            rule_modify_keyword("modify.*" + e_keyword + ".*"),
            rule_modify_price("modify.*" + e_price + ".*"),
            rule_import("import" + quantity + cost_price),
            rule_show("show.*"),
            rule_show_ISBN("show" + e_ISBN),
            rule_show_name("show" + e_book_name),
            rule_show_author("show" + e_author),
            rule_show_keyword("show" + e_keyword),
            rule_show_finance("show finance"),
            rule_show_finance_time("show finance" + time),
            rule_buy("buy" + csISBN + quantity);
    static const regex
            rule_report_finance("report finance"),
            rule_report_employee("report employee"),
            rule_report_myself("report myself"),
            rule_log("log"),
            rule_exit("exit"),
            rule_quit("quit");

    getline(cin, input);

    if (regex_search(input, parameter, rule_su)) {
        user::su(parameter.str(1), parameter.str(2));
        return;
    }
    if (regex_search(input, parameter, rule_su_direct)) {
        user::suDirect(parameter.str(1));
        return;
    }
    if (regex_search(input, parameter, rule_logout)) {
        user::logout();
        return;
    }
    if (regex_search(input, parameter, rule_useradd)) {
        user::useradd(parameter.str(1), parameter.str(2), stoi(parameter.str(3)), parameter.str(4));
        return;
    }
    if (regex_search(input, parameter, rule_register)) {
        user::registerAccount(parameter.str(1), parameter.str(2), parameter.str(3));
        return;
    }
    if (regex_search(input, parameter, rule_delete)) {
        user::deleteAccount(parameter.str(1));
        return;
    }
    if (regex_search(input, parameter, rule_passwd)) {
        user::passwd(parameter.str(1), parameter.str(2), parameter.str(3));
        return;
    }
    if (regex_search(input, parameter, rule_passwd_direct)) {
        user::passwdDirect(parameter.str(1), parameter.str(2));
        return;
    }


    if (regex_search(input, parameter, rule_select)) {
        book::select(parameter.str(1));
        return;
    }
    if (regex_search(input, parameter, rule_modify)) {
        //FIXME: if these value are "" or -1, change won't happen. there include other place like show function
        ISBN l_ISBN;
        Book_name l_bookname;
        Author l_author;
        Keyword l_keyword;
        Price l_price = -1;

        if (regex_search(input, parameter, rule_modify_ISBN)) {
            l_ISBN = parameter.str(1);
        }
        if (regex_search(input, parameter, rule_modify_name)) {
            l_bookname = parameter.str(1);
        }
        if (regex_search(input, parameter, rule_modify_author)) {
            l_author = parameter.str(1);
        }
        if (regex_search(input, parameter, rule_modify_keyword)) {
            l_keyword = parameter.str(1);
        }
        if (regex_search(input, parameter, rule_modify_price)) {
            l_price = stod(parameter.str(1));
        }
        book::modify(l_ISBN, l_bookname, l_author, l_keyword, l_price);
        return;
    }
    if (regex_search(input, parameter, rule_import)) {
        book::import(stoi(parameter.str(1)), stod(parameter.str(2)));
        return;
    }
    if (regex_search(input, parameter, rule_show)) {
        BookInfoType l_infotype;
        string l_info;
        if (regex_search(input, parameter, rule_show_ISBN)) {
            l_infotype = t_ISBN;
            l_info = parameter.str(1);
            return;
        }
        if (regex_search(input, parameter, rule_show_name)) {
            l_infotype = t_Book_name;
            l_info = parameter.str(1);
            return;
        }
        if (regex_search(input, parameter, rule_show_author)) {
            l_infotype = t_Author;
            l_info = parameter.str(1);
            return;
        }
        if (regex_search(input, parameter, rule_show_keyword)) {
            l_infotype = t_Keyword;
            l_info = parameter.str(1);
            return;
        }
        book::show(l_infotype, l_info);
    }
    if (regex_search(input, parameter, rule_show_finance)) {
        book::showFinance();
        return;
    }
    if (regex_search(input, parameter, rule_show_finance_time)) {
        book::showFinanceTime(stoi(parameter.str(1)));
        return;
    }
    if (regex_search(input, parameter, rule_buy)) {
        book::buy(parameter.str(1), stoi(parameter.str(2)));
        return;
    }


    if (regex_search(input, parameter, rule_report_finance)) {
        sys::reportFinance();
        return;
    }
    if (regex_search(input, parameter, rule_report_employee)) {
        sys::reportEmployee();
        return;
    }
    if (regex_search(input, parameter, rule_report_myself)) {
        sys::reportMyself();
        return;
    }
    if (regex_search(input, parameter, rule_log)) {
        sys::log();
        return;
    }
    if (regex_search(input, parameter, rule_exit)) {
        exit(0);
    }
    if (regex_search(input, parameter, rule_quit)) {
        exit(0);
    }


    cout << "Invalid" << endl;
    return;
}


void user::su(User_id _user_id, Passwd _passwd) {
    User l_user;
    try {
        l_user = user_tree.find(_user_id);
    } catch (Not_Found) {
        ErrorOccur
    }
    if (l_user.passwd != _passwd) ErrorOccur
    user_stack.push(l_user);
}

void user::suDirect(User_id _user_id) {
    User l_user;
    try {
        l_user = user_tree.find(_user_id);
    } catch (Not_Found) {
        ErrorOccur
    }
    checkAuthority(l_user.authority * 2 + 1)
    user_stack.push(l_user);
}

void user::logout() {
    if (user_stack.empty()) ErrorOccur
    user_stack.pop();
}

void user::useradd(User_id _user_id, Passwd _passwd, Authority _authority, User_name _user_name) {
    checkAuthority(_authority * 2 + 1)
    try{
        user_tree.find(_user_id);
        ErrorOccur
    }
    catch (Not_Found){
        user_tree.insert(User(_user_id, _passwd, _user_name, _authority));
    }
}

void user::registerAccount(User_id _user_id, Passwd _passwd, User_name _user_name) {
    user_tree.insert(User(_user_id, _passwd, _user_name, 1));
}

void user::deleteAccount(User_id _user_id) {
    checkAuthority(7)
    try{
        user_tree.find(_user_id);
        ErrorOccur
    }
    catch (Not_Found){
        user_tree.erase(_user_id);
    }
}

void user::passwd(User_id _user_id, Passwd _old_passwd, Passwd _new_passwd) {
    checkAuthority(1)
    User l_user;
    try {
        l_user = user_tree.find(_user_id);
        user_tree.erase(_user_id);
    } catch (Not_Found) {
        ErrorOccur
    }
    if (l_user.passwd != _old_passwd) ErrorOccur
    l_user.passwd = _new_passwd;
    user_tree.insert(l_user);
}




void user::passwdDirect(User_id _user_id, Passwd _new_passwd) {
    checkAuthority(7)
    User l_user;
    try {
        User_id l_id = user_stack.top().user_id;
        l_user = user_tree.find(l_id);
        user_tree.erase(l_id);
    } catch (Not_Found) {
        ErrorOccur
    }
    l_user.passwd = _new_passwd;
    user_tree.insert(l_user);
}


void book::select(ISBN _isbn) {
    checkAuthority(3)
    Book l_book;
    try {
        l_book = book_tree.find(_isbn);
    } catch (Not_Found) {
        l_book = Book(_isbn);
        book_tree.insert(l_book);
    }
    user_stack.top().selected_book = _isbn;
}

void book::modify(ISBN _isbn, Book_name _book_name, Author _author, Keyword _keyword, Price _price){
    checkAuthority(3)
    FindAndPopSelectedBook
    if (_isbn != ""){
        usb = _isbn;
        tbook.isbn = _isbn;
    }
    if (_book_name != ""){
        tbook.book_name = _book_name;
    }
    if (_author != ""){
        tbook.author = _author;
    }
    if (_keyword != ""){
        tbook.keyword = _keyword;
    }
    if (_price != -1){
        tbook.price = _price;
    }
    book_tree.insert(tbook);

}

void book::import(Quantity _quantity, Price _price){
    checkAuthority(3)
    FindAndPopSelectedBook
    tbook.quantity += _quantity;
    book_tree.insert(tbook);
    addFinance('-', _price);
}

void book::show(BookInfoType _infotype, string _info){
    checkAuthority(1)

}

void book::showFinance(){
    checkAuthority(7)
}

void book::showFinanceTime(Time _time){
    checkAuthority(7)
}

void book::buy(ISBN _isbn, Quantity _quantity){
    checkAuthority(1)
    FindAndPopSelectedBook
    tbook.quantity -= _quantity;
    book_tree.insert(tbook);
    addFinance('+', _quantity * tbook.price);
}


void sys::reportFinance(){
    checkAuthority(7)
}

void sys::reportEmployee(){
    checkAuthority(7)
}

void sys::reportMyself(){
    checkAuthority(3)
}

void sys::log(){
    checkAuthority(7)
}


//---------------------------------------------

void addFinance(char _c,Price _price){

}



/*
void book::select(ISBN _isbn) {
    checkAuthority(3)
    Book l_book;
    try {
        l_book = book_tree.find(_isbn);
    } catch (Not_Found) {
        l_book = Book(_isbn);
        book_tree.insert(l_book);
    }
    user_stack.top().selected_book = _isbn;
}

void book::modify(ISBN _isbn, Book_name _book_name, Author _author, Keyword _keyword, Price _price){
    checkAuthority(3)
    book_tree.find(_isbn);
}

void book::import(Quantity _quantity, Price _price){
    checkAuthority(3)
}

void book::show(BookInfoType _infotype, string _info){
    checkAuthority(1)
}

void book::showFinance(){
    checkAuthority(7)
}

void book::showFinanceTime(Time _time){
    checkAuthority(7)
}

void book::buy(ISBN _isbn, Quantity _quantity){
    checkAuthority(1)
}


void sys::reportFinance(){
    checkAuthority(7)
}

void sys::reportEmployee(){
    checkAuthority(7)
}

void sys::reportMyself(){
    checkAuthority(7)
}

void sys::log(){
    checkAuthority(3)
}*/
