#include "PC_FB.h"
#include "GameMode_FB.h"
#include "../Base/Weapon_Base.h"
#include <Net/UnrealNetwork.h>
#include "../Primary/CharacterPrimary.h"
#include "../Base/HUD_Base.h"

DEFINE_LOG_CATEGORY_STATIC(PC_FBLog, All, All);

void APC_FB::BeginPlay()
{
	UE_LOG(PC_FBLog, Warning, TEXT("APC_FB::BeginPlay"));
	Super::BeginPlay();
	if (HasAuthority())
	{
		SpawnWeapon();
		AttachWeaponToCharacter();
	}
}

void APC_FB::SpawnWeapon()
{
	AGameMode_FB* GameMode = Cast<AGameMode_FB>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		for (const TSubclassOf<AWeapon_Base>& WeaponType : GameMode->WeaponTypes)
		{
			FActorSpawnParameters params;
			params.Owner = this;
			AWeapon_Base* NewWeapon = GetWorld()->SpawnActor<AWeapon_Base>(WeaponType, FTransform(), params);
			CharacterWeapons.Add(NewWeapon);
		}
	}
	else
	{
		UE_LOG(PC_FBLog, Warning, TEXT("!APC_FB GameMode"));
	}
}

void APC_FB::RespawnPlayer()
{
	Super::SpawnPlayer();
	if (CharacterWeapons.Num() > 0)
	{
		for (AWeapon_Base* Weapon : CharacterWeapons)
		{
			Weapon->ResetWeaponStatClient();
		}
		AttachWeaponToCharacter();
	}
	else
	{
		UE_LOG(PC_FBLog, Warning, TEXT("!CharacterWeapon"));
	}
	ResetHUD();
}

void APC_FB::AttachWeaponToCharacter()
{
	UE_LOG(PC_FBLog, Warning, TEXT("AttachWeaponToCharacter"));
	ACharacterPrimary* character = Cast<ACharacterPrimary>(GetPawn());
	if (character)
	{
		character->SetWeaponArrayInInventory(CharacterWeapons);
	}
	else
	{
		UE_LOG(PC_FBLog, Warning, TEXT("!character"));
	}
}

void APC_FB::ResetHUD_Implementation()
{
	if (MyHUD)
	{
		MyHUD->Reset();
	}
}