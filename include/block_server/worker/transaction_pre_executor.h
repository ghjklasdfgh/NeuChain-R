//
// Created by hzb on 23-4-10.
//

#ifndef NEU_BLOCKCHAIN_TRANSACTION_PRE_EXECUTOR_H
#define NEU_BLOCKCHAIN_TRANSACTION_PRE_EXECUTOR_H

#endif //NEU_BLOCKCHAIN_TRANSACTION_PRE_EXECUTOR_H

class Transaction;
class TransactionPreExecutor{
public:
    ~TransactionPreExecutor() = default;
    static bool TransactionPreExecute(Transaction *transaction);

};