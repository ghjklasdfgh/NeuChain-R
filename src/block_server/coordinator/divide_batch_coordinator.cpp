//
// Created by hzb on 23-4-17.
//

#include "block_server/coordinator/divide_batch_coordinator.h"

std::vector<std::vector<Transaction*>> DivideBatchCoordinator::divideTransactionBatch(const std::vector<Transaction*>& trWrapper){
    std::vector<std::vector<Transaction*>> vectorDivideWrapper(16);
    for(auto tx : trWrapper){
        auto txnIDTail = tx->getTransactionID() & ((uint64_t)15) ;
        vectorDivideWrapper[txnIDTail].push_back(tx);
    }
    return vectorDivideWrapper;
}