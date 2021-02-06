#pragma once

#include "Components.h"
#include "../AssetManager.h"
#include "Weapons.h"
#include "../Game.h"

class FigthComponent : public Component
{
public:

	FigthComponent(AssetManager* assets_obj, Weapon* weapon,Game::groupLabels pr_type):m_assets(assets_obj),currentWeapon(weapon)
	{
		delay = currentWeapon->getDelay(AttackType::UsualAttack);
		//delay = 60;
		projectile_gpoup = pr_type;
	}
	void MakeShot(AttackType type, Vector2D  direction, SoundHandler::soundType sound)
	{
		std::string projTexture = currentWeapon->getTextureID(type);
		int texWidth = m_assets->GetTextureWidth(projTexture);
		
		if (type == UsualAttack)
		{
			Vector2D leftPos = entity->getComponent<TransformComponent>().getLeftAttackPos(texWidth * currentWeapon->getScale(type));
			Vector2D rightPos = entity->getComponent<TransformComponent>().getRightAttackPos(texWidth * currentWeapon->getScale(type));
			m_assets->CreateProjectile(leftPos, texWidth, direction, currentWeapon->getRange(type), currentWeapon->getSpeed(type),
				currentWeapon->getDamage(type) / 2, projTexture, currentWeapon->isAnim(type), false, currentWeapon->getScale(type), projectile_gpoup,currentWeapon->getProjType(type));

			m_assets->CreateProjectile(rightPos, texWidth, direction, currentWeapon->getRange(type), currentWeapon->getSpeed(type),
				currentWeapon->getDamage(type) / 2, projTexture, currentWeapon->isAnim(type), true, currentWeapon->getScale(type), projectile_gpoup, currentWeapon->getProjType(type));

			currentWeapon->WeaponSound(sound);
		}
		if (type == SpecialAttack)
		{
			if (center_cooldown == 0)
			{
				center_cooldown = delay * 4;
				Vector2D centralPos = entity->getComponent<TransformComponent>().getCentralPosTOP(texWidth * currentWeapon->getScale(type));

				m_assets->CreateProjectile(centralPos, texWidth, direction, currentWeapon->getRange(type), currentWeapon->getSpeed(type),
					currentWeapon->getDamage(type), projTexture, currentWeapon->isAnim(type), false, currentWeapon->getScale(type), projectile_gpoup, currentWeapon->getProjType(type));
				currentWeapon->WeaponSound(sound);
			}
		}
		if (type == RicochetAttack)
		{
			if (ricochet_cooldown == 0)
			{
				ricochet_cooldown = delay * 2;
				Vector2D leftPos = entity->getComponent<TransformComponent>().getLeftAttackPos(texWidth * currentWeapon->getScale(type));
				Vector2D rightPos = entity->getComponent<TransformComponent>().getRightAttackPos(texWidth * currentWeapon->getScale(type));

				m_assets->CreateProjectile(leftPos, texWidth, Vector2D(-1, 1), currentWeapon->getRange(type), currentWeapon->getSpeed(type),
					currentWeapon->getDamage(type) / 2, projTexture, currentWeapon->isAnim(type), false, currentWeapon->getScale(type), projectile_gpoup, currentWeapon->getProjType(type));
				
				m_assets->CreateProjectile(rightPos, texWidth, Vector2D(1, 1), currentWeapon->getRange(type), currentWeapon->getSpeed(type),
					currentWeapon->getDamage(type) / 2, projTexture, currentWeapon->isAnim(type), false, currentWeapon->getScale(type), projectile_gpoup, currentWeapon->getProjType(type));
				
				m_assets->CreateProjectile(leftPos, texWidth, Vector2D(-1, -1), currentWeapon->getRange(type), currentWeapon->getSpeed(type),
					currentWeapon->getDamage(type) / 2, projTexture, currentWeapon->isAnim(type), false, currentWeapon->getScale(type), projectile_gpoup, currentWeapon->getProjType(type));

				m_assets->CreateProjectile(rightPos, texWidth, Vector2D(1, -1), currentWeapon->getRange(type), currentWeapon->getSpeed(type),
					currentWeapon->getDamage(type) / 2, projTexture, currentWeapon->isAnim(type), false, currentWeapon->getScale(type), projectile_gpoup, currentWeapon->getProjType(type));

				currentWeapon->WeaponSound(sound);
			}
		}
	}
	void Attack(Vector2D position, Vector2D direction, SoundHandler::soundType sound)
	{
		if (cooldown == 0)
		{
			cooldown = delay;

			for (int i = 0; i < maxAttacks; ++i)
			{
				if (currentWeapon->attackEnabled[i])
				{
					MakeShot(static_cast<AttackType>(i), direction, sound);
				}
			}
		}
	}
	void TargetAttack(Vector2D SourcePosition, Vector2D TargetPosition, SoundHandler::soundType sound)
	{
		/*
		std::string projTexture = currentWeapon->getTextureID();
		int texWidth = m_assets->GetTextureWidth(projTexture);
		Vector2D direction(TargetPosition.x - SourcePosition.x, TargetPosition.y - SourcePosition.y);

		if (cooldown == 0)
		{
			cooldown = delay;
			if (currentWeapon->isRotatable())
			{
				if (direction.y != 0)
					projTexture += "_90";
			}
			m_assets->CreateProjectile(SourcePosition, texWidth, direction.GetNormedVector(), currentWeapon->getRange(), currentWeapon->getSpeed(),
				currentWeapon->getDamage(), projTexture, currentWeapon->isAnim(), false, currentWeapon->getScale(), projectile_gpoup);
			currentWeapon->WeaponSound(sound);
		}
		*/
	}

	void init()
	{
		cooldown = delay;
		center_cooldown = delay * 4;
		ricochet_cooldown = delay * 2;
	}
	void update() override
	{
		if (cooldown != 0)
			--cooldown;
		if (center_cooldown != 0)
			--center_cooldown;
		if (ricochet_cooldown != 0)
			--ricochet_cooldown;
	}

	Weapon* getWeapon() { return currentWeapon; }

protected:
	Game::groupLabels projectile_gpoup;
	int cooldown = 0;
	int center_cooldown = 0;
	int ricochet_cooldown = 0;
	int delay = 30;
	AssetManager* m_assets;
	Weapon* currentWeapon;
};