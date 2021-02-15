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
typedef double Price;
typedef int Quantity;

struct Book {
    ISBN isbn;
    Book_name book_name;
    Author author;
    Keyword keyword;
    Price price = -1;
    Quantity quantity = 0;

    ISBN key() const { return isbn; }

    Book() = default;

    Book(ISBN _isbn) : isbn(_isbn) {}

    bool operator<(const Book &rhs) const {
        return isbn < rhs.isbn;
    }

};


typedef StringType User_id, Passwd, User_name;
typedef int Authority;

struct CoreUser {
    User_id user_id;
    Authority authority;
    ISBN selected_book;

    User_id key() const { return user_id; }

    CoreUser() = default;

    CoreUser(const User_id &_user_id, Authority _authority = 0) : user_id(_user_id),
                                                                  authority(
                                                                          _authority) {}

    bool operator<(const CoreUser &rhs) const {
        if (authority < rhs.authority)
            return true;
        if (rhs.authority < authority)
            return false;
        return user_id < rhs.user_id;
    }
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

typedef char Sign;
struct Finance{
    Finance(Sign sign, Price price) : sign(sign), price(price) {}
    Sign sign;
    Price price;

};

typedef int Time;

#endif //CODE_BASICHEADER_H
