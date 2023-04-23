//
// Created by hzb on 23-4-17.
//

#include "block_server/transaction/transaction.h"
#include "aria_coordinator.h"

#ifndef NEU_BLOCKCHAIN_DIVIDE_BATCH_COORDINATOR_H
#define NEU_BLOCKCHAIN_DIVIDE_BATCH_COORDINATOR_H

#endif //NEU_BLOCKCHAIN_DIVIDE_BATCH_COORDINATOR_H


class DivideBatchCoordinator{
public:
    ~DivideBatchCoordinator() = default;
    static std::unique_ptr<std::queue<std::unique_ptr<std::vector<Transaction*>>>> DivideTransactionBatch(std::unique_ptr<std::vector<Transaction*>> trWrapper);
protected:
};