
network Network
{
    parameters:
        @display("p=500,500");
        int NO_OF_CLIENTS = default(5);
        int LOG_NO_OF_CLIENTS = default(3);
        int x = default(7);
        int subtask_size = default(3);
        
    submodules:
        clients[NO_OF_CLIENTS]: Client {
            parameters:
                nc = parent.NO_OF_CLIENTS;
                mylog = parent.LOG_NO_OF_CLIENTS;
                x = parent.x;
                subtask_size = parent.subtask_size;
        }
        
    connections:


    // Jump connections
        clients[0].jumpout++ --> {delay=100ms;} --> clients[1].jumpin++;
    clients[0].jumpout++ --> {delay=100ms;} --> clients[2].jumpin++;
    clients[0].jumpout++ --> {delay=100ms;} --> clients[4].jumpin++;
    clients[1].jumpout++ --> {delay=100ms;} --> clients[2].jumpin++;
    clients[1].jumpout++ --> {delay=100ms;} --> clients[3].jumpin++;
    clients[1].jumpout++ --> {delay=100ms;} --> clients[0].jumpin++;
    clients[2].jumpout++ --> {delay=100ms;} --> clients[3].jumpin++;
    clients[2].jumpout++ --> {delay=100ms;} --> clients[4].jumpin++;
    clients[2].jumpout++ --> {delay=100ms;} --> clients[1].jumpin++;
    clients[3].jumpout++ --> {delay=100ms;} --> clients[4].jumpin++;
    clients[3].jumpout++ --> {delay=100ms;} --> clients[0].jumpin++;
    clients[3].jumpout++ --> {delay=100ms;} --> clients[2].jumpin++;
    clients[4].jumpout++ --> {delay=100ms;} --> clients[0].jumpin++;
    clients[4].jumpout++ --> {delay=100ms;} --> clients[1].jumpin++;
    clients[4].jumpout++ --> {delay=100ms;} --> clients[3].jumpin++;

        
    //    other connections using python
}
