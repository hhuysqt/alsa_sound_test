#include <alsa/asoundlib.h>
#include <iostream>

using namespace std;

int main()
{
	int rc;
	snd_pcm_t				*handle;
	snd_pcm_hw_params_t		*params;
	snd_pcm_uframes_t		frames;
	unsigned int			val1, val2;
	int						dir;

	//open device
	if((rc = snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, 0)) < 0)
	{
		cerr << "unable to open pcm devices..." << snd_strerror(rc) <<endl;
		return(-1);
	}

	//settings
	snd_pcm_hw_params_alloca(&params);
	snd_pcm_hw_params_any(handle, params);
	snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
	snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE);
	snd_pcm_hw_params_set_channels(handle, params, 2);
	val1 = 44100;
	snd_pcm_hw_params_set_rate_near(handle, params, &val1, &dir);
	frames = 32;
	snd_pcm_hw_params_set_period_size_near(handle, params, &frame, &dir);
	if((rc = snd_pcm_hw_params(handle, params)) < 0)
	{
		cerr << "Unable to set hw parameters..." << endl;
		return(-2);
	}

	//print informations
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
	cout << "period time: " << val1 << endl;

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

	snd_pcm_close(handle);

	return 0;
}
