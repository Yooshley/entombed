// Copyright Yooshley


#include "AbilitySystem/Data/ArchetypeInfo.h"

FEntombedArchetypeDefaultInfo UArchetypeInfo::GetArchetypeDefaultInfo(EEntombedArchetype Archetype)
{
	return ArchetypeInformation.FindChecked(Archetype);
}
