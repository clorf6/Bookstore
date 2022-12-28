## Bookstore 开发文档

Written by clorf/蒋松霖

##### 程序功能概述

实现一个用 C++ 语言编写的书店管理系统，并使用命令行交互。用于向各种不同类型的账户（店主、员工、顾客、访客）提供图书检索与购买相关服务。

* 交互方式

  全程使用命令行（Windows CMD 或 Linux Terminal）进行操作，输入数据以换行符或回车符为分隔为若干指令。详情见[Bookstore 业务需求](https://github.com/ACMClassCourse-2022/Bookstore-2022/blob/master/requirements.md)。

* 账户系统

  有三种不同权限的用户，包括**店主（超级管理员），员工（管理员）以及顾客（用户）**。支持的操作有：登陆账户，注销账户，注册账户，修改密码，创建账户，删除账户。在未登录账户前为游客状态。

* 图书系统

  维护图书信息，包括**ISBN，书名，关键词，及其购买数量，单价和交易总额**。支持的操作有：检索图书，购买图书，选择图书，修改图书信息，图书进货。
  
* 日志系统

  负责记录书店管理系统运行过程中的各种数据（**交易笔数，收入支出等**），提供各类日志信息查询服务。支持的操作有：财务记录查询，生成日志。

需要注意的是，由于图书信息可能过大，需要将部分信息通过文件保存，需要时再通过读写文件进行对应操作。

##### 主题逻辑说明

对于每个系统独立开一个特定的类，并在类中实现对应的操作函数。同时在输入时便要维护一个日志文件，将信息输入到日志中。对于各种指令操作，需要开一个登录栈来记录实时操作的账户。对于不合法的操作则通过报错得到错误信息。

##### 代码文件结构

```
Bookstore
├─ .gitignore
├─ CMakeLists.txt
├─ README.md
└─ src
   ├─ AccountSystem.cpp
   ├─ AccountSystem.h
   ├─ BlockLinkedList.h
   ├─ Bookstore.cpp
   ├─ Bookstore.h
   ├─ BookSystem.cpp
   ├─ BookSystem.h
   ├─ Exception.h
   ├─ main.cpp
   ├─ Utils.cpp
   └─ Utils.h
```

##### 类的接口及成员

账户系统类：

```cpp
struct Account {
    char user_id[31]{};
    char password[31]{};
    char user_name[31]{};
    int privilege;

    Account(const std::string &_user_ID = "",
            const std::string &_password = "",
            const std::string &_user_name = "",
            const int &_privilege = 0)；
};

struct OnlineUser {
    int user_pos;
    int book_pos;
    int user_privilege;

    OnlineUser(const int &User_pos = 0,
               const int &Book_pos = 0,
               const int &User_privilege = 0) :
            user_pos(User_pos),
            book_pos(Book_pos),
            user_privilege(User_privilege) {};
};

extern std::unordered_map<int, int> is_login;
extern std::vector<OnlineUser> online;

extern int GetPrivilege();

extern Account now_account;
const size_t kSizeofAccount = sizeof(Account);

class AccountSystem {
private:
    std::fstream account_data;
    BlockLinkedList<int> account_pos;
public:
    AccountSystem();

    void ReadAccount(int, Account &);

    void WriteAccount(int, Account &);

    void LoginAccount(const std::string &, const std::string &);

    void LogoutAccount();

    void RegisterAccount(const std::string &, const std::string &, const std::string &);

    void ChangePassword(const std::string &, const std::string &, const std::string &);

    void AddAccount(const std::string &, const std::string &, const int &, const std::string &);

    void DelAccount(const std::string &);

    ~AccountSystem();
};
```

图书系统类：

```cpp
struct Book {
    char ISBN[21]{};
    char book_name[61]{};
    char author[61]{};
    char keyword[61]{};
    int quantity;
    double price;

    Book(const std::string &isbn = "",
         const std::string &_book_name = "",
         const std::string &_author = "",
         const std::string &_keyword = "",
         const int &_quantity = 0,
         const double &_price = 0.0)；

    bool operator<(const Book &x) const；
};

const size_t kSizeofBook = sizeof(Book);
static Book now_book;
static std::vector<Book> ans_book;

struct Deal {
    double income;
    double outcome;

    Deal(const double &Income = 0.0,
         const double &Outcome = 0.0)；
};

const size_t kSizeofDeal = 16;
static Deal now_deal, pre_deal;

class BookSystem {
private:
    BlockLinkedList<int> isbn_pos;
    BlockLinkedList<int> book_name_pos;
    BlockLinkedList<int> author_pos;
    BlockLinkedList<int> keyword_pos;
    std::fstream book_data;
    std::fstream finance;
    int count;
public:
    BookSystem();

    void ReadBook(int, Book &);

    void WriteBook(int, Book &);

    void PrintAllBook();

    void SearchBookByISBN(const std::string &);

    void SearchBookByBookName(const std::string &);

    void SearchBookByAuthor(const std::string &);

    void SearchBookByKeyword(const std::string &);

    void BuyBook(const std::string &, const int &);

    int AddBook(const std::string &);

    void SelectBook(const std::string &);

    bool JudgeModify();

    void ModifyBookISBN(const std::string &);

    void ModifyBookName(const std::string &);

    void ModifyBookAuthor(const std::string &);

    void ModifyBookKeyword(const std::string &);

    void ModifyBookPrice(const double &);

    void ImportBook(const int &, const double &);

    void ReadDeal(int, Deal &);

    void WriteDeal(int, Deal &);

    void NowFinance();

    void QueryFinance(const int &);

    void PrintAllFinance();

    ~BookSystem();
};

```

书店类及全局函数：

```cpp
static std::string op;
static std::string str;

class Bookstore {
private:
    AccountSystem account_system;
    BookSystem book_system;
    std::fstream log_file;
    int op_count;
public:
    Bookstore();

    void GenerateLog();

    void Run();

    ~Bookstore();
};
```

##### 文件存储说明

通过块状链表实现数据库，同时通过文件读写将数据库存于外存中。对于账户以及图书信息用单个文件存储所有信息，并使用数据库存储关键信息到文件中所在位置的映射。

##### 其他补充说明

对于日志系统，采取将日志系统中的财务信息与图书系统合并，在图书类中实现财务记录查询；对于日志系统中的日志功能则与书店类合并，在书店类中实现查询日志功能。
