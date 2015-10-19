#include <alsa/asoundlib.h>
#include <iostream>

using namespace std;

int main()
{
	int rc;
	snd_pcm_t				*handle;
	snd_pcm_hw_params_t		*params;
	unsigned int			val1, val2;
	int						dir;
	snd_pcm_uframes_t		*frames;

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

	snd_pcm_close(handle);

	return 0;
}
