//
// Created by hzb on 23-4-10.
//

#include "block_server/transaction/transaction.h"
#include "block_server/worker/transaction_pre_executor.h"
#include "block_server/worker/chaincode_manager.h"
#include "block_server/reserve_table/trasnaction_dependency.h"
#include "kv_rwset.pb.h"
#include "common/hash_map.h"
#include "block_server/reserve_table/reserve_table.h"

void TransactionPreExecutor::SetReserveTable(const std::vector<Transaction *> &transactionList){
    for(auto txn:transactionList){
        reserveTable->reserveRWSet(txn->getRWSet(), txn->getTransactionID());
    }
}

bool TransactionPreExecutor::TransactionPreExecute(Transaction* transaction){
    auto dbTestTs = ChaincodeManager::getChaincodeInstance(transaction);
    if (!dbTestTs->InvokeChaincode(*transaction->getTransactionPayload())) {
        //1.transaction internal error, abort it without adding transaction set
        //also need to analyse dependency (incomplete!)
        return false;
    }
    return true;
}
bool TransactionPreExecutor::TrDependencyAnalyse(Transaction* txn){
    auto&& dep = reserveTable->dependencyAnalysis(txn->getRWSet(), txn->getTransactionID());
    if(dep.waw){
        return false;
    }else if(dep.war && dep.raw){
        return false;
    }
    return true;
}

