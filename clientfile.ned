simple Client
{
    parameters:
        int nc = default(1);
        int mylog = default(1);
        int x=default(1);
        int subtask_size = default(1);
        
    gates:
        input in[];
        output out[];
        input jumpin[];
        output jumpout[];
}
