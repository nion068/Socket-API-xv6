## Socket API (local loopback only) in xv6

### System calls implemented

#### int listen(int);
Listen to a local port (parameter). Return 0 for success, negative for failure.

#### int connect(int, const char* host);
Connect to remote port and host (parameter). Only "localhost" and "127.0.0.1" as host should be supported. Returns the local port number, if connection was successful. Negative value returned indicate failure.

#### int send(int, const char*, int);
Send a buffer to remote host. The local port, buffer, size of data are parameters. Return 0 for success. Negative value for failure. Send blocks, if remote host has not yet read earlier data.

#### int recv(int, char*, int);
Receive data from remote host. The local port, buffer, size of buffer are parameters. Return 0 for success. Negative value for failure. recv blocks, if no data is available.

#### int disconnect(int);
Disconnect (and close) the socket. The local port is parameter. 

### Implemented error codes
We have implemented the following error codes. These are available for user level programs, as well as in kernal space, as the defined constants.
1. E_NOTFOUND -1025
2. E_ACCESS_DENIED -1026
3. E_WRONG_STATE -1027
4. E_FAIL -1028
5. E_INVALID_ARG -1029

### Error Checking
1. Parameter issues return E_INVALID_ARG
2. Accessing a socket that is not in the stable return E_NOTFOUND
3. Accessing a socket from wrong process return E_ACCESS_DENIED
4. Attempts to send or receive, when the socket is not connected, return E_WRONG_STATE
5. If no more socket can be opened (limit exceeded), return E_FAIL

### Limitations
1. Send will block the caller process, until the recepients buffer is empty.
2. Timeout cannot be specified in the recv() call

