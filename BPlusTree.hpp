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
#include <memory.h>
#include "Error.h"
using namespace std;


//------------------------------------

typedef string FileName;
typedef long Address;

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

    static const int R=100, B=100, T=50;

    class Root {
    private:
        Address bas[R];
    public:
        Root(){
            memset(bas, -1, sizeof(bas) * R);
        }

    };

    class Block{
    private:
        Address sas[B];
    public:
        Block(){
            memset(sas, -1, sizeof(sas) * R);
        }
        void write(size_t x, writethings){

        }
    };

    class Element{
    private:

    public

    };

    class Pool{
        private:

    public:

    };




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

//    void erase(const TKey &key){
//        Map.erase(key);
//    }

    void preciseErase(const TKey &key, const TValue &o){
        int i = Map.count(key);
        auto it = Map.find(key);
        while(i--){
            if (it->second == o){
                auto itassert = ++it;
                --it;
                assert(itassert->first != key || itassert->second != o);
                Map.erase(it);
                return;
            }
            ++it;
        }
        assert(false);
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
        for (auto i:Map){
            v.push_back(i.second);
        }
        return v;
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

