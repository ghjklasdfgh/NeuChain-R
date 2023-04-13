//
// Created by hzb on 23-4-10.
//

#include "block_server/reserve_table/trasnaction_dependency.h"

#ifndef NEU_BLOCKCHAIN_TRANSACTION_PRE_EXECUTOR_H
#define NEU_BLOCKCHAIN_TRANSACTION_PRE_EXECUTOR_H

#endif //NEU_BLOCKCHAIN_TRANSACTION_PRE_EXECUTOR_H

class Transaction;
class ReserveTable;

class TransactionPreExecutor{
public:
    ~TransactionPreExecutor() = default;
    void SetReserveTable(const std::vector<Transaction *> &transactionList);
    static bool TransactionPreExecute(Transaction *transaction);
    bool TrDependencyAnalyse(Transaction* transaction);
 protected:
    ReserveTable* reserveTable{};

};