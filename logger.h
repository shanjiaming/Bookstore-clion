//
// Created by JiamingShan on 2021/2/16.
//

#ifndef CODE_LOGGER_H


#include <iostream>
#include <fstream>
using namespace std;
ofstream fout("log.dat", ios::app);
ofstream ff("financelog.dat", ios::app);

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

#define ACT_INFO   __DATE__ << " " << __TIME__ << ':' << __FUNCTION__ << ':' << __LINE__
//把endl改为'\n'会减少超时吗？
//#define fout cout;
#define CUT "---------------------------------------------\n"
#define Info(x)    fout << YELLOW  << ACT_INFO  << ": info: " << x << END << '\n'
#define Success    fout << GREEN  << ACT_INFO  << ": success: " << __FUNCTION__ << END << '\n' << CUT
#define Error(x)   fout << RED    << ACT_INFO  << ": error: " << x << END << '\n' << CUT

#define CHECKSTACK  user_vector.empty()?"NULL":
#define USER fout << YELLOW << "user_id=" << (CHECKSTACK user_vector.back().user_id) << " authority=" << (user_vector.empty()?0:user_vector.back().authority) << END << '\n'
#define BOOK fout << YELLOW << "selected book_id=" <<( CHECKSTACK user_vector.back().selected_book)  << END << '\n'
#define FLUSHLOG fout << flush

#define FInfo(x) ff << YELLOW  << __DATE__ << " " << __TIME__ << ':'  << ": transaction: " << __FUNCTION__ << ':' << x << END << '\n'
#define FUSER ff << YELLOW << "user_id=" << (CHECKSTACK user_vector.back().user_id) << " authority=" << (user_vector.empty()?0:user_vector.back().authority) << END << '\n'
#define FFLUSHLOG ff << flush
#define FCUT ff << CUT

#define CODE_LOGGER_H

#endif //CODE_LOGGER_H
