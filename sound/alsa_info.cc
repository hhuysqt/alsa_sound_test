#include <alsa/asoundlib.h>
#include <iostream>

using namespace std;

int main()
{
	cout << "\nALSA library version" << SND_LIB_VERSION_STR << endl;
	cout << "\nPCM stream types:" << endl;
	for(int val = 0; val <= SND_PCM_STREAM_LAST; val++)
		cout << "\t" << snd_pcm_stream_name((snd_pcm_stream_t)val) << endl;

	cout << "\nPCM access types:" << endl;
	for(int val = 0; val <= SND_PCM_ACCESS_LAST; val++)
		cout << "\t" << snd_pcm_access_name((snd_pcm_access_t)val) << endl;

	cout << "\nPCM subformats:" << endl;
	for(int val = 0; val <= SND_PCM_SUBFORMAT_LAST; val++)
		cout << "\t" << 
			snd_pcm_subformat_name((snd_pcm_subformat_t)val) << 
			snd_pcm_subformat_description((snd_pcm_subformat_t)val)
			<< endl;

	cout << "\nPCM states:" <<endl;
	for(int val = 0; val <= SND_PCM_START_LAST; val++)
		cout << "\t" << snd_pcm_state_name((snd_pcm_state_t)val) << endl;

	cout << "\nPCM formats:" <<endl;
	for(int val = 0; val <= SND_PCM_FORMAT_LAST; val++)
		cout << "\t" << snd_pcm_format_name((snd_pcm_format_t)val) << endl;

	cout << "\nEND." << endl;

	return 0;
}

