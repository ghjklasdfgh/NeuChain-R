//
// Created by hzb on 23-4-23.
//

#include "kv_rwset.pb.h"
#include "block_server/comm/comm.h"
#include "common/hash_map.h"
#include "block_server/reserve_table/trasnaction_dependency.h"

#ifndef NEU_BLOCKCHAIN_TRANSACTION_PRE_RESERVE_TABLE_H
#define NEU_BLOCKCHAIN_TRANSACTION_PRE_RESERVE_TABLE_H

#endif //NEU_BLOCKCHAIN_TRANSACTION_PRE_RESERVE_TABLE_H

class TransactionPreReserveTable{
public:
    bool preReserveRWSet(const KVRWSet* kvRWSet, tid_size_t transactionID);
    TransactionDependency TrDependencyAnalyse(const KVRWSet* kvRWSet, tid_size_t transactionID);
protected:
    HashMap<std::string, HashMap<std::string, std::atomic<tid_size_t>>> readTableList;
    HashMap<std::string, HashMap<std::string, std::atomic<tid_size_t>>> writeTableList;
};
