// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#define CUSTOM_DEPTH_RED 250

#define ECC_Projectile ECollisionChannel::ECC_GameTraceChannel1

/* SOCKET NAMES */
#define MAIN_HAND_SOCKET_NAME  FName("MainHandSocket")
#define OFF_HAND_SOCKET_NAME  FName("OffHandSocket")
#define TIP_SOCKET_NAME  FName("TipSocket")
#define HEAD_SOCKET_NAME  FName("HeadSocket")

/* TAGS */
#define ENEMY_TAG "Enemy"

/*BLACKBOARD KEYS*/
#define BB_KEY_HITREACTING_BOOL "IsHitReacting"
#define BB_KEY_RANGED_BOOL "IsRanged"
#define BB_KEY_DEAD_BOOL "IsDead"