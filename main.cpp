//#define debug
//#define mainstub
//--------------------------------------------------

#include "BookAndUser.h"
#include "logger.h"

class ErrorOccur {
};

//-------------------------------------------------

UserData user_data;
BookData book_data;

vector<CoreUser> user_vector;

string input;


//-------------------------------------------------
//private Macros and functions



void checkAuthority(Authority x) {
    USER;
    if (user_vector.empty()) {
        Error("NO USER LOGIN YET");
        throw ErrorOccur();
    }
    if (user_vector.back().authority < x) {
        Error("NO AUTHORITY");
        throw ErrorOccur();
    }
}

void printBookVector(vector<Book> v_book) {//TODO sorted by isbn
    sort(v_book.begin(), v_book.end());
    if (v_book.empty()) {
        cout << endl;
        return;
    }
    for (Book i : v_book) {
        cout << i.isbn << '\t' << i.book_name << '\t' << i.author << '\t' << i.keyword << '\t' << i.price << '\t'
             << i.quantity << endl;
    }
}

#define FindSelectedBook  \
    if (user_vector.empty() || user_vector.back().selected_book == "") {\
        throw ErrorOccur();\
    }                  \
    cISBN& usb = user_vector.back().selected_book; \
    Book tbook = book_data.find(usb);                                   \

#define FindAndPopSelectedBook  \
    if (user_vector.empty() || user_vector.back().selected_book == "") {\
        throw ErrorOccur();\
    }                  \
    cISBN& usb = user_vector.back().selected_book; \
    Book tbook = book_data.find(usb);   \
    book_data.erase(usb);

//fileName functions

namespace file {

    void addFinance(Sign, Price);

}
//-------------------------------------------------

void initialize();

void function_chooser();

namespace user {

    void su(User_id, Passwd);

    void logout();

    void useradd(User_id, Passwd, Authority, User_name);

    void registerAccount(User_id, Passwd, User_name);

    void deleteAccount(User_id);

    void passwd(User_id, Passwd, Passwd);

}

namespace book {

    void select(ISBN);

    void modify(ISBN, Book_name, Author, Keyword, Price);

    void import(Quantity, Price);

    void show(BookInfoType, string);

    void showFinance(Time);

    void buy(ISBN, Quantity);

}

namespace sys {

    void reportFinance();

    void reportEmployee();

    void reportMyself();

    void log();

}


int main() {
#ifdef debug
    freopen("../../Data/AdvancedDataSet/testcase5/1.in", "r", stdin);
    freopen("../myout.txt", "w", stdout);
#endif
    initialize();
    while (true) {
        try {
            function_chooser();
        }
        catch (ErrorOccur) {
            cout << "Invalid" << endl;
        }
    }
    return 0;
}

void initialize() {
    auto firstOpen = []() -> bool {
        //TODO
    };
    auto openMark = []() {
        //TODO
    };
    cout << fixed << setprecision(2);
    ifstream tester("finance.dat");
    if (!tester) {
        user_data.insert(User("root", "sjtu", "root", 7));
        fcreate("finance.dat");
        fstream fs("finance.dat", ios::binary | ios::out | ios::in);
        fs.seekp(0);
        fwrite(fs, Price(0));
        fs.seekp(sizeof(Price));
        fwrite(fs, Price(0));
        fs.close();
        fcreate("operation.dat");
        //TODO
        openMark();
    } else {
        tester.close();
    }
    //TODO?
    Success;
}

