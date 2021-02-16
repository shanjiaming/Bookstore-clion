//
// Created by JiamingShan on 2021/2/14.
//

#ifndef CODE_BASICHEADER_H
#define CODE_BASICHEADER_H


#include <regex>
#include <cstring>
#include <string>
#include <set>
#include <unordered_map>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <vector>
#include <stack>
#include <cassert>
#include <sstream>
#include <utility>
#include <tuple>
#include <algorithm>
#include <unordered_map>

using namespace std;



enum BookInfoType {
    t_ISBN, t_Book_name, t_Author, t_Keyword
};
const vector<BookInfoType> c_BookInfoTypes{t_ISBN, t_Book_name, t_Author, t_Keyword};

typedef string StringType;
typedef StringType ISBN, Book_name, Author, Keyword;
typedef char cISBN[21], cBook_name[61], cAuthor[61], cKeyword[61];
typedef double Price;
typedef int Quantity;

struct Book {
    cISBN isbn;
    cBook_name book_name;
    cAuthor author;
    cKeyword keyword;
    Price price;
    Quantity quantity;

    Book() = default;

//    Book(cISBN _isbn) { strcpy(isbn, _isbn); price = 0; quantity = 0; strcpy(book_name,""); strcpy(author,""); strcpy(keyword,"");}
    Book(ISBN _isbn, cBook_name _book_name = "",//这儿初始化用什么比较好？
         cAuthor _author = "",
         cKeyword _keyword = "",
         Price _price = 0,
         Quantity _quantity = 0) {
        strcpy(isbn, _isbn.c_str());
        strcpy(book_name, _book_name);
        strcpy(author, _author);
        strcpy(keyword, _keyword);
        price = _price;
        quantity = _quantity;
    }

    bool operator<(const Book &rhs) const {
        return strcmp(isbn, rhs.isbn) < 0;
    }

};


typedef char cUser_id[31], cPasswd[31], cUser_name[31];
typedef StringType User_id, Passwd, User_name;
typedef int Authority;

struct CoreUser {
    cUser_id user_id;
    Authority authority;
    cISBN selected_book;

    CoreUser() = default;

//    CoreUser(const cUser_id &_user_id, Authority _authority = 0) : authority(_authority) { strcpy(user_id, _user_id); strcpy(selected_book,"");}
    CoreUser(const User_id &_user_id, Authority _authority = 0, ISBN _selected_book = "") : authority(_authority) {
        strcpy(user_id, _user_id.c_str());
        strcpy(selected_book, _selected_book.c_str());
    }

    bool operator<(const CoreUser &rhs) const {
        if (authority < rhs.authority)
            return true;
        if (rhs.authority < authority)
            return false;
        return strcmp(user_id, rhs.user_id) < 0;
    }
};

struct User : CoreUser {
    cPasswd passwd;
    cUser_name user_name;

    User() = default;

//    User(const cUser_id &_user_id, const cPasswd &_passwd, const cUser_name &_user_name, Authority _authority = 0)
//            : CoreUser(_user_id, _authority) {
//        strcpy(user_name, _user_name);
//        strcpy(passwd, _passwd);
//    }
    User(const User_id &_user_id, const Passwd &_passwd, const User_name &_user_name, Authority _authority = 0)
            : CoreUser(_user_id, _authority) {
        strcpy(user_name, _user_name.c_str());
        strcpy(passwd, _passwd.c_str());
    }
};

typedef char Sign;

struct Finance {
    Finance(Sign sign, Price price) : sign(sign), price(price) {}

    Sign sign;
    Price price;

};

typedef int Time;


#endif //CODE_BASICHEADER_H
