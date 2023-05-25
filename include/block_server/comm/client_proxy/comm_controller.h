//
// Created by peng on 2021/3/18.
//

#ifndef NEUBLOCKCHAIN_COMM_CONTROLLER_H
#define NEUBLOCKCHAIN_COMM_CONTROLLER_H

#include "common/zmq/zmq_server.h"
#include "block_server/comm/comm.h"
#include "common/concurrent_queue/blocking_mpmc_queue.h"
#include "common/concurrent_queue/blocking_concurrent_queue.hpp"
#include "common/hash_map.h"
#include "block_server/coordinator/divide_batch_coordinator.h"
#include <queue>
#include <unordered_map>

class ClientProxy;
class Transaction;

// this class is for mem mgmt for tx class
class TransactionRecycler: public Comm {
public:
    void clearTransactionForEpoch(epoch_size_t epoch) override;

protected:
    moodycamel::BlockingConcurrentQueue<Transaction*> addingTxQueue;

private:
    std::unordered_map<epoch_size_t, std::vector<Transaction*>> transactionMap;
};


class CommController: public TransactionRecycler{
public:
    explicit CommController(epoch_size_t startWithEpoch, ClientProxy* clientProxy);
    ~CommController() override;

    std::unique_ptr<std::vector<Transaction*>> getTransaction(epoch_size_t epoch, uint32_t maxSize, uint32_t minSize) override;

protected:
    void transferTransaction();

private:
    std::unique_ptr<ClientProxy> clientProxy;

    BlockingMPMCQueue<Transaction*> broadcastTransactions;
    std::queue<Transaction*> processedTransaction;

    std::vector<Transaction *> trWrapper;
    epoch_size_t logicalEpoch = 1;
    std::unique_ptr<DivideBatchCoordinator> divideBatchCoordinator = std::make_unique<DivideBatchCoordinator>();
    std::mutex mutex_;
    BlockingMPMCQueue<std::vector<Transaction *>> abc;
    std::unique_ptr<std::vector<Transaction *>> efg;
    bool divide;
};


#endif //NEUBLOCKCHAIN_COMM_CONTROLLER_H
