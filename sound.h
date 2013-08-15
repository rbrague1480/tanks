#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include <vorbis/vorbisfile.h>

enum ALBuffers {
	fireSoundBuf,
	blowUpSoundBuf,
	numOfBuf
};

enum ALSources {
	ALtank1,
	ALtank2,
	ALblowUpTank1,
	ALblowUpTank2,
	numOfSources
};

class OpenAL{
	ALCdevice *device;
	ALCcontext *context;
	ALuint *buffers;
	ALuint *sources;
	public:	
		void init(void);
		void play(ALSources);
		int snd_load_file(const char * file, ALuint buffer);
		~OpenAL();
		
};