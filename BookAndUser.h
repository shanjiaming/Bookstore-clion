//
// Created by JiamingShan on 2021/2/15.
//

#ifndef CODE_BOOKANDUSER_H
#define CODE_BOOKANDUSER_H

#include "BasicHeader.h"
#include "BPlusTree.hpp"
#include "data.h"

class BookData{
public:
    BookData(): data_book("book_data.dat"), book_isbn_tree("book_isbn_tree.dat"), book_name_tree("book_name_tree.dat"), book_author_tree("book_author_tree.dat"),book_keyword_tree("book_keyword_tree.dat"){}
    vector<Book> showType(BookInfoType _infotype, string _info) {
        for (auto itype : c_BookInfoTypes) {
            if (_infotype == itype) {
                return vAdd2vBook(c_bookShowFileMap.at(itype)->findVector(_info));
            }
        }
    }

    void insert(const Book &b){
        Address a = data_book.insert(b);
        book_isbn_tree.insert(b.isbn, a);
        book_name_tree.insert(b.book_name, a);
        book_author_tree.insert(b.author, a);
        vector<Keyword> keywords = splitKeyword(b.keyword);
        for(Keyword oneword : keywords){
            book_keyword_tree.insert(oneword, a);
        }
    }

    void erase(const ISBN &key){
        Address a = book_isbn_tree.find(key);
        Book b = data_book.find(a);
        data_book.erase(a);
        book_isbn_tree.erase(b.isbn);
        book_name_tree.erase(b.book_name);
        book_author_tree.erase(b.author);
        vector<Keyword> keywords = splitKeyword(b.keyword);
        for(Keyword oneword : keywords){
            book_keyword_tree.erase(oneword);
        }
    }

    Book find(const ISBN &key){
        return data_book.find(book_isbn_tree.find(key));
    }

    vector<Book> findAll(){
        return vAdd2vBook(book_isbn_tree.findAll());
    }

//    vector<Book> findVector(const ISBN &key){
//        vector<Address> v_address = book_isbn_tree.findVector(key);
//        vector<Book> v_book;
//        for (Address a : v_address){
//            v_book.push_back(data_book.find(a));
//        }
//        return v_book;
//    }

private:
    Data<Book> data_book;
    BPlusTree<ISBN, Address> book_isbn_tree;
    BPlusTree<Book_name, Address> book_name_tree;
    BPlusTree<Author, Address> book_author_tree;
    BPlusTree<Keyword, Address> book_keyword_tree;
    const map<BookInfoType, BPlusTree<StringType, Address>*> c_bookShowFileMap{{t_ISBN,      &book_isbn_tree},
        {t_Book_name, &book_name_tree},
        {t_Author,    &book_author_tree},
        {t_Keyword,   &book_keyword_tree}};
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
        sort(v_book.begin(), v_book.end());
        return v_book;
    }
};

class UserData{
public:
    UserData(): data_user("user_data.dat"), user_id_tree("user_id_tree.dat"){}
    void insert(const User &u){
        Address a = data_user.insert(u);
        user_id_tree.insert(u.user_id, a);
    }

    void erase(const User_id &key){
        data_user.erase(user_id_tree.find(key));
        user_id_tree.erase(key);
    }

    User find(const User_id &key){
        return data_user.find(user_id_tree.find(key));
    }

    vector<User> findAll(){
        return vAdd2vUser(user_id_tree.findAll());
    }

private:
    Data<User> data_user;
    BPlusTree<User_id, Address> user_id_tree;
    vector<User> vAdd2vUser(vector<Address> v_address){
        vector<User> v_user;
        for (Address a : v_address){
            v_user.push_back(data_user.find(a));
        }
        sort(v_user.begin(), v_user.end());
        return v_user;
    }



};


#endif //CODE_BOOKANDUSER_H
