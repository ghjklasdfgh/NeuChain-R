//
// Created by hzb on 23-4-17.
//

#include "block_server/coordinator/divide_batch_coordinator.h"

std::vector<std::vector<Transaction*>> DivideBatchCoordinator::divideTransactionBatch(const std::vector<Transaction*>& trWrapper){
    auto divideCount = YAMLConfig::getInstance()->divideBatchCount();
    std::vector<std::vector<Transaction*>> vectorDivideWrapper(divideCount);
    for(auto tx : trWrapper){
        auto txnIDTail = tx->getTransactionID() & ((uint64_t)(divideCount-1)) ;
        vectorDivideWrapper[txnIDTail].push_back(tx);
    }
    return vectorDivideWrapper;
}