//
// Created by hzb on 23-4-10.
//

#include "block_server/transaction/transaction.h"
#include "block_server/worker/transaction_pre_executor.h"
#include "block_server/worker/chaincode_manager.h"
#include "block_server/reserve_table/reserve_table.h"
#include "common/yaml_config.h"

bool TransactionPreExecutor::SetReserveTable(const std::vector<Transaction *> &transactionList){
    if(transactionList.empty())
        return false;
    // since all tx in list are from the same epoch, we only get reserve table once.
    auto* configPtr = YAMLConfig::getInstance();
    if(configPtr->ClProxyPreExecuteTransaction()){
        for(auto transaction: transactionList){
            preReserveTable.preReserveRWSet(transaction->getRWSet(), transaction->getTransactionID());
        }
    }
    return true;
}

bool TransactionPreExecutor::TransactionPreExecute(Transaction* transaction){
    auto dbTestTs = ChaincodeManager::getChaincodeInstance(transaction);
    if (!dbTestTs->InvokeChaincode(*transaction->getTransactionPayload())) {
        //1.transaction internal error, return false
        transaction->setTransactionResult(TransactionResult::ABORT_NO_RETRY);
        return false;
    }
    return true;
}
bool TransactionPreExecutor::TrDependencyAnalyse(Transaction* txn){
    auto&& dep = preReserveTable.TrDependencyAnalyse(txn->getRWSet(), txn->getTransactionID());
    txn->resetRWSet();
    if(dep.waw){
        return false;
    }else if(dep.war && dep.raw){
        return false;
    }
    return true;
}

bool TransactionPreExecutor::SetTrCausality(const std::vector<Transaction *> &transactionList) {
    //self-define txn causality by txn features
    if(transactionList.empty())
        return false;
    auto* configPtr = YAMLConfig::getInstance();
    if(!configPtr->ClProxyPreExecuteTransaction())
        return false;
    if(!configPtr->JudgeTransactionCausality())
        return false;
    txnCausalityJudge.SetTxnCausality(transactionList);
    return true;
}

bool TransactionPreExecutor::JudgeTrCausality(Transaction* txn) {
    auto* configPtr = YAMLConfig::getInstance();
    if(configPtr->JudgeTransactionCausality()){
        if(txnCausalityJudge.txnCausalityJudge(txn)){ return true;}
    }
    return false;
}





