//
// Created by hzb on 23-4-10.
//

#include "block_server/transaction/transaction.h"
#include "block_server/worker/transaction_pre_executor.h"
#include "block_server/worker/chaincode_manager.h"
#include "block_server/reserve_table/reserve_table.h"

bool TransactionPreExecutor::SetReserveTable(const std::vector<Transaction *> &transactionList){
    if(transactionList.empty())
        return false;
    // since all tx in list are from the same epoch, we only get reserve table once.
    for(auto transaction: transactionList){
        preReserveTable.preReserveRWSet(transaction->getRWSet(), transaction->getTransactionID());
    }
    return true;
}

bool TransactionPreExecutor::TransactionPreExecute(Transaction* transaction){
    auto dbTestTs = ChaincodeManager::getChaincodeInstance(transaction);
    if (!dbTestTs->InvokeChaincode(*transaction->getTransactionPayload())) {
        //1.transaction internal error, return false
        return false;
    }
    return true;
}
bool TransactionPreExecutor::TrDependencyAnalyse(Transaction* txn){
    auto&& dep = preReserveTable.TrDependencyAnalyse(txn->getRWSet(), txn->getTransactionID());
    if(dep.waw){
        return false;
    }else if(dep.war && dep.raw){
        return false;
    }
    return true;
}



