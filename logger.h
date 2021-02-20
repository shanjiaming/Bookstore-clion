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
#define Info(x)    main_log << YELLOW  << ACT_INFO  << ": info: " << x << END << '\n'
#define Success    main_log << GREEN  << ACT_INFO  << ": success: " << __FUNCTION__ << END << '\n' << CUT
#define Error(x)   main_log << RED    << ACT_INFO  << ": error: " << x << END << '\n' << CUT

#define CHECKSTACK  user_vector.empty()?"NULL":
#define USER main_log << YELLOW << "user_id=" << (CHECKSTACK user_vector.back().user_id) << " authority=" << (user_vector.empty()?0:user_vector.back().authority) << END << '\n'
#define BOOK main_log << YELLOW << "selected book_id=" <<( CHECKSTACK user_vector.back().selected_book)  << END << '\n'
#define FLUSHLOG main_log << flush

typedef char Input[300];
typedef char TorD [50];
struct Operation{
    cUser_id user_id;
    Authority authority;
    cISBN selected_book;
    Input input;
    TorD time;
    TorD date;
    Operation() = default;
    Operation(const char *_userId, Authority authority, char *_selected_book, string _operation) : authority(authority)
                                                                           {strcpy(user_id,_userId);
                                                                               strcpy(input, _operation.c_str());
                                                                               strcpy(selected_book, _selected_book);
                                                                               strcpy(time, __TIME__);
                                                                               strcpy(date, __DATE__);
    }
};
#define OInfo {operation_log.seekp(0,ios::end); \
fwrite(operation_log, Operation(user_vector.back().user_id, user_vector.back().authority, user_vector.back().selected_book, input)); \
}
#define OFLUSHLOG operation_log << flush

#define FUSER finance_log << YELLOW << "user_id=" << (CHECKSTACK user_vector.back().user_id) << " authority=" << (user_vector.empty()?0:user_vector.back().authority) << END << '\n'
#define FFLUSHLOG finance_log << flush
#define FCUT finance_log << CUT
#define FInfo(x) FCUT;FUSER;finance_log << YELLOW  << __DATE__ << " " << __TIME__ << " " << __FUNCTION__ << ':' << x << END << '\n'


#define CODE_LOGGER_H

#endif //CODE_LOGGER_H