void function_chooser() {

    smatch parameter;

    //management
    static const string
            user_id = " (\\w{1,30})",
            passwd = " (\\w{1,30})",
            user_name = " ([^ ]{1,30})",
            permission = " ([731])";
    static const regex
            rule_su("^su" + user_id + passwd + "$"),
            rule_su_direct("^su" + user_id + "$"),
            rule_logout("^logout$"),
            rule_useradd("^useradd" + user_id + passwd + permission + user_name + "$"),
            rule_register("^register" + user_id + passwd + user_name + "$"),
            rule_delete("^delete" + user_id + "$"),
            rule_passwd("^passwd" + user_id + passwd + passwd + "$"),
            rule_passwd_direct("^passwd" + user_id + passwd + "$");
    //book
    static const string
            _ISBN = "([^ ]{1,20})", csISBN = " " + _ISBN, e_ISBN =
            " -ISBN=" + _ISBN,//TODO ISBN 带空格吗？空白字符之类的要底要限多死？严重关心正则表达式。
    _book_name = "\"(.{1,60}?)\"", book_name = " " + _book_name, e_book_name = " -name=" + _book_name,
            _author = "\"(.{1,60}?)\"", author = " " + _author, e_author = " -author=" + _author,
            _keyword = "\"([^ ]{1,60}?)\"", keyword = " " + _keyword, e_keyword = " -keyword=" + _keyword,
            _price = "(\\d+(?:\\.\\d+)?)", price = " " + _price, e_price = " -price=" + _price,
            cost_price = price,
            quantity = " (\\d+)",
            time = " (\\d+)";
    static const regex
            rule_select("^select" + csISBN + "$"),
            rule_modify("^modify.*$"),
            rule_modify_ISBN("^modify.*" + e_ISBN + ".*$"),
            rule_modify_name("^modify.*" + e_book_name + ".*$"),
            rule_modify_author("^modify.*" + e_author + ".*$"),
            rule_modify_keyword("^modify.*" + e_keyword + ".*$"),
            rule_modify_price("^modify.*" + e_price + ".*$"),
            rule_import("^import" + quantity + cost_price + "$"),
            rule_show("^show.*$"),
            rule_show_ISBN("^show" + e_ISBN + "$"),
            rule_show_name("^show" + e_book_name + "$"),
            rule_show_author("^show" + e_author + "$"),
            rule_show_keyword("^show" + e_keyword + "$"),
            rule_show_finance("^show finance$"),
            rule_show_finance_time("^show finance" + time + "$"),
            rule_buy("^buy" + csISBN + quantity + "$");
    static const regex
            rule_report_finance("^report finance$"),
            rule_report_employee("^report employee$"),
            rule_report_myself("^report myself$"),
            rule_log("^log$"),
            rule_exit("^exit$"),
            rule_quit("^quit$");

    getline(cin, input);
    input.erase(input.find_last_not_of(" ") + 1);

//    USER;
    Info("try to " + input);

    if (regex_search(input, parameter, rule_su)) {
        user::su(parameter.str(1), parameter.str(2));
        return;
    }
    if (regex_search(input, parameter, rule_su_direct)) {
        user::su(parameter.str(1), "");
        return;
    }
    if (regex_search(input, parameter, rule_logout)) {
        user::logout();
        return;
    }
    if (regex_search(input, parameter, rule_useradd)) {
        user::useradd(parameter.str(1), parameter.str(2), stoi(parameter.str(3)), parameter.str(4));
        return;
    }
    if (regex_search(input, parameter, rule_register)) {
        user::registerAccount(parameter.str(1), parameter.str(2), parameter.str(3));
        return;
    }
    if (regex_search(input, parameter, rule_delete)) {
        user::deleteAccount(parameter.str(1));
        return;
    }
    if (regex_search(input, parameter, rule_passwd)) {
        user::passwd(parameter.str(1), parameter.str(2), parameter.str(3));
        return;
    }
    if (regex_search(input, parameter, rule_passwd_direct)) {
        user::passwd(parameter.str(1), "", parameter.str(2));
        return;
    }


    if (regex_search(input, parameter, rule_select)) {
        book::select(parameter.str(1));
        return;
    }
    if (regex_search(input, parameter, rule_modify)) {
        //FIXME: if these value are "" or -1, change won't happen. there include other place like show function
        ISBN l_ISBN;
        Book_name l_bookname;
        Author l_author;
        Keyword l_keyword;
        Price l_price = 0;

        if (regex_search(input, parameter, rule_modify_ISBN)) {
            l_ISBN = parameter.str(1);
        }
        if (regex_search(input, parameter, rule_modify_name)) {
            l_bookname = parameter.str(1);
        }
        if (regex_search(input, parameter, rule_modify_author)) {
            l_author = parameter.str(1);
        }
        if (regex_search(input, parameter, rule_modify_keyword)) {
            l_keyword = parameter.str(1);
        }
        if (regex_search(input, parameter, rule_modify_price)) {
            l_price = stod(parameter.str(1));
        }
        book::modify(l_ISBN, l_bookname, l_author, l_keyword, l_price);
        return;
    }

    if (regex_search(input, parameter, rule_show_finance)) {
        book::showFinance(-1);
        return;
    }
    if (regex_search(input, parameter, rule_show_finance_time)) {
        book::showFinance(stoi(parameter.str(1)));
        return;
    }
    if (regex_search(input, parameter, rule_import)) {
        book::import(stoi(parameter.str(1)), stod(parameter.str(2)));
        return;
    }
    if (regex_search(input, parameter, rule_show)) {
        if (input == "show") {
            book::show(t_ISBN, "");
            return;
        }
        BookInfoType l_infotype;
        string l_info;
        if (regex_search(input, parameter, rule_show_ISBN)) {
            l_infotype = t_ISBN;
            l_info = parameter.str(1);
        }
        if (regex_search(input, parameter, rule_show_name)) {
            l_infotype = t_Book_name;
            l_info = parameter.str(1);
        }
        if (regex_search(input, parameter, rule_show_author)) {
            l_infotype = t_Author;
            l_info = parameter.str(1);
        }
        if (regex_search(input, parameter, rule_show_keyword)) {
            l_infotype = t_Keyword;
            l_info = parameter.str(1);
        }
        book::show(l_infotype, l_info);
        return;
    }

    if (regex_search(input, parameter, rule_buy)) {
        book::buy(parameter.str(1), stoi(parameter.str(2)));
        return;
    }


    if (regex_search(input, parameter, rule_report_finance)) {
        sys::reportFinance();
        return;
    }
    if (regex_search(input, parameter, rule_report_employee)) {
        sys::reportEmployee();
        return;
    }
    if (regex_search(input, parameter, rule_report_myself)) {
        sys::reportMyself();
        return;
    }
    if (regex_search(input, parameter, rule_log)) {
        sys::log();
        return;
    }
    if (regex_search(input, parameter, rule_exit)) {
        exit(0);
    }
    if (regex_search(input, parameter, rule_quit)) {
        exit(0);
    }
    if (cin.eof()) {
        exit(0);
    }
    Error("SYNTAX ERROR");
    throw ErrorOccur();
}


