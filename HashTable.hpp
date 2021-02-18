//#define stub
//#define treedebug
//------------------------------------
//
// Created by JiamingShan on 2021/1/28.
//

#ifndef SRC_BPLUSTREE_HPP
#define SRC_BPLUSTREE_HPP

#include <vector>
#include <iostream>
#include <algorithm>
#include <map>
#include <memory.h>
#include "filemanip.h"


//------------------------------------
class NotFound {};

class MultipleElement {};

template<class TKey, class TValue>
class HashTable {
private:
    FileName fileName;
    fstream file;
public:
    HashTable<TKey, TValue>(const FileName &_fileName) : fileName(_fileName) {
        fcreate(_fileName);
        file.seekp(0);
        fwrite(file, Block());
    }
#ifdef stub
    multimap<TKey, TValue> Map;
#endif

#ifndef stub

    using Node = pair<TKey, TValue>;
    static const int Nsize = sizeof(Node);
    static const int Nmax = 400;
    static const int Merge = 300;
    static const int Bsize = Nmax * Nsize;

//
//    int hash(const Node &n){
//        return n.second.first % R;
//    }


    struct Block {
        friend HashTable<TKey, TValue>;
        int next, num;
        Node nodes[Nmax];

        Block() : next(-1), num(0) {}
    };

        void getblock(Address x, Block &b) {
            file.seekp(x);
            fread(file, b);
        }

        void putblock(Address x, Block &b) {
            file.seekp(x);
            fwrite(file, b);
        };

        void putblockend(Block &b) {
            file.seekp(0, ios::end);
            fwrite(file, b);
        }
public:
        TValue find(const TKey &key) {
            vector<TValue> v = findVector(key);
            auto sz = v.size();
            if (sz == 0) throw NotFound();
            if (sz == 1) return v[0];
            throw MultipleElement();
        }

        void insert(const TKey &key, const TValue &o) {
            openfile
            Block bl;
            Node insert_node = {key, o};
            getblock(0, bl);
            Address block_pos = 0;
            while (bl.nodes[bl.num] < insert_node) {
                block_pos = bl.next;
                getblock(bl.next, bl);
            }
            int insert_pos = lower_bound(bl.nodes, bl.nodes + bl.num, insert_node) - bl.nodes;
            if (bl.num < Nmax) {//insert directly 0-base!
                Node temp = insert_node;
                Node record;
                for (int i = insert_pos; i < bl.num; ++i) {
                    record = bl.nodes[i];
                    bl.nodes[i] = temp;
                    temp = record;
                }
                ++bl.num;
                putblock(block_pos, bl);
            } else {// insert with split in insert pos
                Block bl_end;
                for (int i = insert_pos; i < bl.num; ++i) {
                    bl_end.nodes[i - insert_pos] = bl.nodes[i];
                }
                bl.nodes[insert_pos] = insert_node;
                bl_end.num = bl.num - insert_pos;
                bl.num = insert_pos + 1;
                file.seekp(0,ios::end);
                bl.next = file.tellp();
                putblock(block_pos, bl);
                putblockend(bl_end);
            }
            closefile
        }


        void erase(const TKey &key, const TValue &o) {
            openfile
            Block bl;
            Node erase_node = {key, o};
            getblock(0, bl);
            Address block_pos = 0;
            while (bl.nodes[bl.num] < erase_node) {
                block_pos = bl.next;
                getblock(bl.next, bl);
            }
            int erase_pos = lower_bound(bl.nodes, bl.nodes + bl.num, erase_node) - bl.nodes;

            Node temp = erase_node;
            Node record;
            for (int i = erase_pos + 1; i < bl.num; ++i) {
                bl.nodes[i - 1] = bl.nodes[i];
            }
            --bl.num;
            if (bl.next != -1) {//这么写可读性很不好，然而是getblock没法直接返回值的锅。
                Block bl_next;
                getblock(bl.next, bl_next);
                if (bl.num + bl_next.num > Merge) {//merge
                    for (int i = 0; i < bl_next.num; ++i) {
                        bl.nodes[i + bl.num] = bl_next.nodes[i];
                    }
                    bl.num += bl_next.num;
                    bl.next = bl_next.next;
                }
            }
            putblock(block_pos, bl);
            closefile
        }

        vector<TValue> findVector(const TKey &key) {
            openfile
            vector<TValue> v;
            Block bl;
            getblock(0, bl);
            while (bl.nodes[bl.num].first < key) {
                getblock(bl.next, bl);
            }
            vector<TKey> onlykey;
            for (int i = 0; i < bl.num; ++i) {
                onlykey.push_back(bl.nodes[i].first);
            }
            int find_pos = lower_bound(onlykey.begin(), onlykey.end(), key) - onlykey.begin();
            int find_max = upper_bound(onlykey.begin(), onlykey.end(), key) - onlykey.begin();
            for(int i = find_pos; i < find_max; ++i){
                v.push_back(bl.nodes[i].second);
            }
            if (find_max == bl.num - 1){
                Block bl_next;
                getblock(bl.next, bl_next);
                vector<TKey> onlykeynext;
                for (int i = 0; i < bl.num; ++i) {
                    onlykeynext.push_back(bl.nodes[i].first);
                }
                int find_max = upper_bound(onlykey.begin(), onlykey.end(), key) - onlykeynext.begin();
                for(int i = 0; i < find_max; ++i){
                    v.push_back(bl.nodes[i].second);
                }
            }
            return v;
        }

        vector<TValue> findAll() {
            openfile
            vector<TValue> v;
            Block bl;
            getblock(0, bl);
            do {
                for(int i = 0; i < bl.num; ++i){
                    v.push_back(bl.nodes[i].second);
                }
                getblock(bl.next, bl);
            }while (bl.next != -1);
            closefile
        }

#endif


#ifndef stub

    private:

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


        void erase(const TKey &key, const TValue &o){
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

