#include "clientfile.h"
#include <fstream>

vector<int> Client::calculate_my_jump() {
    vector<int> jump_options;
    for(int j=0;j<mylog;j++) {
        int jump_distance = pow(2,j);
        int k = (myindex+jump_distance)%numClients;
        gate_jump_map[k] = j;
        jump_options.push_back(k);
    }
    return jump_options;
}

void Client::sendsubtask(string mytype, std::vector<int> v, int taskno, int subtaskno, int subtaskid, int source, int destination) {
    Mymessage *msg = new Mymessage();
    msg->source = source;
    msg->mytype = mytype;
    msg->v = v;
    msg->subtaskid = subtaskid;
    msg->taskno = taskno;
    msg->subtaskno = subtaskno;
    msg->destination=destination;
    SendToBestJump(msg, destination);
};

void Client::SendToBestJump(Mymessage *msg, int destination) {
    for(int i=jump_options.size()-1;i>=0;i--) {
        if(res_ind(jump_options[i])<=res_ind(destination)) {
            send(msg, "jumpout", i);
            break;
        }
    }
}

int Client::res_ind(int destination) {
    int index = destination;
    if(destination>myindex) {
        index = destination;
    } else {
        index = destination + numClients;
    }
    return index;
}

void Client::initialize() {
    number_of_calculated_subtasks = 0;
    final_result = INT_MIN;
    numClients = par("nc");
    mylog = par("mylog");
    myindex = getIndex();
    jump_options = calculate_my_jump();

    total_subtasks = par("x");
    int x = total_subtasks;
    int N = numClients;
    int k = par("subtask_size");
    vector<int> task;
    int task_array_size = x * k;

    // Generate and write task array elements to the file
    ofstream outFile("out.txt", ios::app); // Open the file in append mode
    outFile << "Task array:\n";
    for (int i = 0; i < task_array_size; i++) {
        int random_number = rand() % 100;
        task.push_back(random_number);
        outFile << random_number << " ";
    }
    outFile << "\n";
    outFile.close();

    // Send subtasks to clients
    for (int i = 0; i < x; i++) {
        std::vector<int> subtask(task.begin() + i * k, task.begin() + (i + 1) * k);
        int ClientId = i % N;
        if(ClientId!=myindex) {
            sendsubtask("subtask", subtask, i, i, 1, myindex, ClientId);
        } else {
            final_result = max(final_result, *max_element(subtask.begin(), subtask.end()));
            number_of_calculated_subtasks++;
        }
    }
}


void Client::handleMessage(cMessage *msg) {
    Mymessage *m = check_and_cast<Mymessage *>(msg);
    string mytype = m->mytype;
    int source = m->source;
    int destination = m->destination;

    ofstream outFile("out.txt", ios::app); // Open the file in append mode

    // Write subtask, output, and gossip message details to the file
    outFile << "Handling message of type: " << mytype << "\n";
    outFile << "Source: " << source << ", Destination: " << destination << "\n";
    if (mytype == "subtask") {
        outFile << "Subtask received with task ID: " << m->taskno << ", subtask ID: " << m->subtaskid << "\n";
        outFile << "Subtask data: ";
        for (int i = 0; i < m->v.size(); i++) {
            outFile << m->v[i] << " ";
        }
        outFile << "\n";
    } else if (mytype == "result") {
        outFile << "Result received from source: " << source << ", result: " << m->result << "\n";
    }

    // Write array elements to the file
    outFile << "Array elements: ";
    for (int i = 0; i < m->v.size(); i++) {
        outFile << m->v[i] << " ";
    }
    outFile << "\n";

    // Close the file
    outFile.close();

    if (destination == myindex) {
        if (mytype == "subtask") {
            int maxElement = *max_element(m->v.begin(), m->v.end());
            m->result = maxElement;
            m->mytype = "result";
            m->destination = source;
            m->source = myindex;
            SendToBestJump(m, m->destination);
        } else if (mytype == "result") {
            final_result = max(final_result, m->result);
            number_of_calculated_subtasks++;
            if (number_of_calculated_subtasks == total_subtasks) {
                EV << "final result is " << final_result << "\n";
            }
        }
    } else {
        string hash = to_string(m->source) + to_string(m->subtaskid) + to_string(m->subtaskno);
        if (ML.find(hash) != ML.end()) {
            return;
        }
        ML[hash] = true;
        SendToBestJump(m, destination);
    }
}

