// Copyright Yooshley


#include "AbilitySystem/Data/LevelInfo.h"

int32 ULevelInfo::FindLevelForXP(const int32 XP) const
{
	int32 Level = 1;
	bool bSearching = true;
	while (bSearching)
	{
		if (LevelInformation.Num() - 1 <= Level) return Level; //Level 0 is meaningless

		if (XP >= LevelInformation[Level].XPRequirement)
		{
			++Level;
		}
		else
		{
			bSearching = false;
		}
 	}
	return Level;
}
