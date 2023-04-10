//
// Created by hzb on 23-4-10.
//

#include "block_server/transaction/transaction.h"
#include "block_server/worker/transaction_pre_executor.h"
#include "block_server/worker/chaincode_manager.h"
bool TransactionPreExecutor::TransactionPreExecute(Transaction* transaction){
    auto dbTestTs = ChaincodeManager::getChaincodeInstance(transaction);
    if (!dbTestTs->InvokeChaincode(*transaction->getTransactionPayload())) {
        //transaction internal error, abort it without adding transaction set
        return false;
    }
    return true;
}

