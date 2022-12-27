//
// Created by 屋顶上的小丑 on 2022/12/20.
//
#ifndef BOOKSTORE_ACCOUNTSYSTEM_H
#define BOOKSTORE_ACCOUNTSYSTEM_H

#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <vector>
#include <unordered_map>
#include "BlockLinkedList.h"
#include "Exception.h"

struct Account {
    char user_id[31]{};
    char password[31]{};
    char user_name[31]{};
    int privilege;

    Account(const std::string& _user_ID = "",
            const std::string& _password = "",
            const std::string& _user_name = "",
            const int& _privilege = 0) : privilege(_privilege) {
        memset(user_id, 0, 31);
        memset(password, 0, 31);
        memset(user_name, 0, 31);
        strcpy(user_id, _user_ID.c_str());
        strcpy(password, _password.c_str());
        strcpy(user_name,_user_name.c_str());
    };
};

struct OnlineUser {
    int user_pos;
    int book_pos;
    int user_privilege;

    OnlineUser(const int& User_pos = 0,
               const int& Book_pos = 0,
               const int& User_privilege = 0) :
               user_pos(User_pos),
               book_pos(Book_pos),
               user_privilege(User_privilege) {};
};

extern std::unordered_map<int, int> is_login;
extern std::vector<OnlineUser> online;
extern int GetPrivilege();
static Account now_account;
const size_t kSizeofAccount = sizeof(Account);

class AccountSystem {
private:
    std::fstream account_data;
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
//    void GetTop();
    ~AccountSystem();

    BlockLinkedList<int> account_pos;
};

#endif //BOOKSTORE_ACCOUNTSYSTEM_H
