# Bookstore document

----

written by 

shanjiaming 520030910138

----

## Program Structure：

### Contain Structure 

- main.cpp

  - logger.h (tool header)

  - BookAndUser

    1. UnrolledLinkedList
  
    2. Data.h

       - BasicHeader.h
  

__Note__: all include `filemanip.h` (tool header)

## BasicHeader.h

- provide simpest struct about user `CoreUser` `User` and book `Book`. Also, some typedef are given.

  (some structs begin with c, meaning they are char[*], version not begin with c are strings)

## filemanip.h

- provide tool functions to manipulate file. Also, some typedef are given.
```
typedef string FileName;
typedef int Address;

inline void fcreate(FileName fn);

#define openfile ...
#define closefile ...

<T>
void fwrite(ostream &_file, const T &t);

<T>
void fread(istream &_file, T &t);
```

## Logger.h

- provide tool functions to manipulate log.  __Purely automatic__. __Expandable__ with almost __0 cost__.

  (**\_\_FUNCTION\_\_** is __GOD__!!!)

```
#define CUT
#define Info(x)
#define Success
#define Error(x)
#define USER
#define BOOK 
#define FLUSHLOG
#define FUSER
#define FFLUSHLOG
#define FCUT
#define FInfo(x)
struct Operation;
```

  

## Main.cpp

- initialize`initialize()`

- string parser`function_chooser()`


  - functions in `user` namespace


  ```
namespace user {
    void su(User_id, Passwd);
    void logout();
    void useradd(User_id, Passwd, Authority, User_name);
    void registerAccount(User_id, Passwd, User_name);
    void deleteAccount(User_id);
    void passwd(User_id, Passwd, Passwd);
}
  ```


  - functions in `book` namespace
  ```
namespace book {
    void select(ISBN);
    void modify(ISBN, Book_name, Author, Keyword, Price);
    void import(Quantity, Price);
    void show(BookInfoType, string);
    void showFinance(Times);
    void buy(ISBN, Quantity);
}
  ```

  - functions in `sys` namespace

  ```
namespace sys {
  void reportFinance();
  void reportEmployee();
  void reportMyself();
  void log();
}
  ```

   - functions in `tool` namespace

  ```
namespace tool {
  void checkAuthority(Authority x);
  void printBookVector(vector<Book> v_book);
  void addFinance(Sign, Price);
}
  ```

- provide data stuctures:

```
UserData user_data;
BookData book_data;
vector<CoreUser> user_vector;//record users who are login.
```

  While class `UserData` and `BookData` are defined in `BookAndUser.h`. Let's see what it can do.

  

__Note__: The pivotal logic of main is to use `function_chooser` to analyse the input string by regex, and then go to `user`, `book`, or `sys` functions to do the back-end work. In the work checked by OJ, it only uses the two class in BookAndUser, which has very few interfaces.	 

## BookAndUser

This header provides two classes, `UserData` and `BookData`. 
These two classes have very simple interface, which are easy to use.

### BookData

  ```
public:  
    vector<Book> showType(BookInfoType _infotype, string _info)
    void insert(const Book &b)
    void change(const ISBN &key, const Book &b)
    Book find(const ISBN &key)
    vector<Book> findAll()
  ```

The reason it can be so simple is due to the service `Data` and `UnrolledLinkedList` provided.

```
private:
    Data<Book> data_book;
    UnrolledLinkedList book_isbn_table;
    UnrolledLinkedList book_name_table;
    UnrolledLinkedList book_author_table;
    UnrolledLinkedList book_keyword_table;
```

Also, let's see `UserData`.

### UserData

  ```
public:  
    void insert(const User &u)
    void erase(const User_id &key)
    User find(const User_id &key)
  ```

```
private:
    Data<User> data_user;
    UnrolledLinkedList user_id_table;
```

Almost the same.

After that, let's see what are class `Data` and `UnrolledLinkedList`.

## Data.h

```
template <class T>
class Data {
private:
    FileName fileName;
    fstream file;
public:
    Data<T>(const FileName &_fileName);
    T find(Address a);
    Address insert(const T &t);
    void erase(Address a);//It do nothing.
    void change(Address a, T t);
};
```

It functions as map<Address, T>, storing the real data of books and users. Easy and short. 

Note: it does not have .cpp file. This is because usually, the definition of a template class is also in the header, says《C++  Primer中文版（第五版》p582, 模板编译。

## UnrolledLinkedList

### Functions
It functions like a map<string, int> that can be saved in a file permanently.

Two switchs. Default: close

```
#define Template
#define IO
```
The first `Template` switch enables you to change the ULL to an map<int, int>. Mostly used in debugging. 
The second `IO` switch enables you to print out the content in ULL. Mostly used in debugging. 

Interface provided:
```
public:
    UnrolledLinkedList(const FileName &_fileName);
    TValue find(const TKey &key);
    void insert(const TKey &_key, const Address &o);
    void erase(const TKey &_key, const Address &o);
    vector<TValue> findVector(const TKey &_key);
    vector<TValue> findAll();
# ifdef IO
public:
    void printVector(TKey k);
    void printAll();
#endif
```

### Template
Sorry to say, this class __cannot__ be a template class because of the changeable size of string. (If use char[], since char[] isn't assignable, it will bring out a lot of unnecessary difficulty.) Though, to make it works like a map<TKey, TValue>, we made the following definition.
```
using TKey = string;
using TKM = tuple<int, int, int>;
using TValue = Address;
```
ULL use `TKM hash(TKey)` to turn input type `TKey` to middle type `TKM`, which is easier to handle, because `TKM` __have constant size, operator=, operator<)__.

Also, you can change the ULL to other types by only changing TKey and TValue like a real template, if your desired type have constant size, operator=, operator<. 
Otherwise, you should rewrite `TKM hash(TKey)` to make the ULL work correctly. Since type `string` has not constant size, default ULL has already write the hash function of string.

### Structure
```
using Node = pair<TKM, TValue>;   
struct Block {    
    friend UnrolledLinkedList;        
    Address next = -1;      //next means the address of next Block in the file.        
    int num = 0;            //num means how many valid nodes are there in this Block.
    Node nodes[Nmax];       //This means 0 <= num <= Nmax
};
```
__Note__: Block has constant size.

__getblock-changeblock-putblock__. This is the way to complete one *read-write* operation like `insert` or `erase`. __getblock-analyseblock__. This is the way to complete one *read-only* operation like `findVector` or `findAll`.

Blocks are stored in a file.
```
private:
    FileName fileName;
    fstream file;
```

### Visualization

First we made some packaging to file manipulation. It included `filemanip.h` and provided some private interfaces.

```
void getblock(Address x, Block &b);
void putblock(Address x, const Block &b);
void putblockend(const Block &b);
```

Then write. All cliches. I wrote split logics in insert and merge in erase.

## The spirit of code: How to build this project clean and beautiful?

- As the saying goes, "**High aggregation and low coupling**" make a good code.  This embodies in the existence of `BookAndUser`, 

- Also, shorter code with better **packaging** are easier to write, debug and maintain. In addition, we can use **tool functions, tool headers and tool macros** smartly to eliminate duplicated code (Because of this, I only use one day to complete my `UnrollerLinkedList`, and it is extremely **short** and has **clear logic**).

- Fewer if-else are also better, lowering the logical complexcity of the code. We can use early return and const map to achieve this goal. See this in `function_chooser(main)`, `showType(BookData)`, `UnrollerLinkedList`, etc.

- Finally, we can write `log` like a **function decorater** (though it is not). This **decouples** business logic and log logic. You can see this clearly in my code. Almost nothing about log in `main` function.



​								