void user::su(User_id _user_id, Passwd _passwd) {
    User l_user;
    try {
        l_user = user_data.find(_user_id);
    } catch (NotFound) {
        Error("NO THIS USER");
        throw ErrorOccur();
    }
    if (_passwd == "") {
        checkAuthority(l_user.authority * 2 + 1);
    } else {
        if (l_user.passwd != _passwd) {
            Error("WRONG PASSWD");
            throw ErrorOccur();
        }
    }
    user_vector.push_back(l_user);
    Success;
}

void user::logout() {
    checkAuthority(1);
    user_vector.pop_back();
    Success;
}

void user::useradd(User_id _user_id, Passwd _passwd, Authority _authority, User_name _user_name) {

    checkAuthority(_authority * 2 + 1);
    try {
        user_data.find(_user_id);
        Error("ALREADY EXISTS");
        throw ErrorOccur();
    }
    catch (NotFound) {
        user_data.insert(User(_user_id, _passwd, _user_name, _authority));
    }
    Success;
}

void user::registerAccount(User_id _user_id, Passwd _passwd, User_name _user_name) {
    user_data.insert(User(_user_id, _passwd, _user_name, 1));
    Success;
}

void user::deleteAccount(User_id _user_id) {
    checkAuthority(7);
    try {
        user_data.find(_user_id);
        for (auto each_login : user_vector) {
            if (each_login.user_id == _user_id) {
                Error("THIS USER HAS ALREADY LOGIN");
                throw ErrorOccur();
            }
        }
        user_data.erase(_user_id);
    }
    catch (NotFound) {
        Error("NO THIS USER");
        throw ErrorOccur();
    }
    Success;
}

