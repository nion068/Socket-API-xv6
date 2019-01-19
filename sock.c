#include "types.h"
#include "defs.h"
#include "param.h"
#include "spinlock.h"
#include "sock.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "proc.h"

//
// TODO: Create a structure to maintain a list of sockets
// Should it have locking?
//

struct {
  struct spinlock lock;
  struct sock sock[NSOCK];
} stable;

void
sinit(void)
{
  //
  // TODO: Write any initialization code for socket API
  // initialization.
  //
  initlock(&stable.lock, "stable");

}

struct sock*
getemptysocket(void)
{
    struct sock *s;
//  char *sp;

  //acquire(&stable.lock);

  for(s = stable.sock; s < &stable.sock[NSOCK]; s++)
    if(s->sstate == CLOSED)
      goto found;

  //release(&stable.lock);
  return 0;
found:
  //release(&stable.lock);
  return s;
};

struct sock*
getsocket(int port)
{
    struct sock *s;
//  char *sp;

  //acquire(&stable.lock);

  for(s = stable.sock; s < &stable.sock[NSOCK]; s++)
    if(s->sstate == CONNECTED && s->lport == port)
      goto found;

  //release(&stable.lock);
  return 0;
found:
  //release(&stable.lock);
  return s;
};

int
listen(int lport) {

  //
  // TODO: Put the actual implementation of listen here.
  //
  struct sock *s;
//  char *sp;

  acquire(&stable.lock);

  for(s = stable.sock; s < &stable.sock[NSOCK]; s++)
    if(s->sstate == CLOSED)
      goto found;

  release(&stable.lock);
  return -1;

found:
  s->lport = lport;
  s->owner = myproc()->pid;
  s->isAvailableData = 0;   // no data;
  strncpy(s->buf, "", 128);
  s->sstate = LISTENING;

  sleep(s, &stable.lock);

  release(&stable.lock);

  return 0;
}

int
connect(int rport, const char* host) {
  //
  // TODO: Put the actual implementation of connect here.
  //
  struct sock *s;
  int clientport = 127;
//  char *sp;
  struct sock *s_client;

  acquire(&stable.lock);

  for(s = stable.sock; s < &stable.sock[NSOCK]; s++)
    if(s->sstate == LISTENING && s->lport == rport)
      goto found;

  release(&stable.lock);
  return -1;

found:
  s->rport = clientport;
  s->sstate = CONNECTED;
  // client socket
  s_client = getemptysocket();
  if(s_client == 0) return -1;
  s_client->lport = clientport;
  s_client->rport = rport;
  s_client->owner = myproc()->pid;
  strncpy(s_client->buf, "", 128);
  s_client->isAvailableData = 0;
  s_client->sstate = CONNECTED;
  wakeup(s);
  release(&stable.lock);
  return clientport;
}

int
send(int lport, const char* data, int n) {
  //
  // TODO: Put the actual implementation of send here.
  //
  struct sock *s;
  struct sock *s_client;
  int rport;
//  char *sp;

  acquire(&stable.lock);

  for(s = stable.sock; s < &stable.sock[NSOCK]; s++)
    if(s->sstate == CONNECTED && s->lport == lport)
      goto found;

  release(&stable.lock);
  return -1;

found:
  rport = s->rport;
  s_client = getsocket(rport);
  if(s_client == 0) return -1;
  if(strlen(s_client->buf) != 0){
    sleep(s_client, &stable.lock);
    strncpy(s_client->buf, data, n);
  }
  else{
    strncpy(s_client->buf, data, n);
  }
  release(&stable.lock);
  return 0;
}


int
recv(int lport, char* data, int n) {
  //
  // TODO: Put the actual implementation of recv here.
  //
   struct sock *s;
//  char *sp;

  acquire(&stable.lock);

  for(s = stable.sock; s < &stable.sock[NSOCK]; s++)
    if(s->sstate == CONNECTED && s->lport == lport)
      goto found;

  release(&stable.lock);
  return -1;

found:
  if(strlen(s->buf) != 0){
    strncpy(data, s->buf, n);
    strncpy(s->buf, "", n);
    wakeup(s);
  }
  else{
    strncpy(data, "", n);
  }
  release(&stable.lock);
  return 0;
}

int
disconnect(int lport) {
  //
  // TODO: Put the actual implementation of disconnect here.
  //
  struct sock *s;

  acquire(&stable.lock);
  s = getsocket(lport);
  s->sstate = CLOSED;
  release(&stable.lock);

  return 0;
}
