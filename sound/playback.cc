#include <alsa/asoundlib.h>
#include <iostream>
#include "MySound.hpp"

#define WAVEFILENAME "./sound.wav"

int main()
{
		long loops;
		MySound playback;

		if(playback.init(SND_PCM_STREAM_PLAYBACK) < 0)
		{
				cerr << "Init error." << endl;
				return -1;
		}

		FILE *fp;
		fp = fopen(WAVEFILENAME, "r");
		if(fp == NULL)
		{
				cerr << "No such file: " << "WAVEFILENAME" << endl;
				return -2;
		}

		playback.play(fp);

		playback.close();
		fclose(fp);

		return 0;
}
