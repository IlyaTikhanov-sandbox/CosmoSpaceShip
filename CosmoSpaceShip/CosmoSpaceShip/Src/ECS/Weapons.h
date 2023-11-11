#pragma once
#include <string>
#include <bitset>
#include "../Sounds.h"
#include "../Game.h"


constexpr std::size_t maxAttacks = 3;

class Weapon
{
public:
	Weapon() {}

	Weapon(WeaponParams params)
	{
		attacks = params.attacks;
		pSounder = params.pSounder;
		for (auto a : attacks)
		{
			//a.first is AttackType which could be UsualAttack,SpecialAttack..
			attackEnabled[a.first] = true;
			++num_attackEnabled;
		}
	}

	int getRange(AttackType type) { return attacks[type].range; }
	int getSpeed(AttackType type) { return attacks[type].speed; }
	int getDamage(AttackType type) { return attacks[type].damage; }
	int getDelay(AttackType type) { return attacks[type].delay; }
	float getScreenFactor(AttackType type) { return attacks[type].screenFactor; }
	std::string getTextureID(AttackType type) { return attacks[type].textureID; }
	bool isAnim(AttackType type) { return attacks[type].isAnimated; }
	int getScale(AttackType type) { return attacks[type].scale; }
	bool isRotatable(AttackType type) { return attacks[type].rotate; }
	AttackType getAttackType(AttackType type) { return attacks[type].attackType; }
	ProjectileType getProjType(AttackType type) { return attacks[type].projType; }
	SoundHandler::soundType getSound(AttackType type) { return attacks[type].attackSound; }
	SoundHandler getHandler() { return *pSounder; }

	void WeaponSound(SoundHandler::soundType);
	

	std::bitset<maxAttacks> attackEnabled;
	int num_attackEnabled = 0;

private:
	std::map<AttackType, Attack> attacks;
	SoundHandler *pSounder;
	
};