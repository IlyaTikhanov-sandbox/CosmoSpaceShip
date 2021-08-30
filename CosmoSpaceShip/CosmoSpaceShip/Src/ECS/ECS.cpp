#include "ECS.h"

void Entity::addGroup(Group mGroup)
{
	groupBitset[mGroup] = true;
	manager.AddToGroup(this, mGroup);
}

void Entity::changeGroupTo(Group group)
{
	groupBitset.reset();
	addGroup(group);
}
