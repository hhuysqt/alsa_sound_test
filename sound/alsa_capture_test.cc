#include <alsa/asoundlib.h>
#include <stdio.h>
#include <iostream>

using namespace std;

class MySound
{
		private:
				snd_pcm_t             *handle;
				snd_pcm_hw_params_t   *params;
				snd_pcm_uframes_t     frames;

				unsigned int val1, val2;
				int dir;
				char*buffer;
				int period_time;

		public:
				int init(void);
				void printData(void);
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

		this->printData();

		return 0;
}

void MySound::printData(void)
{
	cout << "\nPCM handle name: " << snd_pcm_name(handle) <<endl;
	cout << "PCM states: " << snd_pcm_state_name(snd_pcm_state(handle)) << endl;

	snd_pcm_hw_params_get_access(params, (snd_pcm_access_t*)&val1);
	cout << "access type: " << 	snd_pcm_access_name((snd_pcm_access_t)val1) << endl;
	
	snd_pcm_hw_params_get_format(params, (snd_pcm_format_t*)&val1);
	cout << "format: " 
			<< snd_pcm_format_name((snd_pcm_format_t)val1) 
			<< " (" 
			<< snd_pcm_format_description((snd_pcm_format_t)val1) 
			<< ")" << endl;

	snd_pcm_hw_params_get_subformat(params, (snd_pcm_subformat_t*)&val1);
	cout << "subformat: "
			<< snd_pcm_subformat_name((snd_pcm_subformat_t)val1)
			<< " ("
			<< snd_pcm_subformat_description((snd_pcm_subformat_t)val1)
			<< ")" << endl;

	snd_pcm_hw_params_get_channels(params, &val1);
	cout << "channel: " << val1 << endl;

	snd_pcm_hw_params_get_rate(params, &val1, &dir);
	cout << "rates: " << val1 << "bps" << endl;

	snd_pcm_hw_params_get_rate_numden(params, &val1, &val2);
	cout << "exact rate: " << val1/val2 << " bps" << endl;

	snd_pcm_hw_params_get_period_time(params, &val1, &dir);
	period_time = val1;
	cout << "period time: " << period_time << endl;

	snd_pcm_hw_params_get_period_size(params, &frames, &dir);
	cout << "period size: " << static_cast<int>(frames) << " frames." << endl;

	snd_pcm_hw_params_get_buffer_time(params, &val1, &dir);
	cout << "buffer time: " << val1 << " us." << endl;

	snd_pcm_hw_params_get_buffer_size(params, (snd_pcm_uframes_t*)&val1);
	cout << "buffer size: " << val1 << " frames." << endl;

	snd_pcm_hw_params_get_periods(params, &val1, &dir);
	cout << "period per buffer: " << val1 << " frames." << endl;

	cout << "significant bits: " << snd_pcm_hw_params_get_sbits(params) << endl;

	snd_pcm_hw_params_get_tick_time(params, &val1, &dir);
	cout << "tick time: " << val1 << " us" << endl;

	cout << "is batch: " << snd_pcm_hw_params_is_batch(params) << endl;

	cout << "is block transfer: " << snd_pcm_hw_params_is_block_transfer(params) << endl;

	cout << "is double: " << snd_pcm_hw_params_is_double(params) << endl;

	cout << "is half duplex: " << snd_pcm_hw_params_is_half_duplex(params) << endl;

	cout << "is joint duplex: " << snd_pcm_hw_params_is_joint_duplex(params) << endl;

	cout << "can overrange: " << snd_pcm_hw_params_can_overrange(params) << endl;

	cout << "can mmap: " << snd_pcm_hw_params_can_mmap_sample_resolution(params) << endl;

	cout << "can pause: " << snd_pcm_hw_params_can_pause(params) << endl;

	cout << "can resume: " << snd_pcm_hw_params_can_resume(params) << endl;

	cout << "can sync start: " << snd_pcm_hw_params_can_sync_start(params) << endl;

	/*
	cout << "press any keys to continue" << endl;

	char dummy;
	cin >> dummy;
	*/
}

int MySound::capture(FILE*fp, int seconds)
{
		// malloc a buffer large enough to hold data of one period
		//snd_pcm_hw_params_get_period_size(params, &frames, &dir);
		int size = static_cast<int>(frames) * 2*2; /* 2 channels * 2 bytes */
		cout << "malloc size: " << size << endl;

		buffer = (char*)malloc(size);
		if(buffer == NULL)
		{
				cerr << "malloc failed..." << endl;
				return -1;
		}

		// calculate periods
		snd_pcm_hw_params_get_period_time(this->params, &val1, &dir);
		cout << "period time: " << val1 << " / " << period_time << endl;

		//now start to capture
		long loops = (long)seconds * 1000000L / val1;
		cout << loops << endl;
		int counter = 0;
		getchar();
		while(loops >= 0)
		{
				loops--;

				int rc = snd_pcm_readi(handle, buffer, frames);
				cout << counter++ ;

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
				/*
				if(rc != size)
						cerr << "Write error: " << rc << endl;
				else
						cout << "write ok" << endl;
						*/
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

		//sound_cp.printData();
		getchar();

		if(sound_cp.capture(fp, 1) < 0)
		{
				cerr << "Capture error" << endl;
				return -3;
		}

		cout << "OK" << endl;
		fflush(fp);
		fclose(fp);

		return 0;
}

