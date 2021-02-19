//
// Created by JiamingShan on 2021/2/15.
//

#ifndef CODE_BOOKANDUSER_H
#define CODE_BOOKANDUSER_H

#include "BasicHeader.h"
#include "UnrolledLinkedList.hpp"
#include "Data.h"
#include "logger.h"

class BookData{
public:
    BookData(): data_book("book_data.dat"), book_isbn_table("book_isbn_table.dat"), book_name_table("book_name_table.dat"), book_author_table("book_author_table.dat"), book_keyword_table("book_keyword_table.dat"){}
    vector<Book> showType(BookInfoType _infotype, string _info) {
        for (auto itype : c_BookInfoTypes) {
            if (_infotype == itype) {
                return vAdd2vBook(c_bookShowFileMap.at(itype)->findVector(_info));//TODO debug
            }
        }
    }

    void insert(const Book &b){
        Address a = data_book.insert(b);
        book_isbn_table.insert(b.isbn, a);
        book_name_table.insert(b.book_name, a);
        book_author_table.insert(b.author, a);
        vector<Keyword> keywords = splitKeyword(b.keyword);
        for(Keyword oneword : keywords){
            book_keyword_table.insert(oneword, a);
        }
    }

    void erase(const ISBN &key){
        Address a = book_isbn_table.find(key);
        Book b = data_book.find(a);
        data_book.erase(a);
        book_isbn_table.erase(b.isbn, a);
        book_name_table.erase(b.book_name, a);
        book_author_table.erase(b.author, a);
        vector<Keyword> keywords = splitKeyword(b.keyword);
        for(Keyword oneword : keywords){
            book_keyword_table.erase(oneword, a);
        }
    }

//    void erase(const ISBN &key){
//        Address a = book_isbn_table.find(key);
//        Book b = data_book.find(a);
//        data_book.erase(a);
//        book_isbn_table.erase(b.isbn);
//        book_name_table.erase(b.book_name);
//        book_author_table.erase(b.author);
//        vector<Keyword> keywords = splitKeyword(b.keyword);
//        for(Keyword oneword : keywords){
//            book_keyword_table.erase(oneword);
//        }
//    }


    void change(const ISBN &key, const Book &b){
        //待优化，重合两个地址，可是不知道为什么总会报错
        {
        Address a = book_isbn_table.find(key);
        Book b_erase = data_book.find(a);
        data_book.erase(a);
            book_isbn_table.erase(b_erase.isbn, a);
            book_name_table.erase(b_erase.book_name, a);
            book_author_table.erase(b_erase.author, a);
        vector<Keyword> ekeywords = splitKeyword(b_erase.keyword);
        for(Keyword oneword : ekeywords){
            book_keyword_table.erase(oneword, a);
        }
        }
        {
            Address a = data_book.insert(b);
            book_isbn_table.insert(b.isbn, a);
            book_name_table.insert(b.book_name, a);
            book_author_table.insert(b.author, a);
            vector<Keyword> keywords = splitKeyword(b.keyword);
            for(Keyword oneword : keywords){
                book_keyword_table.insert(oneword, a);
            }
        }
    }

    Book find(const ISBN &key){
        return data_book.find(book_isbn_table.find(key));
    }

    vector<Book> findAll(){//TODO 这个不知道在那个位置实现。不过先不管了，总有办法的。
        return vAdd2vBook(book_isbn_table.findAll());
    }


private:
    Data<Book> data_book;
    UnrolledLinkedList book_isbn_table;
    UnrolledLinkedList  book_name_table;
    UnrolledLinkedList book_author_table;
    UnrolledLinkedList  book_keyword_table;
    const map<BookInfoType, UnrolledLinkedList*> c_bookShowFileMap{{t_ISBN,      &book_isbn_table},
                                                                                        {t_Book_name, &book_name_table},
                                                                                        {t_Author,    &book_author_table},
                                                                                        {t_Keyword,   &book_keyword_table}};
    vector<Keyword> splitKeyword(const Keyword& whole){
        vector<Keyword> ret;
        stringstream ss(whole);
        string oneword;
        while (getline(ss, oneword, '|')){
            ret.push_back(oneword);
        }
        return ret;
    }
    vector<Book> vAdd2vBook(vector<Address> v_address){
        vector<Book> v_book;
        for (Address a : v_address){
            v_book.push_back(data_book.find(a));
        }
        return v_book;
    }
};

class UserData{
public:
    UserData(): data_user("user_data.dat"), user_id_table("user_id_table.dat"){}
    void insert(const User &u){
        Address a = data_user.insert(u);
        user_id_table.insert(u.user_id, a);
    }

    void erase(const User_id &key){
        data_user.erase(user_id_table.find(key));
        user_id_table.erase(key, user_id_table.find(key));
    }

    User find(const User_id &key){
        return data_user.find(user_id_table.find(key));
    }

    vector<User> findAll(){
        return vAdd2vUser(user_id_table.findAll());
    }

private:
    Data<User> data_user;
    UnrolledLinkedList user_id_table;
    vector<User> vAdd2vUser(vector<Address> v_address){
        vector<User> v_user;
        for (Address a : v_address){
            v_user.push_back(data_user.find(a));
        }
        return v_user;
    }



};


#endif //CODE_BOOKANDUSER_H
