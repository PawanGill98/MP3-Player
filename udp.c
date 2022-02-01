#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>			
#include <unistd.h>			
#include <time.h>
#include <pthread.h>
#include <stdbool.h>
#include "audioMixer_template.h"


#define MSG_MAX_LEN 999999
#define PORT        12345


static pthread_t udpID;

int createSocket() {
	// Address
	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;                   // Connection may be from network
	sin.sin_addr.s_addr = htonl(INADDR_ANY);    // Host to Network long
	sin.sin_port = htons(PORT);                 // Host to Network short
	
	// Create the socket for UDP
	int socketDescriptor = socket(PF_INET, SOCK_DGRAM, 0);

	// Bind the socket to the port (PORT) that we specify
	bind (socketDescriptor, (struct sockaddr*) &sin, sizeof(sin));
	return socketDescriptor;
}

bool udp = true;

void UDP_join(void){
    pthread_join(udpID,NULL);
}

void UDP_end(void){
    udp = false;
}

static void* udpThread(void *arg){
    int socketDescriptor = createSocket();
    printf("Net Listen Test on UDP port %d:\n", PORT);
	printf("Connect using: \n");
	printf("    netcat -u 192.168.7.2 %d\n", PORT);
	
    while (udp) {		
		// Get the data (blocking)
		// Will change sin (the address) to be the address of the client.
		// Note: sin passes information in and out of call!
		struct sockaddr_in sinRemote;
		unsigned int sin_len = sizeof(sinRemote);
		char messageRx[MSG_MAX_LEN];
		int bytesRx = recvfrom(socketDescriptor, messageRx, MSG_MAX_LEN, 0, (struct sockaddr *) &sinRemote, &sin_len);

		// Make it null terminated (so string functions work):
		int terminateIdx = (bytesRx < MSG_MAX_LEN) ? bytesRx : MSG_MAX_LEN - 1;
		messageRx[terminateIdx] = 0;

		printf("Message received (%d bytes): \n\n'%s'\n", bytesRx, messageRx);

		// Compose the reply message:
		// (NOTE: watch for buffer overflows!).
		
		char *helpCommand = "Accepted command examples:\n count      -- display number arrays sorted.\n get length -- display length of array currently being sorted.\n get array  -- display the full array being sorted.\n get 10     -- display the tenth element of array currently being sorted.\n stop       -- cause the server program to end.";
		//char *stopCommand = "Program Terminating";
		//char *countCommand = "Number of arrays sorted = ";
		//char* errorCommand = "Command not recognized! Use command 'help' for list of commands";
        //char* lengthCommand = "Current array length = ";
        //char* arrCommand = "WILL PRINT ARRAY HERE";

		char messageTx[MSG_MAX_LEN];

		if(strncmp(messageRx, "noBeat", strlen("noBeat")) == 0){
			AudioMixer_getState(0);
		}
		else if(strncmp(messageRx, "rockBeat1", strlen("rockBeat1")) == 0){	
			AudioMixer_getState(1);
		}
		else if(strncmp(messageRx, "rockBeat2", strlen("rockBeat2")) == 0){
			AudioMixer_getState(2);
		}
		else if(strncmp(messageRx, "volUp", strlen("VolUp")) == 0){
			AudioMixer_setVolume(AudioMixer_getVolume() + 5);
			//sprintf(messageTx, "%s%d\n", lengthCommand, val);
		}
		else if(strncmp(messageRx, "volDown", strlen("volDown")) == 0){
			AudioMixer_setVolume(AudioMixer_getVolume() - 5);
		}
		else if(strncmp(messageRx, "tempoUp", strlen("tempoUp")) == 0){
			AudioMixer_setBPM(AudioMixer_getBPM() + 5);
		}
		else if(strncmp(messageRx, "tempoDown", strlen("tempoDown")) == 0){
			AudioMixer_setBPM(AudioMixer_getBPM() - 5);
		}
		else{
			sprintf(messageTx, "%s\n", errorCommand);
		}
		sin_len = sizeof(sinRemote);
		sendto( socketDescriptor, messageTx, strlen(messageTx), 0, (struct sockaddr *) &sinRemote, sin_len);
		printf(messageTx, "\n");

		

	}
    // Close
	close(socketDescriptor);
	
	return 0;

}

void UDP_start(void){
    pthread_create(&udpID,NULL,*udpThread,NULL);
}