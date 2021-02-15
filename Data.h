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

    void fwrite(ostream &_file, const T &t) {
        _file.write(reinterpret_cast<const char *>(&t), sizeof(t));
    }

    void fread(istream &_file, T &t) {
        _file.read(reinterpret_cast<char *>(&t), sizeof(t));
    }

    void openfile(){
        file.open(fileName, ios::in | ios::out | ios::binary | ios::app);//FIXME 因为写文件不加app是覆盖式写法，会毁坏原来的数据。而加了app以后只不过是文件置尾，因为没有存到树的文件，也不记得那些数据到底是什么，也不会去读，所以看起来也没有影响。解决方法是加app-这步无论如何都要加，并且把树文件化-这步决定文件的持久性。
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


    T find(Address a) {
        openfile();
        T ret;
        file.seekg(a);
        fread(file, ret);
        file.close();
        return ret;
    }

    Address insert(const T &t){
        openfile();
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
        openfile();
        file.seekp(a);
        fwrite(file, t);
        file.close();
    }

};

#endif //CODE_DATA_H
