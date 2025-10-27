// Copyright Yooshley


#include "Input/EntombedInputDataAsset.h"

const UInputAction* UEntombedInputDataAsset::GetInputActionByTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const FEntombedInputAction& Action : InputActions)
	{
		if (Action.InputAction && Action.InputTag.MatchesTagExact(InputTag))
		{
			return Action.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("InputAction not found for InputTag [%s] in InputDataAsset [%s]"), *InputTag.ToString(), *GetNameSafe(this));
	}

	return nullptr;
}
