#pragma once

#include <SDL_mixer.h>
#include <string>

enum CustomChunkType { TWINKLE, COLLISION, WISP };
struct CustomChunkQueuer {
	CustomChunkType type;
	int volume;
};

class SFX_Module
{
public:
	SFX_Module();
	~SFX_Module();

	void processQueue();
	void addToQueue(CustomChunkType chunk_type, int volume_percent);
	void clearQueue();
private:
	int twinkle_type = 0;
	Mix_Chunk* twinkles[4];
	Mix_Chunk* wisp;
	Mix_Chunk* collision;
	CustomChunkQueuer* playback_queue[5];
	Mix_Music* soundtrack;

	void loadAudio();
};

