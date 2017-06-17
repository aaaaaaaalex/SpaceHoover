#include "SFX_Module.h"

SFX_Module::SFX_Module()
{
	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
	loadAudio();
}
SFX_Module::~SFX_Module()
{
	clearQueue();
	Mix_FreeChunk(wisp);
	Mix_FreeChunk(collision);
	for (int i = 0; i < 4; i++) Mix_FreeChunk(twinkles[i]);
}

void SFX_Module::processQueue()
{
	// NO VOLUME FUNCTIONALITY ADDED YET
	CustomChunkQueuer* c;
	for (int i = 0; i < 5; i++) {
		if (playback_queue[i] != NULL) {
			c = playback_queue[i];
			switch (c->type) {
			case TWINKLE:
				if (twinkle_type > 3) twinkle_type = 0;
				Mix_PlayChannelTimed(-1, twinkles[twinkle_type], 0, i*40);
				twinkle_type++;
				break;
			case COLLISION:
				// given its own priority channel, so that other sounds dont overload it
				Mix_PlayChannel(5, collision, 0);
				break;
			case WISP:
				Mix_PlayChannel(-1, wisp, 0);
				break;
			}
		}
	}
	clearQueue();
}

void SFX_Module::addToQueue(CustomChunkType chunk_type, int volume_percent)
{
	int twinkle_counter = 0;
	int collision_counter = 0;
	int wisp_counter = 0;

	for (int i = 0; i < 5; i++) {
		if (playback_queue[i] == NULL) {
			CustomChunkQueuer* c = new CustomChunkQueuer;
			c->type = chunk_type;

			// if volume is not in range 0-100, assign a default val
			if (volume_percent >= 0 && volume_percent <= 100) {
				c->volume = volume_percent;
			}
			else c->volume = 50;

			// check if addition to queue is permitted
			bool permit = true;
			switch (chunk_type) {
			case TWINKLE:
				if (twinkle_counter > 2) permit = false;
				break;
			case COLLISION:
				if (collision_counter > 1) permit = false;
				break;
			case WISP:
				if (wisp_counter > 2) permit = false;
				break;
			}
			if (permit) playback_queue[i] = c;
		}
		else {
			// keep count of how many duplicate types are in the queue
			// a maximum number of the same amount of chunks can be played at once
			switch (playback_queue[i]->type) {
			case TWINKLE:
				twinkle_counter++;
				break;
			case COLLISION:
				collision_counter++;
				break;
			case WISP:
				wisp_counter++;
				break;
			}
		}
	}
}

void SFX_Module::clearQueue()
{
	for (int i = 0; i < 5; i++) {
		if (playback_queue[i] != NULL) {
			delete playback_queue[i];
			playback_queue[i] = NULL;
		}
	}
}

void SFX_Module::loadAudio()
{
	this->wisp = Mix_LoadWAV("./assets/audio/star/wisp.wav");
 	this->collision = Mix_LoadWAV("./assets/audio/collision.wav");
	std::string twinkle_basename = "./assets/audio/star/twinkle";
	for (int i = 0; i < 4; i++) {
		std::string filename = twinkle_basename + std::to_string(i+1) + ".wav";
		this->twinkles[i] = Mix_LoadWAV(filename.c_str());
		//printf("error? : %s\n", Mix_GetError());
	}

	this->soundtrack = Mix_LoadMUS("./assets/audio/soundtrack.wav");
}

