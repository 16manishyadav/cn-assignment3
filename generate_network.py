import math

NUMBER_OF_CLIENTS = 5
X = 7
SUBTASK_SIZE = 3

def make_jump_connectionsV2(no_of_clients):
    log_no_of_clients = math.ceil(math.log2(no_of_clients))
    jump_connections = ""
    for i in range(no_of_clients):
        for j in range(log_no_of_clients):
            jump_distance = 2 ** j
            k = (i + jump_distance) % no_of_clients
            jump_connections += f"    clients[{i}].jumpout++ --> {{delay=100ms;}} --> clients[{k}].jumpin++;\n"
    return jump_connections

def generate_ned_file(no_of_clients, log_no_of_clients):
    ned_file = f"""
network Network
{{
    parameters:
        @display("p=500,500");
        int NO_OF_CLIENTS = default({no_of_clients});
        int LOG_NO_OF_CLIENTS = default({log_no_of_clients});
        int x = default({X});
        int subtask_size = default({SUBTASK_SIZE});
        
    submodules:
        clients[NO_OF_CLIENTS]: Client {{
            parameters:
                nc = parent.NO_OF_CLIENTS;
                mylog = parent.LOG_NO_OF_CLIENTS;
                x = parent.x;
                subtask_size = parent.subtask_size;
        }}
        
    connections:


    // Jump connections
    {make_jump_connectionsV2(no_of_clients)}
        
    //    other connections using python
}}
"""
    return ned_file

# Usage
no_of_clients = NUMBER_OF_CLIENTS
print("Generating NED file for", no_of_clients, "clients...")
log_no_of_clients = math.ceil(math.log2(no_of_clients))
print("Log of number of clients:", log_no_of_clients)

generated_ned = generate_ned_file(no_of_clients, log_no_of_clients)

# Save the generated NED file
with open("net.ned", "w") as file:
    file.write(generated_ned)

print("File generated")
