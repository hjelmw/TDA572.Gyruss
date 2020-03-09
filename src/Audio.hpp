#pragma once

//////////////////////////////////////////////////////////////
//						Audio
// \brief Plays background and sound effects during gameplay
//
//	
// \see AvancezLib
/////////////////////////////////////////////////////////////



#include "SDL_mixer.h"

class Audio {
private:
	Mix_Music* backgroundMusic;
	Mix_Chunk* soundEffect;
	
	const char* filename;
	int chunk;
	bool type;

public:

	// Creates new audio instance
	Audio(const char* filename, bool type);	
	
	// Destroys the audio instance
	void Destroy();

	// Starts playing audio
	void Play(int loops);
	// Halts all music
	void Stop();
};
