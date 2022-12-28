//
// Created by 屋顶上的小丑 on 2022/12/20.
//
#ifndef BOOKSTORE_ACCOUNTSYSTEM_CPP
#define BOOKSTORE_ACCOUNTSYSTEM_CPP

#include "AccountSystem.h"

std::vector<OnlineUser> online;
std::unordered_map<int, int> is_login;

int GetPrivilege() {
    if (online.empty()) {
        return 0;
    }
    return online.back().user_privilege;
}

AccountSystem::AccountSystem(): account_pos("accountpos") {
    account_data.open("accountdata", std::ios::in | std::ios::out | std::ios::binary);
    if (!account_data.is_open()) {
        std::ofstream create;
        create.open("accountdata");
        create.close();
        account_data.open("accountdata", std::ios::in | std::ios::out | std::ios::binary);
        Account root("root", "sjtu", "", 7);
        WriteAccount(1, root);
        account_pos.insert(Element<int>{"root", 1});
    }
    online.clear();
    is_login.clear();
}

void AccountSystem::ReadAccount(int pos,
                                Account &ret) {
    account_data.seekg((pos - 1) * kSizeofAccount);
    account_data.read(reinterpret_cast<char *>(&ret), kSizeofAccount);
}

void AccountSystem::WriteAccount(int pos,
                                 Account &ret) {
    account_data.seekp((pos - 1) * kSizeofAccount);
    account_data.write(reinterpret_cast<char *>(&ret), kSizeofAccount);
}

void AccountSystem::LoginAccount(const std::string &User_ID,
                                 const std::string &Password) {
    account_pos.find(User_ID);
    if (account_pos.ans.empty()) {
        throw Exception("Invalid");
    }
    int pos = account_pos.ans[0];
    ReadAccount(pos, now_account);
    //printf("fucking %s\n",now_account.password);
    if (now_account.password != Password) {
        if (!Password.empty() || GetPrivilege() <= now_account.privilege) {
            throw Exception("Invalid");
        }
    }
    online.emplace_back(pos, 0, now_account.privilege);
    is_login[pos]++;
}

void AccountSystem::LogoutAccount() {
    if (online.empty()) {
        throw Exception("Invalid");
    }
    is_login[online.back().user_pos]--;
    online.pop_back();
}

void AccountSystem::RegisterAccount(const std::string &User_ID,
                                    const std::string &Password,
                                    const std::string &User_name) {
    account_pos.find(User_ID);
    if (!account_pos.ans.empty()) {
        throw Exception("Invalid");
    }
    Account new_account(User_ID, Password, User_name, 1);
    account_data.seekp(0, std::ios::end);
    int new_pos = account_data.tellp() / kSizeofAccount + 1;
    account_pos.insert(Element<int>{User_ID, new_pos});
    WriteAccount(new_pos, new_account);
}

void AccountSystem::ChangePassword(const std::string &User_ID,
                                   const std::string &CurPasswd,
                                   const std::string &NewPasswd) {
    if (online.empty()) {
        throw Exception("Invalid");
    }
    account_pos.find(User_ID);
    if (account_pos.ans.empty()) {
        throw Exception("Invalid");
    }
    int pos = account_pos.ans[0];
    ReadAccount(pos, now_account);
    if (now_account.password != CurPasswd) {
        if (!CurPasswd.empty() || GetPrivilege() != 7) {
            throw Exception("Invalid");
        }
    }
    memset(now_account.password, 0, 31);
    strcpy(now_account.password, NewPasswd.c_str());
    WriteAccount(pos, now_account);
}

void AccountSystem::AddAccount(const std::string &User_ID,
                               const std::string &Password,
                               const int &Privilege,
                               const std::string &User_name) {
    if (GetPrivilege() < 3) {
        throw Exception("Invalid");
    }
    if (GetPrivilege() <= Privilege) {
        throw Exception("Invalid");
    }
    account_pos.find(User_ID);
    if (!account_pos.ans.empty()) {
        throw Exception("Invalid");
    }
    Account new_account(User_ID, Password, User_name, Privilege);
    account_data.seekp(0, std::ios::end);
    int new_pos = account_data.tellp() / kSizeofAccount + 1;
    account_pos.insert(Element<int>{User_ID, new_pos});
    WriteAccount(new_pos, new_account);
}

void AccountSystem::DelAccount(const std::string &User_ID) {
    if (GetPrivilege() != 7) {
        throw Exception("Invalid");
    }
    account_pos.find(User_ID);
    if (account_pos.ans.empty()) {
        throw Exception("Invalid");
    }
    int pos = account_pos.ans[0];
    if (is_login[pos]) {
        throw Exception("Invalid");
    }
    account_pos.erase(Element<int>{User_ID, pos});
}

//void AccountSystem::GetTop() {
//    ReadAccount(online.back().user_pos, top);
//    std::cout << top.user_ID << ' ' << top.password << ' ' << top.user_name << ' ' << top.privilege << '\n';
//}

AccountSystem::~AccountSystem() {
    account_data.close();
    online.clear();
    is_login.clear();
}

#endif //BOOKSTORE_ACCOUNTSYSTEM_CPP

