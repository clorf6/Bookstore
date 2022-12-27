//
// Created by 屋顶上的小丑 on 2022/12/22.
//
#ifndef BOOKSTORE_UTILS_CPP
#define BOOKSTORE_UTILS_CPP

#include "Utils.h"

std::vector<std::string> ops;

void DivideOperation(const std::string& Op) {
    now_op.clear();
    ops.clear();
    for (auto& i : Op) {
        if (i == ' ') {
            ops.push_back(now_op);
            now_op.clear();
        } else {
            now_op += i;
        }
    }
    if (!now_op.empty()) ops.push_back(now_op);
}

bool JudgeUserIDAndPasswd(const std::string& s) {
    if (s.length() > 30) return false;
    for (auto& i : s) {
        if (!(std::isalpha(i) || std::isdigit(i) || i == '_')) return false;
    }
    return true;
}

bool JudgeUsername(const std::string& s) {
    if (s.length() > 30) return false;
    for (auto& i : s) {
        if (i <= 32 || i >= 127) return false;
    }
    return true;
}

bool JudgePrivilege(const std::string& s) {
    if (s.length() != 1) return false;
    if (s[0] == '1' || s[0] == '3' || s[0] == '7') return true;
    return false;
}

bool JudgeISBN(const std::string& s) {
    if (s.length() > 20 || !s.length()) return false;
    for (auto& i : s) {
        if (i <= 32 || i >= 127) return false;
    }
    return true;
}

bool JudgeBooknameAndAuthor(const std::string& s) {
    if (s.length() > 60 || s.length() <= 2) return false;
    if (s[0] != '"' || s.back() != '"') return false;
    for (int i = 1; i < s.length() - 1; i++) {
        if (s[i] <= 32 || s[i] >= 127 || s[i] == '"') return false;
    }
    return true;
}

void DivideKeyword(const std::string &_keyword, std::vector<std::string> &ret_keyword) {
    ret_keyword.clear();
    std::string now_keyword = "";
    for (auto& i : _keyword) {
        if (i == '|') {
            ret_keyword.push_back(now_keyword);
            now_keyword.clear();
        } else {
            now_keyword += i;
        }
    }
    if (!now_keyword.empty()) ret_keyword.push_back(now_keyword);
}

bool JudgeKeyword(const std::string& s) {
    if (s.length() > 60 || s.length() <= 2) return false;
    if (s[0] != '"' || s.back() != '"') return false;
    for (int i = 1; i < s.length() - 1; i++) {
        if (s[i] <= 32 || s[i] >= 127 || s[i] == '"') return false;
        if (s[i] == '|' && s[i - 1] == '|') return false;
    }
    DivideKeyword(s.substr(1, s.length() - 2), ans_keyword);
    std::sort(ans_keyword.begin(), ans_keyword.end());
    for (int i = 1; i < ans_keyword.size(); i++) {
        if (ans_keyword[i - 1] == ans_keyword[i]) {
            return false;
        }
    }
    return true;
}

bool JudgeQuantity(const std::string& s) {
    if (s.length() > 10 || !s.length()) return false;
    if (s.length() > 1 && s[0] == '0') return false;
    for (auto& i : s) {
        if (!std::isdigit(i)) return false;
    }
    long long Quantity = std::stoll(s);
    if (Quantity > maxnum || Quantity <= 0) return false;
    return true;
}

bool JudgePrice(const std::string& s) {
    if (s.length() > 13) return false;
    if (s.length() > 1 && (s[0] == '0' && s[1] != '.')) return false;
    if (std::count(s.begin(), s.end(), '.') > 1) return false;
    if (s[0] == '.' || s.back() == '.') return false;
    for (auto& i : s) {
        if (!(std::isdigit(i) || i == '.')) return false;
    }
    return true;
}

bool JudgeCost(const std::string& s) {
    if (!JudgePrice(s) || std::stod(s) < eps) return false;
    return true;
}

bool JudgeCount(const std::string& s) {
    if (s.length() > 10 || !s.length()) return false;
    if (s.length() > 1 && s[0] == '0') return false;
    for (auto& i : s) {
        if (!std::isdigit(i)) return false;
    }
    long long Count = std::stoll(s);
    if (Count > maxnum || Count < 0) return false;
    return true;
}

#endif //BOOKSTORE_UTILS_CPP