#define stub
//#define treedebug
//------------------------------------
//
// Created by JiamingShan on 2021/1/28.
//

#ifndef SRC_BPLUSTREE_HPP
#define SRC_BPLUSTREE_HPP

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <map>
#include "Error.h"
using namespace std;


//------------------------------------

typedef string FileName;

template <class TKey ,class TValue>
class BPlusTree {
private:
    FileName tree_fn;
    fstream ftree;
#ifdef stub
    multimap<TKey, TValue> Map;
#endif
public:
    BPlusTree<TKey, TValue>(const FileName &_tree_fn) : tree_fn(_tree_fn){}

#ifndef stub

private:

    static const int M=100, L=100;





#endif

#ifdef stub

    TValue find(const TKey &key) const{
        auto v = findVector(key);
        auto sz = v.size();
        if (sz == 0) throw NotFound();
        if (sz == 1) return v[0];
        throw MultipleElement();
    }

    void insert(const TKey &key, const TValue &o){
        Map.insert(make_pair(key, o));
    }

    void erase(const TKey &key){
        Map.erase(key);
    }

    vector<TValue> findVector(const TKey &key) const{
        vector<TValue> v;
        int i = Map.count(key);
        auto it = Map.find(key);
        while(i--){
            v.push_back(it++->second);
        }
        return v;
    }

    vector<TValue> findAll() const{
        vector<TValue> v;
        //TODO
    }

#ifdef treedebug
    void print(){
        for (auto i:Map){
            cout << i << endl;
        }
    }
#endif
#endif

};


















#endif //SRC_BPLUSTREE_HPP


//1.树的设计：双类型模板？
//2.怎么索引结构设计？

