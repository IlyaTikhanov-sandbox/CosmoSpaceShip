#include "Weapons.h"


void Weapon::WeaponSound(SoundHandler::soundType sound)
{
	pSounder->playSound(sound);
}
