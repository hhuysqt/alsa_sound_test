#include <stdio.h>
#include <math.h>

#define PI 3.14159265358979

#define WAVE_PEAK  (double)(1L<<12)
// 1000 Hz
#define WAVE_FREQ 300
// 44.1kbps
#define WAVE_SAMP 44100
// It lasts for 5 seconds.
#define WAVE_LAST 10

#define WAVE_OMEGA (PI*2*wave_freq)
#define WAVE_SCALE ((double)1/WAVE_SAMP)
#define WAVE_DIFF (PI*0.00000003)

int main(int argc, char** argv)
{
		long i;
		int wave_freq;
		double wave_scale, time;
		double wave_diff;

		if(argc != 3)
		{
				printf("Usage: ./.test xxx 0.xxxxxx\n");
				return -1;
		}
		sscanf(argv[1], "%d", &wave_freq);
		sscanf(argv[2], "%lf", &wave_diff);
		printf("input: %lf\n", wave_diff);

		time = 0;
		i = 0;
		while(1)
		{
				short result;
				i++;
				result = (int)(WAVE_PEAK * sin((time + wave_diff * i) * WAVE_OMEGA));
				write(1, &result, sizeof(short));
				result = (int)(WAVE_PEAK * sin(time * WAVE_OMEGA));
				write(1, &result, sizeof(short));
				time += WAVE_SCALE;
		}
		return 0;
}

