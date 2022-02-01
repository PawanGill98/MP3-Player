/*#ifndef BEATS_H
#define BEATS_H

typedef struct {
	int numSamples;
	short *pData;
} wavedata_t;

#define AUDIOMIXER_MAX_VOLUME 100

// init() must be called before any other functions,
// cleanup() must be called last to stop playback threads and free memory.
void AudioMixer_init(void);
void AudioMixer_cleanup(void);

// Read the contents of a wave file into the pSound structure. Note that
// the pData pointer in this structure will be dynamically allocated in
// readWaveFileIntoMemory(), and is freed by calling freeWaveFileData().
void AudioMixer_readWaveFileIntoMemory(char *fileName, wavedata_t *pSound);
void AudioMixer_freeWaveFileData(wavedata_t *pSound);

// Queue up another sound bite to play as soon as possible.
void AudioMixer_queueSound(wavedata_t *pSound);

// Get/set the volume.
// setVolume() function posted by StackOverflow user "trenki" at:
// http://stackoverflow.com/questions/6787318/set-alsa-master-volume-from-c-code
int  AudioMixer_getVolume();
void AudioMixer_setVolume(int newVolume);

#endif*/

#ifndef BEATGEN_H
#define BEATGEN_H

#define BASEDRUM "wave-files/100051__menegass__gui-drum-bd-hard.wav"
#define HIHAT "wave-files/100053__menegass__gui-drum-cc.wav"
#define SNARE "wave-files/100059__menegass__gui-drum-snare-soft.wav"
#define DEFAULT_BPM 120

void beatInit();
long halfBeat(int bpm);

void noDrumBeat();
void standardRockDrumBeat();
void otherDrumBeat();

void* beatThread(void* arg);
void beatFree();

#endif
