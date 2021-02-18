//
// Created by JiamingShan on 2021/2/14.
//

#ifndef CODE_DATA_H
#define CODE_DATA_H


#include "filemanip.h"
using namespace std;


template <class T>
class Data {
private:
    FileName fileName;
    fstream file;



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


    T find(Address a) {
        openfile
        T ret;
        file.seekg(a);
        fread(file, ret);
        file.close();
        return ret;
    }

    Address insert(const T &t){
        openfile
        file.seekp(0, ios::end);//和上面呼应，注意这里的置尾
        Address ret = file.tellp();
        fwrite(file, t);
        file.close();
        return ret;
    }

    void erase(Address a){
        //Maybe we can do nothing.
    }

    void change(Address a, T t){
        openfile
        file.seekp(a);
        fwrite(file, t);
        file.close();
    }

};

#endif //CODE_DATA_H
