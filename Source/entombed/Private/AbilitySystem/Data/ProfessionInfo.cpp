// Copyright Yooshley


#include "AbilitySystem/Data/ProfessionInfo.h"

FProfessionDefaultInfo UProfessionInfo::GetProfessionDefaultInfo(EProfession Profession)
{
	return ProfessionInformation.FindChecked(Profession);
}
