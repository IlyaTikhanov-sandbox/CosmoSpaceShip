#include "Sounds.h"

class SoundHandler;

SoundHandler::SoundHandler()
{
}

SoundHandler::~SoundHandler()
{
	Mix_FreeMusic(backgroundMusic);
	Mix_FreeChunk(leftAttackEffect);
	Mix_FreeChunk(rightAttackEffect);
	Mix_FreeChunk(upAttackEffect);
	Mix_FreeChunk(downAttackEffect);

}

void SoundHandler::playSound(soundType sound)
{
	//change sounds depending on input since we already loaded all sounds(maybe) , so not to load every time
	//better to change it to enum and switch
	switch (sound)
	{
	case SoundHandler::soundLeftAttack:
		Mix_PlayChannel(0, leftAttackEffect, 0);
		break;
	case SoundHandler::soundRightAttack:
		Mix_PlayChannel(1, rightAttackEffect, 0);
		break;
	case SoundHandler::soundUpAttack:
		Mix_PlayChannel(2, upAttackEffect, 0);
		break;
	case SoundHandler::soundDownAttack:
		Mix_PlayChannel(3, downAttackEffect, 0);
		break;
	case SoundHandler::soundEnemyAttack:
		Mix_PlayChannel(4, enemy_attack, 0);
		break;
	case SoundHandler::soundExplosive:
		Mix_PlayChannel(5, explosive_sound,0);
	default:
		break;
	}
}

void SoundHandler::changeBackgroundSound(SoundHandler::MusicType mtype)
{
	//if (CurrentBackground)
	//	Mix_FreeMusic(CurrentBackground);

	//Mix_GetSynchroValue
	//Mix_GetMusicP
	//Mix_SetMusicPosition();

	//Uint32 pos

	//Mix_GetMusicPositio



	switch (mtype)
	{
	case SoundHandler::Boring:
		CurrentBackgroundChunk = backgroundBoringChunk;
		break;
	case SoundHandler::Rock:
		CurrentBackgroundChunk = backgroundRockChunk;
		break;
	default:
		break;
	}
	//Mix_PlayChannel(0, CurrentBackgroundChunk, -1);
	Mix_PlayMusic(CurrentBackground, -1);
	
	/*
	switch (mtype)
	{
	case SoundHandler::Boring:
		CurrentBackground = backgroundBoringMusic;
		break;
	case SoundHandler::Rock:
		CurrentBackground = backgroundRockMusic;
		break;
	default:
		break;
	}
	Mix_PlayMusic(CurrentBackground, -1);
	*/
}

void SoundHandler::init()
{

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	backgroundMusic = Mix_LoadMUS("assets/space_theme.wav");
	/*
	backgroundRockMusic = Mix_LoadMUS("assets/theme_rock.wav");

	backgroundBoringChunk = Mix_LoadWAV("assets/theme_boring.wav");
	backgroundRockChunk = Mix_LoadWAV("assets/theme_rock.wav");

	leftAttackEffect = Mix_LoadWAV("assets/attack_left.wav");
	rightAttackEffect = Mix_LoadWAV("assets/attack_right.wav");

	downAttackEffect = Mix_LoadWAV("assets/attack_down.wav");
	*/

	explosive_sound = Mix_LoadWAV("assets/explosive.wav");
	upAttackEffect = Mix_LoadWAV("assets/attack.wav");
	enemy_attack = Mix_LoadWAV("assets/enemy_attack.wav");
	Mix_AllocateChannels(8);


	Mix_Volume(2, 80);
	Mix_Volume(4, 70);
	Mix_Volume(5, 128);

	Mix_VolumeMusic(40);

	CurrentBackground = backgroundMusic;


	//Mix_PlayChannel(0, CurrentBackgroundChunk, -1);
	Mix_PlayMusic(CurrentBackground, -1);
}

void SoundHandler::handleEvents()
{
	if (delay != 0)
		--delay;
}

void MyAudioCallback(void* userdata, Uint8* stream, int streamLength)
{
	AudioData* audio = (AudioData*)userdata;

	if (audio->length == 0)
	{
		return;
	}

	Uint32 length = (Uint32)streamLength;
	length = (length > audio->length ? audio->length : length);

	SDL_memcpy(stream, audio->pos, length);

	audio->pos += length;
	audio->length -= length;
}
