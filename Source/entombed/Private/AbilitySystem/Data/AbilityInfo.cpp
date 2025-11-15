// Copyright Yooshley


#include "AbilitySystem/Data/AbilityInfo.h"

#include "entombed/EntombedLogChannels.h"

FEntombedAbilityInfo UAbilityInfo::FindAbilityInfoByTag(const FGameplayTag& AbilityTag, bool bLogNotFound)
{
	for (const FEntombedAbilityInfo& AbilityInfo : AbilityInformation)
	{
		if (AbilityInfo.AbilityTag.MatchesTagExact(AbilityTag))
		{
			return AbilityInfo;
		}
	}
	if (bLogNotFound)
	{
		UE_LOG(LogEntombed, Error, TEXT("Ability Info [%s] not found for Tag [%s]"), *GetNameSafe(this), *AbilityTag.ToString());
	}
	return FEntombedAbilityInfo();
}
