//
// Created by peng on 2021/3/18.
//

#include "block_server/comm/client_proxy/comm_controller.h"
#include "block_server/comm/client_proxy/client_proxy.h"
#include "block_server/transaction/transaction.h"
#include "glog/logging.h"

CommController::CommController(epoch_size_t startWithEpoch, ClientProxy* clientProxy)
        :clientProxy(clientProxy) {
    divide = YAMLConfig::getInstance()->divideTransactionBatch();
    auto preExecute = YAMLConfig::getInstance()->clProxyPreExecuteTransaction();
    auto divideCount = YAMLConfig::getInstance()->divideBatchCount();
    LOG(INFO) << "divide mode: " << divide << " divide count: " << divideCount;
    LOG(INFO) << "pre-execute mode: " << preExecute;
    logicalEpoch = 1;
    if (!divide) {
        clientProxy->epochBroadcastTransactionsHandle = [this] (Transaction* tx) {
            broadcastTransactions.push(tx);
        };
    } else {
        clientProxy->epochBroadcastTransactionsHandle = [this] (Transaction* tx) {
            std::unique_lock<std::mutex> lock(mutex_);
            if (trWrapper.empty()) {
                trWrapper.push_back(tx);
                return;
            }
            if (tx->getEpoch() == trWrapper.front()->getEpoch()) {
                trWrapper.push_back(tx);
                return;
            }
            auto divideBatchWrapper = divideBatchCoordinator->divideTransactionBatch(trWrapper);
            trWrapper.clear();
            for (auto &item : divideBatchWrapper) {
                if (item.empty()) {
                    continue;
                }
                DLOG(INFO) << "item size: " << item.size();
                for (auto txn: item) {
                    //LOG(INFO) << "transactionID: " << txn->getTransactionID();
                    txn->setEpoch(logicalEpoch);
                }
                abc.push(std::move(item));
                logicalEpoch++;
            }
        };
    }
    clientProxy->startService();
}

CommController::~CommController() = default;

std::unique_ptr<std::vector<Transaction*>> CommController::getTransaction(epoch_size_t epoch, uint32_t maxSize, uint32_t minSize) {
    if (divide) {
        if (efg == nullptr) {
            efg = std::make_unique<std::vector<Transaction*>>();
            abc.pop(*efg);
        }
        if (efg->front()->getEpoch() == epoch) {
            return std::move(efg);
        }
        return nullptr;
    }
    // 0. check if we already gave all trs in _epoch
    if(processedTransaction.empty()) {
        // 1. all trs in _epoch has not finished
        // 2. all trs in _epoch has finished, but we didnt received any _epoch+1 trs
        // for both 1 and 2, the processedTransaction queue must be empty,
        // so pull at least one transaction, check its epoch
        DCHECK(processedTransaction.empty() || processedTransaction.front()->getEpoch() == epoch);
        // we must wait for a transaction to fill in processedTransaction queue
        transferTransaction();
    }
    // check condition 2
    if(processedTransaction.front()->getEpoch() != epoch) {
        DCHECK(processedTransaction.front()->getEpoch() == epoch + 1 ||
               processedTransaction.front()->getEpoch() == epoch + 2);
        return nullptr;
    }
    // create new tr wrapper and reserve size.
    auto trWrapper1 = std::make_unique<std::vector<Transaction*>>();
    trWrapper1->reserve(maxSize);
    // deal with condition 1
    // 1.1 we have emptied processedTransaction queue but still not / just collect all trs
    // 1.2 we have collected all trs in _epoch and queue is not empty
    // for 1.1
    // 1.1.1 greater than minSize, less than maxSize, return
    // 1.1.2 greater than maxSize, return
    // 1.1.3 less than minSize, block wait
    // the loop condition is 1.1.2
    while(trWrapper1->size() < maxSize) {
        Transaction* transaction = processedTransaction.front();
        // 1.2
        if(transaction->getEpoch() != epoch) {
            break;
        }
        processedTransaction.pop();
        trWrapper1->push_back(transaction);
        addingTxQueue.enqueue(transaction);
        if(processedTransaction.empty()) {
            // 1.1.1
            if(trWrapper1->size() > minSize) {
                break;
            }
            // 1.1.3
            transferTransaction();
        }
    }
    return trWrapper1;
}

void CommController::transferTransaction() {
    // this func is a adaptor to processedTransaction, because concurrent queue does not support front() func.
    // when func return, make sure that processedTransaction has at least 1 tr
    Transaction *transaction;
    static epoch_size_t currentEpoch = 0;
    if (processedTransaction.empty()) {
        broadcastTransactions.pop(transaction);
        if (transaction->getEpoch() > currentEpoch)
            currentEpoch = transaction->getEpoch();
        CHECK(transaction->getEpoch() == currentEpoch);
        processedTransaction.push(transaction);
    }
    while (broadcastTransactions.try_pop(transaction)) {
        if (transaction->getEpoch() > currentEpoch)
            currentEpoch = transaction->getEpoch();
        CHECK(transaction->getEpoch() == currentEpoch);
        processedTransaction.push(transaction);
    }
}

void TransactionRecycler::clearTransactionForEpoch(epoch_size_t epoch) {
    // first add all tx from queue to map
    Transaction* tx = nullptr;
    // pre-alloc memory
    transactionMap[epoch + 20].reserve(10000);
    while (addingTxQueue.try_dequeue(tx)) {
        transactionMap[tx->getEpoch()].push_back(tx);
    }
    auto& previousEpochTransaction = transactionMap[epoch];
    for (auto tr: previousEpochTransaction) {
        if(tr->getEpoch() == epoch) {
            delete tr;
            continue;
        }
        transactionMap[tr->getEpoch()].push_back(tr);
    }
    transactionMap.erase(epoch);
}
