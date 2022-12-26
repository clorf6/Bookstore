//
// Created by 屋顶上的小丑 on 2022/12/22.
//

#ifndef BOOKSTORE_LOGSYSTEM_H
#define BOOKSTORE_LOGSYSTEM_H

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include "Exception.h"

extern std::fstream finance;
extern std::fstream log_file;
extern int count;

struct Deal {
    double income;
    double outcome;

    explicit Deal(const double &Income = 0.0,
                  const double &Outcome = 0.0) :
            income(Income),
            outcome(Outcome) {};
};

const size_t kSizeofDeal = 16;
extern Deal now_deal, pre_deal;

void OpenLog();

void CloseLog();

void ReadDeal(int, Deal &);

void WriteDeal(int, Deal &);

void NowFinance();

void QueryFinance(const int&);
//void GenerateLog();

#endif //BOOKSTORE_LOGSYSTEM_H
