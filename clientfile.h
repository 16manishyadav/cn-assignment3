#ifndef CLIENT_H_
#define CLIENT_H_

#include <string>
#include <vector>
#include <algorithm>
#include <omnetpp.h>

using namespace omnetpp;
using namespace std;

class Mymessage : public cMessage {
public:
    std::string name;
    std::string mytype;
    std::string s;
    int source;
    int subtaskid;
    int destination;
    int taskno;
    int subtaskno;
    std::vector<int> v;
    int result;
};

class Client : public cSimpleModule {
public:
    int myindex;
    int numClients;
    int mylog;
    int final_result;
    int number_of_calculated_subtasks;
    int total_subtasks;
    std::vector<int> jump_options;
    map<int, int> gate_jump_map;
    std::map<std::string, bool> ML;

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    vector<int> calculate_my_jump();
    void sendsubtask(string mytype, std::vector<int> v, int taskno, int subtaskno, int subtaskid, int source, int destination);
    void SendToBestJump(Mymessage *msg, int destination);
    int res_ind(int destination);
};

Define_Module(Client);

#endif /* CLIENT_H_ */
