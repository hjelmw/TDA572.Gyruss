
//////////////////////////////////////////////////////////////
//						Audio
// \brief Plays background and sound effects during gameplay
//
//	
// \see AvancezLib
/////////////////////////////////////////////////////////////



#include "Audio.hpp"

Audio::Audio(const char* filename, bool type)
{
	this->type = type;

	if (type)
		this->backgroundMusic = Mix_LoadMUS(filename);
	else
		this->soundEffect = Mix_LoadWAV(filename);
}


void Audio::Play(int loops)
{
	// 128 / 12 => about 10% volume
	Mix_VolumeMusic(12);
	Mix_Volume(-1, 12); 
	if (!Mix_PlayingMusic())
	{
		if (this->type && !Mix_PlayingMusic())
			Mix_PlayMusic(this->backgroundMusic, loops); // Play background music
		else
			Mix_PlayChannel(-1, this->soundEffect, loops); // Play sound effect on first available channel
	}
}


void Audio::Stop()
{
	if (Mix_PlayingMusic())
		Mix_HaltMusic();
}


void Audio::Destroy()
{
	Mix_CloseAudio();
	Mix_FreeChunk(this->soundEffect);
	Mix_FreeMusic(this->backgroundMusic);
}
