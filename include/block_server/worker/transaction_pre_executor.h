//
// Created by hzb on 23-4-10.
//

#include "block_server/reserve_table/trasnaction_dependency.h"
#include "block_server/reserve_table/reserve_table.h"
#include "block_server/reserve_table/mock/mock_reserve_table.h"
#include "transaction_executor.h"
#include "block_server/worker/transaction_pre_reserve_table.h"
#include "block_server/worker/transaction_causality_judge.h"

#ifndef NEU_BLOCKCHAIN_TRANSACTION_PRE_EXECUTOR_H
#define NEU_BLOCKCHAIN_TRANSACTION_PRE_EXECUTOR_H

#endif //NEU_BLOCKCHAIN_TRANSACTION_PRE_EXECUTOR_H

class KVRWSet;
class Transaction;
class ReserveTable;

class TransactionPreExecutor{
public:
    ~TransactionPreExecutor() = default;
    bool SetReserveTable(const std::vector<Transaction *> &transactionList);
    static bool TransactionPreExecute(Transaction *transaction);
    bool TrDependencyAnalyse(Transaction* transaction);
    bool SetTrCausality(const std::vector<Transaction *> &transactionList);
    bool JudgeTrCausality(Transaction* txn);

protected:
    TransactionPreReserveTable preReserveTable;
    TransactionCausalityJudge txnCausalityJudge;

};