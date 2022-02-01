// sorter.h
// Module to create a socket so that we can communicate to the sorter.c program
//over a udp connection

#ifndef _UDP_
#define _UDP_

int createSocket();
void UDP_start(void);
void UDP_end(void);
void UDP_join(void);

#endif


/*#ifndef _UDP_LISTENER_H_
#define _UDP_LISTENER_H_

// Start listening to the UDP port on a separate thread.
void UdpListener_startListening(void);

void UdpListener_cleanup(void);

#endif*/
