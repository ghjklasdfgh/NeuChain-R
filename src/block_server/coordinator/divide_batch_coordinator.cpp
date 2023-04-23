//
// Created by hzb on 23-4-17.
//

#include "block_server/coordinator/divide_batch_coordinator.h"

std::unique_ptr<std::queue<std::unique_ptr<std::vector<Transaction*>>>> DivideBatchCoordinator::DivideTransactionBatch(std::unique_ptr<std::vector<Transaction*>> trWrapper){
    std::unique_ptr<std::queue<std::unique_ptr<std::vector<Transaction*>>>> queueDivideWrapper = std::make_unique<std::queue<std::unique_ptr<std::vector<Transaction*>>>>();
    for(tid_size_t i=0; i<16; i++){
        std::unique_ptr<std::vector<Transaction*>> txnDivideWrapper = std::make_unique<std::vector<Transaction*>>();
        for(auto *tx : *trWrapper){
            tid_size_t txnIDTail = tx->getTransactionID() & ((uint64_t)15) ;
            if(txnIDTail == i){
                txnDivideWrapper->push_back(tx);
            }
        }
        queueDivideWrapper->push(std::move(txnDivideWrapper));
    }
    return queueDivideWrapper;
}