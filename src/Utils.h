//
// Created by 屋顶上的小丑 on 2022/12/22.
//

#ifndef BOOKSTORE_UTILS_H
#define BOOKSTORE_UTILS_H

#include <iostream>
#include <cstring>
#include <algorithm>
#include <string>
#include <vector>
#include "Exception.h"

extern std::vector<std::string> ops;
static std::string now_op;
static std::vector<std::string> ans_keyword;

const long long maxnum = 2147483647;
const double eps = 1e-7;

void DivideOperation(const std::string&);

bool JudgeUserIDAndPasswd(const std::string&);

bool JudgeUsername(const std::string&);

bool JudgePrivilege(const std::string&);

bool JudgeISBN(const std::string&);

bool JudgeBooknameAndAuthor(const std::string&);

void DivideKeyword(const std::string &, std::vector<std::string> &);

bool JudgeKeyword(const std::string&);

bool JudgeQuantity(const std::string&);

bool JudgePrice(const std::string&);

bool JudgeCost(const std::string&);

bool JudgeCount(const std::string&);

#endif //BOOKSTORE_UTILS_H
