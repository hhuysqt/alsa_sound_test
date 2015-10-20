#include <stdio.h>
#include <math.h>

#define PI 3.14159265358979

#define SOUND_SPEED 340
#define HEAD_WIDTH 0.15
#define POW_NUMBER 1.2

#define WAVE_PEAK  (double)(1L<<15)
// 44.1kbps
#define WAVE_SAMP 44100

#define WAVE_OMEGA (PI*2*wave_freq)
#define WAVE_SCALE ((double)1/WAVE_SAMP)
#define WAVE_DIFF (PI*0.00000003)

int main(int argc, char** argv)
{
		int i;
		int wave_freq;
		int sign = 0;
		double time;
		double wave_diff;
		double phace_top;
		double wave_scale;
		int wave_scale_i;

		if(argc != 4)
		{
				printf("Usage: ./.test xxx 0.xxxxxx [0~15]\n");
				return -1;
		}
		sscanf(argv[1], "%d", &wave_freq);
		sscanf(argv[2], "%lf", &wave_diff);
		sscanf(argv[3], "%d", &wave_scale_i);
		
		wave_scale = (double)(1L << wave_scale_i);

		time = 0;
		i = 0;
		sign = 0;
		phace_top = PI * HEAD_WIDTH / ((double)SOUND_SPEED/ 2 /wave_freq);
		while(1)
		{
				short result;
				double phace_diff;

				i++;
				if(i > 3)
				{
					i = 0;
					if(sign == 0)
					  phace_diff += wave_diff * PI;
					else
					  phace_diff -= wave_diff * PI;
				}

				if(phace_diff > phace_top)
				{
					phace_diff -= wave_diff * PI;
					sign = 1;
				}
				else if(phace_diff < -phace_top)
				{
					phace_diff += wave_diff * PI;
					sign = 0;
				}

				result = (int)(wave_scale * sin(time * WAVE_OMEGA + phace_diff));
				result *= pow(POW_NUMBER, phace_diff);
				write(1, &result, sizeof(short));
				result = (int)(wave_scale * sin(time * WAVE_OMEGA));
				result *= pow(POW_NUMBER, -phace_diff);
				write(1, &result, sizeof(short));
				
				time += WAVE_SCALE;
		}
		return 0;
}

