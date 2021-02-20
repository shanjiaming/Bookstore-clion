//
// Created by JiamingShan on 2021/2/15.
//

#ifndef CODE_BOOKANDUSER_H
#define CODE_BOOKANDUSER_H

#include "BasicHeader.h"
#include "UnrolledLinkedList.hpp"
#include "Data.h"

class BookData {

public:

    BookData();

    vector<Book> showType(BookInfoType _infotype, string _info);

    void insert(const Book &b);

    void erase(const ISBN &key);

    void change(const ISBN &key, const Book &b);

    Book find(const ISBN &key);

    vector<Book> findAll();


private:

    Data<Book> data_book;

    UnrolledLinkedList book_isbn_table;

    UnrolledLinkedList book_name_table;

    UnrolledLinkedList book_author_table;

    UnrolledLinkedList book_keyword_table;

    vector<Keyword> splitKeyword(const Keyword &whole);

    vector<Book> vAdd2vBook(vector<Address> v_address);
};

class UserData {

public:

    UserData();

    void insert(const User &u);

    void erase(const User_id &key);

    User find(const User_id &key);

    vector<User> findAll();

private:

    Data<User> data_user;

    UnrolledLinkedList user_id_table;

    vector<User> vAdd2vUser(vector<Address> v_address);


};


#endif //CODE_BOOKANDUSER_H
