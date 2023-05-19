//
// Created by hzb on 23-4-28.
//

#include "block_server/worker/transaction_causality_judge.h"
#include "common/yaml_config.h"

bool TransactionCausalityJudge::txnCausalityJudge(Transaction* txn) {
    auto txnCausalityVector = causalityTable[txn->getTransactionID()];
    for(auto txnCausality : txnCausalityVector){
        if(txnCausality->getTransactionResult() == TransactionResult::ABORT_NO_RETRY){
            return false;
        }
    }
    return true;
}

void TransactionCausalityJudge::SetTxnCausality(const std::vector<Transaction *> &transactionList) {
    //self-define txn causality by txn features
    auto* configPtr = YAMLConfig::getInstance();
    if(configPtr->judgeTransactionCausality()){
        for(auto txn:transactionList){
            auto tmpID = txn->getTransactionID();
            std::vector<Transaction*> causalityTxn;
            for(auto txn_new:transactionList){
                if((tmpID & txn_new->getTransactionID()) == 0){
                    causalityTxn.push_back(txn_new);

                }
                causalityTable.insert(tmpID,causalityTxn);
            }
        }
    }
}
