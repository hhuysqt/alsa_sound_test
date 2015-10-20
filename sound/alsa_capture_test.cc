#include <alsa/asoundlib.h>
#include <iostream>

using namespace std;

class MySound
{
		private:
				snd_pcm_t             *handle;
				snd_pcm_hw_params_t   *params;
				snd_pcm_uframes_t     frames;

				int dir;
				char*buffer;

		public:
				int init(void);
				int capture(FILE* fp, int seconds);
				int capture(char* buff, int seconds);
				void close();
};

int MySound::init(void)
{
		int rc;

		rc = snd_pcm_open(&this->handle, "default", SND_PCM_STREAM_CAPTURE, 0);
		if(rc < 0)
		{
				cerr << "Unable to open pcm device... " << snd_strerror(rc) << endl;
				return -1;
		}
		else
				cout << "opened audio device!" << endl;

		// allocate a hardware parameters object
		snd_pcm_hw_params_alloca(&this->params);

		// fill it in with default values
		snd_pcm_hw_params_any(this->handle, this->params);

		// And then set the desired hardware parameters
		// INTERLEVELED mode
		snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
		// signed 16-bit little-endian format
		snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE);
		// 2 channels stereo output
		snd_pcm_hw_params_set_channels(handle, params, 2);
		// 44100 sample rates
		unsigned int val = 44100;
		snd_pcm_hw_params_set_rate_near(handle, params, &val, &dir);
		// set period size to 32 frames
		frames = 32;
		snd_pcm_hw_params_set_period_size_near(handle, params, &frames, &dir);

		// Write them to the driver
		rc = snd_pcm_hw_params(handle, params);
		if(rc < 0)
		{
				cerr << "Failed to set parameters... " << snd_strerror(rc) << endl;
				return -2;
		}
		else
				cout << "Set parameters well!" << endl;

		return 0;
}

		unsigned int period_num;
int MySound::capture(FILE*fp, int seconds)
{
		// malloc a buffer large enough to hold data of one period
		snd_pcm_hw_params_get_period_size(params, &frames, &dir);
		int size = static_cast<int>(frames) * 2*2; /* 2 channels * 2 bytes */
		cout << "malloc size: " << size << endl;

		buffer = (char*)malloc(size);
		if(buffer == NULL)
		{
				cerr << "malloc failed..." << endl;
				return -1;
		}

		// calculate periods
		snd_pcm_hw_params_get_period_time(params, &period_num, &dir);
		cout << "period time: " << period_num << endl;

		//now start to capture
		long loops = seconds * 1000000 / period_num;
		int counter = 0;
		while(loops >= 0)
		{
				loops--;

				int rc = snd_pcm_readi(handle, buffer, frames);
				cout << counter++ << endl;

				if(rc == -EPIPE)
				{
						// overrun
						cerr << "Overrun occurred" << endl;
						snd_pcm_prepare(handle);
				}
				else if(rc < 0)
				{
						cerr << "Read error" << snd_strerror(rc) << endl;
				}
				else if(rc != (int)frames)
				{
						cerr << "Only read " << rc << " bytes, not expected" << endl;
				}

				// write it to file
				rc = fwrite(buffer, size, 1, fp);
				if(rc != size)
						cerr << "Write error: " << rc << endl;
				else
						cout << "write ok" << endl;
		}

		free(buffer);
		return 0;
}

int MySound::capture(char *buff, int seconds)
{
		return 0;
}

int main()
{
		MySound sound_cp;
		FILE *fp = fopen("sound.wav", "w");
		if(fp == NULL)
		{
				cerr << "Failed to open file" << endl;
				return -1;
		}

		if(sound_cp.init() < 0)
		{
				cerr << "Init error..." << endl;
				return -2;
		}

		if(sound_cp.capture(fp, 5) < 0)
		{
				cerr << "Capture error" << endl;
				return -3;
		}

		cout << "OK" << endl;
		fclose(fp);

		return 0;
}

