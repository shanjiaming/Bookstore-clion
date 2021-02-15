//
// Created by JiamingShan on 2021/2/14.
//

#ifndef CODE_DATACLASS_H
#define CODE_DATACLASS_H

#include "BasicHeader.h"
#include "BPlusTree.hpp"

template <class TKey, class TValue>
class Data {
protected:
    FileName data_fn;
    fstream ftree, fdata;
    multimap<TKey, TValue> Map;
    BPlusTree<TKey, TValue> IDTree;
public:
    Data<TKey, TValue>(const FileName &_data_fn, const FileName &_tree_fn) : data_fn(_data_fn), IDTree(_tree_fn, data_fn){}


    TValue find(const TKey &key) const{
        auto v = findVector(key);
        auto sz = v.size();
        if (sz == 0) throw NotFound();
        if (sz == 1) return v[0];
        throw MultipleElement();
    }

    virtual void insert(const TValue &o){
        IDTree.insert(o.key(), o);
    }

    virtual void erase(const TKey &key){
        IDTree.erase(key);
    }

    virtual vector<TValue> findVector(const TKey &key) const{//TODO:不完善，除了换stub外，还要对各B+树进行处理
        vector<TValue> v;
        int i = Map.count(key);
        auto it = Map.find(key);
        while(i--){
            v.push_back(it++->second);
        }
        return v;
    }

    vector<TValue> findVectorAll() const{
        vector<TValue> v;
        for (auto i : Map){
            v.push_back(i.second);
        }
        return v;
    }

};

class BookData : public Data<StringType, Book>{
public:
    BookData(): Data<StringType, Book>("book_data.dat", "book_isbn_tree.dat"){}
    vector<Book> showType(BookInfoType _infotype, string _info) {
        for (auto itype : c_BookInfoTypes) {
            if (_infotype == itype) {
                return c_bookShowFileMap.at(itype)->findVector(_info);
            }
        }
    }

    void insert(const Book &o) final override{
        IDTree.insert(o.key(), o);
        book_name_tree.insert(o.book_name, o);
    }

    void erase(const ISBN &key) final override{
        IDTree.erase(key);
    }

    vector<Book> findVector(const ISBN &key) const final override{//TODO:不完善，除了换stub外，还要对各B+树进行处理
        vector<Book> v;
        int i = Map.count(key);
        auto it = Map.find(key);
        while(i--){
            v.push_back(it++->second);
        }
        return v;
    }

private:
//    BPlusTree<Book,ISBN> book_isbn_tree{"book_isbn_tree.dat",data_fn};
    BPlusTree<ISBN, Book>& book_isbn_tree = IDTree;
    BPlusTree<Book_name, Book> book_name_tree{"book_name_tree.dat",data_fn};
    BPlusTree<Author, Book> book_author_tree{"book_author_tree.dat",data_fn};
    BPlusTree<Keyword, Book> book_keyword_tree{"book_keyword_tree.dat",data_fn};
    const map<BookInfoType, BPlusTree<StringType, Book>*> c_bookShowFileMap{{t_ISBN,      &book_isbn_tree},
                                                                            {t_Book_name, &book_name_tree},
                                                                            {t_Author,    &book_author_tree},
                                                                            {t_Keyword,   &book_keyword_tree}};

};

class UserData : public Data<StringType, User>{
public:
    UserData(): Data<StringType, User>("user_data.dat", "user_id_tree.dat"){}
private:
//    BPlusTree<User,User_id> user_id_tree{"user_id_tree.dat", data_fn};

};

#endif //CODE_DATACLASS_H
