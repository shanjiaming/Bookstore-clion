//
// Created by JiamingShan on 2021/2/14.
//

#ifndef CODE_DATA_H
#define CODE_DATA_H

#include <string>
#include <fstream>
using namespace std;

typedef string FileName;
typedef long Address;

template <class T>
class Data {
private:
    FileName fileName;
    fstream file;
    Address offset;
//    ifstream fin;
//    ofstream fout;

    void fwrite(ofstream &_file, const T &t) {
        _file.write(reinterpret_cast<const char *>(&t), sizeof(t));
    }

    void fread(ifstream &_file, T &t) {
        _file.read(reinterpret_cast<char *>(&t), sizeof(t));
    }

    void openfile(){
        file.open(_fileName, ios::in | ios::out | ios::binary);
        assert(file);
    }

public:
    Data<T>(const FileName &_fileName) : fileName(_fileName){
        ifstream itemp(_fileName);
        if(!itemp){
            ofstream otemp;
            otemp.open(_fileName, ios::out | ios::binary);
            otemp.close();
        }
        //TODO 未考虑initialize。需要在打开模式的地方抉择是否进行文件创建。
    }


    T find(Address a) const{
        T ret;
        file.seekg(a);
        fread(file, ret);
        return ret;
    }

    Address insert(const T &t){
        Address ret = offset;
        file.seekp(offset);
        fwrite(file, t);
        offset += sizeof(t);
        return ret;
    }

    void erase(Address a){
        //Maybe we can do nothing.
    }

};

#endif //CODE_DATA_H
