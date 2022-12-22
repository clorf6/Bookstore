//
// Created by 屋顶上的小丑 on 2022/12/22.
//
#ifndef BOOKSTORE_LOGSYSTEM_CPP
#define BOOKSTORE_LOGSYSTEM_CPP

#include "LogSystem.h"

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

void QueryFinance(int& number) {
    if (!number) {
        std::cout << '\n';
        return ;
    }
    if (number > count) {
        std::cout << "Invalid\n";
        return ;
    }
    ReadDeal(count, now_deal);
    ReadDeal(count - number, pre_deal);
    std::cout << std::fixed << std::setprecision(2)
              << "+ " << now_deal.income - pre_deal.income
              << " - " << now_deal.outcome - pre_deal.outcome << '\n';
}

#endif //BOOKSTORE_LOGSYSTEM_CPP