Bookstore document

----

written by shanjiaming

----

Program Structure：

1. Contain Structure

- main.cpp

  - BookAndUser.h

    1. HashTable.h

    2. Data.h

       - BasicHeader.h

# BasicHeader

- provide simpest struct about user `CoreUser` `User` and book `Book`

----

# Main.cpp

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
      void showFinance(Time);
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

- provide data stuctures:

  ```
  UserData user_data;
  BookData book_data;
  vector<CoreUser> user_vector;
  ```
  
  While class `UserData` and `BookData` are defined in `BookAndUser.h`. Let's see what it can do.
  

# BookAndUser.h

This header provides two classes, `UserData` and `BookData`. 
These two classes have very simple interface, which are easy to use.

## BookData

  ```
public:  
    vector<Book> showType(BookInfoType _infotype, string _info)
    void insert(const Book &b)
    void change(const ISBN &key, const Book &b)
    Book find(const ISBN &key)
    vector<Book> findAll()
  ```

The reason it can be so simple is due to the service `Data` and `HashTable` provided.

```
private:
    Data<Book> data_book;
    HashTable<ISBN, Address> book_isbn_table;
    HashTable<Book_name, Address> book_name_table;
    HashTable<Author, Address> book_author_table;
    HashTable<Keyword, Address> book_keyword_table;
```

Also, let's see `UserData`.

## UserData

  ```
public:  
    void insert(const User &u)
    void erase(const User_id &key)
    User find(const User_id &key)
  ```

```
private:
    Data<User> data_user;
    HashTable<User_id, Address> user_id_table;
```

Almost the same.

After that, let's see what are class `Data` and `HashTable`.







​								