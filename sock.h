
//
// TODO: Define an enumeration to represent socket state.
//

enum sockstate { CLOSED, CONNECTED, LISTENING };

//
// TODO: Define a structure to represent a socket.
//

struct sock {
  int lport;
  int rport;
  enum sockstate sstate;        // Socket state
  int owner;
  char buf[128];
  int isAvailableData;
};
