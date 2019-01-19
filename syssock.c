#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "proc.h"

int
sys_listen(void)
{
  int port;

  //
  // TODO: Write your code to get and validate port no.
  //

  if(argint(0, &port) < 0)
    return -1;

  return listen(port);
}

int
sys_connect(void)
{
  int port;
  char *host;

  //
  // TODO: Write your code to get and validate port no., host.
  // Allow connection to "localhost" or "127.0.0.1" host only
  //

  if(argint(0, &port) < 0 || argstr(1, &host) < 0)
    return -1;
  else if(port < 0 || port > NPORT || strncmp(host, "localhost", strlen("localhost")) != 0)
    return -1;

  return connect(port, host);
}

int
sys_send(void)
{
  int port;
  char* buf ;
  int n;

  if(argint(0, &port) < 0 || argstr(1, &buf) < 0 || argint(2, &n))
    return -1;
  //
  // TODO: Write your code to get and validate port no., buffer and buffer size
  //

  return send(port, buf, n);
}

int
sys_recv(void)
{
  int port;
  char* buf;
  int n;

  //
  // TODO: Write your code to get and validate port no., buffer and buffer size
  //

  if(argint(0, &port) < 0 || argstr(1, &buf) < 0 || argint(2, &n))
    return -1;

  return recv(port, buf, n);
}

int
sys_disconnect(void)
{
  int port;

  if(argint(0, &port) < 0)
    return -1;
  //
  // TODO: Write your code to get and validate port no.
  //

  return disconnect(port);
}
