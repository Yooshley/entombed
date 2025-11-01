// Copyright Yooshley


#include "AbilitySystem/Data/AttributeInfo.h"

FEntombedAttributeInfo UAttributeInfo::FindAttributeInfoByTag(const FGameplayTag& AttributeTag,
                                                              bool bLogNotFound) const
{
	for (const FEntombedAttributeInfo& Info : AttributeInformation)
	{
		if (Info.AttributeTag.MatchesTagExact(AttributeTag))
		{
			return Info;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("AttributeTag [%s] Tag not found in AttributeInfo [%s]"), *AttributeTag.ToString(), *GetNameSafe(this));
	}
	
	return FEntombedAttributeInfo();
}