void user::passwd(User_id _user_id, Passwd _old_passwd, Passwd _new_passwd) {
    User l_user;
    try {
        l_user = user_data.find(_user_id);
    } catch (NotFound) {
        Error("NO THIS USER");
        throw ErrorOccur();
    }
    if (_old_passwd == "") {
        checkAuthority(7);
    } else {
        checkAuthority(1);
        if (l_user.passwd != _old_passwd) {
            Error("WRONG PASSWORD");
            throw ErrorOccur();
        }
    }
    user_data.erase(_user_id);
    strcpy(l_user.passwd, _new_passwd.c_str());
    user_data.insert(l_user);
    Success;
}


void book::select(ISBN _isbn) {
    checkAuthority(3);
    try {
        book_data.find(_isbn);
    } catch (NotFound) {
        book_data.insert(Book(_isbn));
    }
    strcpy(user_vector.back().selected_book, _isbn.c_str());
    Success;
}

void book::modify(ISBN _isbn, Book_name _book_name, Author _author, Keyword _keyword, Price _price) {
    BOOK;
    checkAuthority(3);//FIXME
//    FindAndPopSelectedBook
    if (user_vector.empty() || !strcmp(user_vector.back().selected_book, "")) {
        Error("NO SELECTED BOOK");
        throw ErrorOccur();
    }
    if (_isbn != "") {
        try {
            book_data.find(_isbn);
            Error("NO BOOK WITH THIS ISBN");
            throw ErrorOccur();
        } catch (NotFound) {}
    }
    cISBN &usb = user_vector.back().selected_book;
    Book tbook = book_data.find(usb);
//    book_data.erase(usb);
    cISBN usb2;
    strcpy(usb2, usb);
    if (_isbn != "") {
        strcpy(usb, _isbn.c_str());
        strcpy(tbook.isbn, _isbn.c_str());
    }
    if (_book_name != "") {
        strcpy(tbook.book_name, _book_name.c_str());
    }
    if (_author != "") {
        strcpy(tbook.author, _author.c_str());
    }
    if (_keyword != "") {
        strcpy(tbook.keyword, _keyword.c_str());
    }
    if (_price != 0) {
        tbook.price = _price;
    }
//    book_data.insert(tbook);
    book_data.change(usb2, tbook);
    Success;
    OInfo;
}

void book::import(Quantity _quantity, Price _price) {
    BOOK;
    checkAuthority(3);
//    FindAndPopSelectedBook
    if (user_vector.empty() || !strcmp(user_vector.back().selected_book, "")) {
        Error("NO SELECTED BOOK");
        throw ErrorOccur();
    }
    cISBN &usb = user_vector.back().selected_book;
    Book tbook = book_data.find(usb);
//    book_data.erase(usb);

    tbook.quantity += _quantity;
//    book_data.insert(tbook);
    book_data.change(usb, tbook);
    file::addFinance('-', _price);
    Success;


    FInfo("  BookISBN=" << usb << "  Quantity=" << _quantity << "  Cost Per Book=" << double(_price) / _quantity
                        << "  Total Cost=" << _price);
    OInfo;
}

void book::show(BookInfoType _infotype, StringType _info) {
    checkAuthority(1);
    printBookVector((_infotype == t_ISBN && _info == "") ? book_data.findAll() : book_data.showType(_infotype, _info));
    Success;
}

void book::showFinance(Time _time) {//TODO
    checkAuthority(7);
    fstream finance_file("finance.dat", ios::binary | ios::in | ios::out);
    Price plus = 0, minus = 0;
    if (_time == -1) {
        finance_file.seekg(0);
        fread(finance_file, plus);
        finance_file.seekg(sizeof(Price));
        fread(finance_file, minus);
        finance_file.close();
    } else {
        Time t = _time;
        for (finance_file.seekg(0, ios::end); t--;) {
            Price temp;
            char c;
            finance_file.seekg(-sizeof(Price) - 1, ios::cur);
            fread(finance_file, c);
            fread(finance_file, temp);
            ((c == '+') ? plus : minus) += temp;
            finance_file.seekg(-sizeof(Price) - 1, ios::cur);
        }
        finance_file.seekg(-sizeof(Price) * 2, ios::cur);
        if (!finance_file) {
            Error("NO THAT MANY TRANSACTION TIMES");
            throw ErrorOccur();
        }
    }
    cout << "+ " << plus << " - " << minus << endl;
    Success;
}


