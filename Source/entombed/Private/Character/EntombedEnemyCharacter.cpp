// Copyright Yooshley


#include "Character/EntombedEnemyCharacter.h"

#include "entombed/entombed.h"

AEntombedEnemyCharacter::AEntombedEnemyCharacter()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
}

void AEntombedEnemyCharacter::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	//MainHandItem->SetRenderCustomDepth(true);
	//MainHandItem->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AEntombedEnemyCharacter::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	//MainHandItem->SetRenderCustomDepth(false);
}
