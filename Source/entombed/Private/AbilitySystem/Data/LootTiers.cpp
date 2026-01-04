// Copyright Yooshley


#include "AbilitySystem/Data/LootTiers.h"

TArray<FLootItem> ULootTiers::GetLootItems()
{
	TArray<FLootItem> Items;
	
	for (FLootItem& Item : LootItems)
	{
		for (int32 i = 0; i < Item.LootQuantity; ++i)
		{
			if (FMath::RandRange(1, 100) < Item.LootChance)
			{
				FLootItem NewItem;
				NewItem.LootClass = Item.LootClass;
				NewItem.bLootLevelOverride = Item.bLootLevelOverride;
				Items.Add(NewItem);
			}
		}
	}
	
	return Items;
}