void book::buy(ISBN _isbn, Quantity _quantity) {
    checkAuthority(1);
//    FindAndPopSelectedBook
    Book tbook;
    try {
        tbook = book_data.find(_isbn);
    } catch (NotFound) {
        Error("NO THIS BOOK");
        throw ErrorOccur();
    }
    if (tbook.quantity < _quantity) {
        Error("NO THAT MANY BOOKS, ONLY " << tbook.quantity << " BOOKS");
        throw ErrorOccur();
    }
//    book_data.erase(_isbn);
    tbook.quantity -= _quantity;
//    book_data.insert(tbook);
    book_data.change(_isbn, tbook);
    Price total_price = _quantity * tbook.price;
    file::addFinance('+', total_price);
    cout << total_price << endl;
    Success;
    FInfo("  BookISBN=" << _isbn << "  Quantity=" << _quantity << "  Income Per Book=" << tbook.price
                        << "  Total Income=" << total_price);
}


void sys::reportFinance() {//问题：未算总价，总价本来是可以记录于Basic文件中的。
    checkAuthority(7);
    FFLUSHLOG;
    ifstream fin("financelog.dat");
    string s;
    while (!fin.eof()) {
        getline(fin, s);
        cout << s << endl;
    }
    fin.close();
    fin.open("finance.dat", ios::binary | ios::in);
    Price plus, minus;
    fin.seekg(0);
    fread(fin, plus);
    fin.seekg(sizeof(Price));
    fread(fin, minus);
    fin.close();
    int profit = plus - minus;
    cout << CUT << CUT << "Your total income is " << plus << '\n' << "Your total outcome is " << minus << '\n'
         << "Your total profit is " << profit << '\n';
    if (profit > 0) cout << "Congratulations! Your bookstore makes a lot of money!" << '\n';
    else cout << "Opps! Your bookstore is losing money!" << '\n';
    cout << CUT << CUT << flush;

    Success;
}

void sys::reportEmployee() {
    checkAuthority(7);
//    vector<Operation> cache;
    map<User_id, vector<Operation> > employeesOperations;
    ifstream fin("operation.dat", ios::binary|ios::in);
    fin.seekg(0);

    while (true) {
//        cout << __LINE__ << endl;

        Operation temp;
        cout << "tellg" << fin.tellg()<<endl;
        fread(fin, temp);
        cout << "tellg" << fin.tellg()<<endl;
        if (!fin) break;
//cout << __LINE__ << endl;
//        cache.push_back(temp);
        if (temp.authority == 3) {
//            cout << __LINE__ << endl;
            employeesOperations[temp.user_id].push_back(temp);
        }
    }

    fin.close();
    cout << CUT << '\n';
    for (auto employeeOperations : employeesOperations) {
//        cout << __LINE__ << endl;
        cout << CUT << YELLOW << "Employee: UserID=" << employeeOperations.first << END << '\n'<< CUT;
        for (auto oper : employeeOperations.second){
//            cout << __LINE__ << endl;
            cout << YELLOW << "Date:" << oper.date <<
             "  Time:" << oper.time << END << '\n';
            cout << YELLOW << "Selected book_id=" << oper.selected_book << END << '\n';
            cout << YELLOW << "Opertion:" << oper.input << END << '\n' << '\n';
        }
        cout << CUT;

    }
    cout << CUT << '\n';

    Success;
}

void sys::reportMyself() {
    checkAuthority(3);
    Success;
}

void sys::log() {
    checkAuthority(7);
    FLUSHLOG;
    ifstream fin("log.dat");
    string s;
    while (!fin.eof()) {
        getline(fin, s);
        cout << s << endl;
    }
    fin.close();
    Success;
}


void file::addFinance(Sign _c, Price _price) {
    int I = ((_c == '+') ? 0 : sizeof(Price));
//    finance_vector.push_back(Finance(_c, _price));
    fstream finance_file("finance.dat", ios::binary | ios::in | ios::out);
    finance_file.seekg(I);
    Price temp;
    fread(finance_file, temp);
    finance_file.seekp(I);
    fwrite(finance_file, temp + _price);
    finance_file.seekp(0, ios::end);
    fwrite(finance_file, _c);
    fwrite(finance_file, _price);
    finance_file.close();
}

