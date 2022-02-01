//#include "RusHourBeats.h"
#include "joystick.h"
#include "beats.h"
#include "audioMixer_template.h"
#include "udp.h"
#include <pthread.h>

int main() {
	AudioMixer_init();

//	pthread_t playbackId;
//	pthread_t udpId;
	pthread_t joystickId;
	pthread_t beatId;

	pthread_attr_t attributes;
	pthread_attr_init(&attributes);


	UDP_start();

	pthread_create(&beatId, NULL, beatThread, NULL);
//	pthread_create(&playbackId, NULL, playbackThread, NULL);
//	pthread_create(&udpId, &attributes, netcat, NULL);
	pthread_create(&joystickId, &attributes, joystickThread, NULL);

	pthread_join(beatId, NULL);
//	pthread_join(playbackId, NULL);
//	pthread_join(udpId, NULL);
	pthread_join(joystickId, NULL);


	UDP_end();



	AudioMixer_cleanup();

	return 0;
}