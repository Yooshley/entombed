// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "EntombedInputDataAsset.h"
#include "EntombedInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class ENTOMBED_API UEntombedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
public:
	template<class UserClass, typename PressedType, typename ReleasedType, typename HeldType>
	void BindAbilityActions(const UEntombedInputDataAsset* InputDataAsset, UserClass* Object, PressedType PressedFunc, ReleasedType ReleasedFunc, HeldType HeldFunc);
};

template <class UserClass, typename PressedType, typename ReleasedType, typename HeldType>
void UEntombedInputComponent::BindAbilityActions(const UEntombedInputDataAsset* InputDataAsset, UserClass* Object,
	PressedType PressedFunc, ReleasedType ReleasedFunc, HeldType HeldFunc)
{
	check(InputDataAsset);

	for (const FEntombedInputAction& Action : InputDataAsset->InputActions)
	{
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Started, Object, PressedFunc, Action.InputTag);
			}

			if (ReleasedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Completed , Object, ReleasedFunc, Action.InputTag);
			}
			
			if (HeldFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, Action.InputTag);
			}
		}
	}
}
