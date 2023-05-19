//
// Created by hzb on 23-4-10.
//

#include "block_server/reserve_table/trasnaction_dependency.h"
#include "block_server/reserve_table/reserve_table.h"
#include "block_server/reserve_table/mock/mock_reserve_table.h"
#include "transaction_executor.h"
#include "block_server/worker/transaction_causality_judge.h"

#ifndef NEU_BLOCKCHAIN_TRANSACTION_PRE_EXECUTOR_H
#define NEU_BLOCKCHAIN_TRANSACTION_PRE_EXECUTOR_H

#endif //NEU_BLOCKCHAIN_TRANSACTION_PRE_EXECUTOR_H

class KVRWSet;
class Transaction;
class ReserveTable;

class TransactionPreExecutor {
public:
    TransactionPreExecutor() :preReserveTable(0) {}
    ~TransactionPreExecutor() = default;
    bool transactionPreExecute(const std::vector<Transaction *> &transactionList);
    std::vector<Transaction *> trDependencyAnalyse(const std::vector<Transaction *> &transactionList);
    bool setTrCausality(const std::vector<Transaction *> &transactionList);
    bool judgeTrCausality(Transaction* txn);

protected:
    MockReserveTable preReserveTable;
    TransactionCausalityJudge txnCausalityJudge;

};