#pragma once
#include "SDL.h"
#include "SDL_mixer.h"
#include <iostream>

#define FILE_PATH "assets/theme.wav"
#define ROCK_FILE_PATH "assets/theme_rock.wav"
#define ATTACK_FILE_PATH "assets/attack.wav"

struct AudioData
{
	Uint8* pos;
	Uint32 length;
	SDL_AudioSpec Spec;
};

class SoundHandler
{
public:
	Mix_Music *backgroundMusic;
	Mix_Music *backgroundRockMusic;
	Mix_Music *CurrentBackground;

	Mix_Chunk *backgroundBoringChunk;
	Mix_Chunk *backgroundRockChunk;
	Mix_Chunk *CurrentBackgroundChunk;

	Mix_Chunk *enemy_attack;
	Mix_Chunk *explosive_sound;



	Mix_Chunk *leftAttackEffect;
	Mix_Chunk *rightAttackEffect;
	Mix_Chunk *upAttackEffect;
	Mix_Chunk *downAttackEffect;

	int delay = 0;

	enum MusicType : std::size_t
	{
		Boring,
		Rock
	};

	enum soundType : std::size_t
	{
		soundPlaceholder,
		soundLeftAttack,
		soundRightAttack,
		soundUpAttack,
		soundDownAttack,
		soundEnemyAttack,
		soundExplosive
	};
	
	SoundHandler();
	~SoundHandler();

	void playSound(soundType sound);
	void changeBackgroundSound(SoundHandler::MusicType mtype);

	void init();

	void handleEvents();

	friend void MyAudioCallback(void* userdata, Uint8* stream, int streamLength);
};

