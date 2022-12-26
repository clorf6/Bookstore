//
// Created by 屋顶上的小丑 on 2022/12/22.
//
#ifndef BOOKSTORE_LOGSYSTEM_CPP
#define BOOKSTORE_LOGSYSTEM_CPP

#include "LogSystem.h"

std::fstream finance;
std::fstream log_file;
int count;
Deal now_deal, pre_deal;

void OpenLog() {
    finance.open("finance", std::ios::in | std::ios::out | std::ios::binary);
    if (!finance.is_open()) {
        std::ofstream create;
        create.open("finance");
        finance.open("finance", std::ios::in | std::ios::out | std::ios::binary);
    }
    log_file.open("log", std::ios::in | std::ios::out | std::ios::binary);
    if (!log_file.is_open()) {
        std::ofstream create;
        create.open("log");
        finance.open("log", std::ios::in | std::ios::out | std::ios::binary);
    }
}

void CloseLog() {
    finance.close();
    log_file.close();
}

void ReadDeal(int pos, Deal &ret) {
    finance.seekg((pos - 1) * kSizeofDeal);
    finance.read(reinterpret_cast<char *>(&ret), kSizeofDeal);
}

void WriteDeal(int pos, Deal &ret) {
    finance.seekp((pos - 1) * kSizeofDeal);
    finance.write(reinterpret_cast<char *>(&ret), kSizeofDeal);
}

void NowFinance() {
    ReadDeal(count, now_deal);
    std::cout << std::fixed << std::setprecision(2)
              << "+ " << now_deal.income << " - " << now_deal.outcome << '\n';
}

void QueryFinance(const int& number) {
    if (!number) {
        std::cout << '\n';
        return ;
    }
    if (number > count) {
        throw Exception("Invalid");
    }
    ReadDeal(count, now_deal);
    ReadDeal(count - number, pre_deal);
    std::cout << std::fixed << std::setprecision(2)
              << "+ " << now_deal.income - pre_deal.income
              << " - " << now_deal.outcome - pre_deal.outcome << '\n';
}

#endif //BOOKSTORE_LOGSYSTEM_CPP