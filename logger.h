//
// Created by JiamingShan on 2021/2/16.
//

#ifndef CODE_LOGGER_H


#include <iostream>
#include "filemanip.h"
#include "BasicHeader.h"
using namespace std;


#define COLOUR
#ifdef COLOUR
#define RED     "\033[0;31;1m"
#define GREEN   "\033[0;32;1m"
#define YELLOW  "\033[0;33;1m"
#define BLUE    "\033[0;34;1m"
#define END     "\033[0m"
#else
#define RED     ""
#define GREEN   ""
#define YELLOW  ""
#define BLUE    ""
#define END     ""
#endif

#define ACT_INFO   __DATE__ << " " << __TIME__ << ':' << __FUNCTION__

#define CUT "---------------------------------------------\n"
#define Info(x)    ffout << YELLOW  << ACT_INFO  << ": info: " << x << END << '\n'
#define Success    ffout << GREEN  << ACT_INFO  << ": success: " << __FUNCTION__ << END << '\n' << CUT
#define Error(x)   ffout << RED    << ACT_INFO  << ": error: " << x << END << '\n' << CUT

#define CHECKSTACK  user_vector.empty()?"NULL":
#define USER ffout << YELLOW << "user_id=" << (CHECKSTACK user_vector.back().user_id) << " authority=" << (user_vector.empty()?0:user_vector.back().authority) << END << '\n'
#define BOOK ffout << YELLOW << "selected book_id=" <<( CHECKSTACK user_vector.back().selected_book)  << END << '\n'
#define FLUSHLOG ffout << flush

typedef char Input[300];
struct Operation{
    cUser_id user_id;
    Authority authority;
    cISBN selected_book;
    Input input;
    Input time;
    Input date;
    Operation() = default;
    Operation(const char *_userId, Authority authority, char *_selected_book, string _operation) : authority(authority)
                                                                           {strcpy(user_id,_userId);
                                                                               strcpy(input, _operation.c_str());
                                                                               strcpy(selected_book, _selected_book);
                                                                               strcpy(time, __TIME__);
                                                                               strcpy(date, __DATE__);
    }
};
#define OInfo {fo.seekp(0,ios::end);/*cout << "tellp"<<fo.tellp()<<endl;*/ \
fwrite(fo, Operation(user_vector.back().user_id, user_vector.back().authority, user_vector.back().selected_book, input)); \
/*cout << "tellp"<<fo.tellp()<<endl;*/}

#define FUSER ff << YELLOW << "user_id=" << (CHECKSTACK user_vector.back().user_id) << " authority=" << (user_vector.empty()?0:user_vector.back().authority) << END << '\n'
#define FFLUSHLOG ff << flush
#define FCUT ff << CUT
#define FInfo(x) FCUT;FUSER;ff << YELLOW  << __DATE__ << " " << __TIME__ << " " << __FUNCTION__ << ':' << x << END << '\n'


#define CODE_LOGGER_H

#endif //CODE_LOGGER_H
