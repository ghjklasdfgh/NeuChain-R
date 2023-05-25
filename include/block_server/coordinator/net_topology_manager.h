//
// Created by user on 23-5-18.
//

#include <string>
#include <unordered_map>

#ifndef NEU_BLOCKCHAIN_NET_TOPOLOGY_MANAGER_H
#define NEU_BLOCKCHAIN_NET_TOPOLOGY_MANAGER_H

#endif //NEU_BLOCKCHAIN_NET_TOPOLOGY_MANAGER_H

class NetTopologyManager {
public:
    void addNode(const std::string& nodeId, const std::string& ipAddress) {
        routingTable[nodeId] = ipAddress;
    }

    void removeNode(const std::string& nodeId) {
        auto it = routingTable.find(nodeId);
        if (it != routingTable.end()) {
            routingTable.erase(it);
        } else {
        }
    }

    std::string getIpAddress(const std::string& nodeId) {
        auto it = routingTable.find(nodeId);
        if (it != routingTable.end()) {
            return it->second;
        } else {
            return "";
        }
    }

private:
    std::unordered_map<std::string, std::string> routingTable;
};