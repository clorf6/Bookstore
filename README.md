## Bookstore 开发文档

Written by clorf/蒋松霖

##### 程序功能概述

实现一个用 C++ 语言编写的书店管理系统，并使用命令行交互。用于向各种不同类型的账户（店主、员工、顾客、访客）提供图书检索与购买相关服务。

* 交互方式

  全程使用命令行（Windows CMD 或 Linux Terminal）进行操作，输入数据以换行符或回车符为分隔为若干指令。详情见[Bookstore 业务需求](https://github.com/ACMClassCourse-2022/Bookstore-2022/blob/master/requirements.md)。

* 账户系统

  有**店主（超级管理员）**，**员工（管理员）以及顾客（用户）**三种不同权限的用户。支持的操作有：登陆账户，注销账户，注册账户，修改密码，创建账户，删除账户。在未登录账户前为游客状态。

* 图书系统

  维护图书信息，包括**ISBN，书名，关键词，及其购买数量，单价和交易总额**。支持的操作有：检索图书，购买图书，选择图书，修改图书信息，图书进货。
  
* 日志系统

  负责记录书店管理系统运行过程中的各种数据（**交易笔数，收入支出等**），提供各类日志信息查询服务。支持的操作有：财务记录查询，生成日志。

需要注意的是，由于图书信息可能过大，需要将部分信息通过文件保存，需要时再通过读写文件进行对应操作。

##### 主题逻辑说明

对于每个系统独立开一个特定的类，并在类中实现对应的操作函数。同时在输入时便要维护一个日志文件，将信息输入到日志中。对于各种指令操作，需要开一个登录栈来记录实时操作的账户。

##### 代码文件结构

暂无

##### 类的接口及成员

账户系统类：

```cpp
struct Account {
    std::string user_ID;
    std::string password;
    std::string user_name;
    int privilege;
}

class AccountSystem {
    private:
    	BlockLinkedList<Account, int> account_data;
    public:
    	AccountSystem();
        void RegisterAccount(std::string &, std::string &, std::string &);
        void ChangePassword(std::string &, std::string &, std::string &);
        void AddAccount(std::string &, std::string &, int &, std::string &);
        void DelAccount(std::string &);
    	~AccountSystem();
}
```

图书系统类：

```cpp
struct Book {
    std::string ISBN;
    std::string book_name;
    std::string author;
    std::vector<std::string> keyword;
    int quantity;
    double price;
    double total_cost;
}

class BookSystem {
    private:
        BlockLinkedList<Book, int> book_data;
    public:
        BookSystem();
        void SearchBook(std::string &, int &);
        void BuyBook(std::string &, int &);
        void SelectBook(std::string &);
        void ModifyBook(std::string &, int &);
        void ImportBook(int &, int &);
        ~BookSystem();
}
```

日志系统类：

```cpp
class LogSystem {
    private:
        int count;
        double income;
        double expense;
    public:
        Logsystem();
        void QueryFinance(int &);
        void GenerateLog();
        ~Logsystem();
}
```

书店类及全局函数：

```cpp
std::vector<int> login_stack;
class BookStore {
	private:
		AccountSystem account_system;
		BookSystem book_system;
		LogSystem log_system;
		int id;
	public:
		BookStore();
		void LoginAccount(std::string &, std::string &);
        void LogoutAccount();
        void Quit();
		~BookStore();
}
```

##### 文件存储说明

通过块状链表实现数据库，同时通过文件读写将数据库存于外存中，对于账户与图书以及日志均有一个数据库来维护。

##### 其他补充说明

暂无
