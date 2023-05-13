//
// Created by hzb on 23-4-28.
//

#include "block_server/transaction/transaction.h"
#include "common/hash_map.h"

#ifndef NEU_BLOCKCHAIN_TRANSACTION_CAUSALITY_JUDGE_H
#define NEU_BLOCKCHAIN_TRANSACTION_CAUSALITY_JUDGE_H

#endif //NEU_BLOCKCHAIN_TRANSACTION_CAUSALITY_JUDGE_H

class TransactionCausalityJudge{
public:
    ~TransactionCausalityJudge() = default;
    void SetTxnCausality(const std::vector<Transaction *> &transactionList);
    bool txnCausalityJudge(Transaction* txn);
protected:
    HashMap<tid_size_t, std::vector<Transaction*>> causalityTable;
};