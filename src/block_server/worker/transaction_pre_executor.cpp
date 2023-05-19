//
// Created by hzb on 23-4-10.
//

#include <glog/logging.h>
#include "block_server/transaction/transaction.h"
#include "block_server/worker/transaction_pre_executor.h"
#include "block_server/worker/chaincode_manager.h"
#include "block_server/reserve_table/reserve_table.h"
#include "kv_rwset.pb.h"
#include "common/yaml_config.h"


bool TransactionPreExecutor::transactionPreExecute(const std::vector<Transaction *> &transactionList) {
    for (auto transaction:transactionList) {
        transaction->readOnly(true);
        {
            //LOG(INFO)<<transaction->getTransactionID();
            auto dbTestTs = ChaincodeManager::getChaincodeInstance(transaction);
            if (!dbTestTs->InvokeChaincode(*transaction->getTransactionPayload())) {
                //1.transaction internal error, return false
                transaction->setTransactionResult(TransactionResult::ABORT_NO_PRE);
            }
            //LOG(INFO) << "rw:" << transaction->getRWSet()->writes().size() << ", re:" << transaction->getRWSet()->reads().size();
            this->preReserveTable.reserveRWSet(transaction->getRWSet(), transaction->getTransactionID());
        }
    }
    return true;
}
std::vector<Transaction *> TransactionPreExecutor::trDependencyAnalyse(const std::vector<Transaction *> &transactionList){
    std::vector<Transaction *> transactionPreList;
    for(auto txn:transactionList) {
        //LOG(INFO) << txn->getTransactionID() << ", " << txn->getRWSet()->writes_size() << ", " << txn->getRWSet()->reads_size();
        if(txn->getTransactionResult() == TransactionResult::ABORT_NO_PRE) {
            continue;
        }
        auto&& dep = preReserveTable.dependencyAnalysis(txn->getRWSet(), txn->getTransactionID());
        if(dep.waw) {
            continue;
        }
        if(dep.war && dep.raw) {
            continue;
        }
        txn->resetRWSet();
        txn->readOnly(false);
        transactionPreList.push_back(txn);
    }
    return transactionPreList;
}

bool TransactionPreExecutor::setTrCausality(const std::vector<Transaction *> &transactionList) {
    //self-define txn causality by txn features
    auto* configPtr = YAMLConfig::getInstance();
    if(!configPtr->clProxyPreExecuteTransaction())
        return false;
    if(!configPtr->judgeTransactionCausality())
        return false;
    if(transactionList.empty())
        return false;
    txnCausalityJudge.SetTxnCausality(transactionList);
    return true;
}

bool TransactionPreExecutor::judgeTrCausality(Transaction* txn) {
    auto* configPtr = YAMLConfig::getInstance();
    if(configPtr->judgeTransactionCausality()){
        if(txnCausalityJudge.txnCausalityJudge(txn)){ return true;}
    }
    return false;
}





