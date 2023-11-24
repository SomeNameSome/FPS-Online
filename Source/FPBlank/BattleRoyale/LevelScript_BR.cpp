#include "LevelScript_BR.h"
#include "Kismet/GameplayStatics.h"
#include "GameMode_BR.h"
#include "../Base/Weapon_Base.h"
#include "GameFramework/PlayerStart.h"
#include <Engine/TargetPoint.h>
#include "../StaticLibrary.h"
#include "Engine.h"

DEFINE_LOG_CATEGORY_STATIC(LevelScriptBRLog, All, All);

ALevelScript_BR::ALevelScript_BR()
{

}

void ALevelScript_BR::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		SetWeaponSpawnPoints();
		SpawnWeapon();
	}
}

void ALevelScript_BR::SpawnWeapon()
{
	UE_LOG(LevelScriptBRLog, Error, TEXT("ALevelScript_BR SpawnWeapon()"));

	AGameMode_Base* GameMode = Cast<AGameMode_Base>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		for (int8 iter = 0; iter < GameMode->WeaponTypes.Num(); iter++)
		{
			FTransform Transform = GetRandomWeaponSpawnPoint();
			Transform.SetScale3D(FVector::OneVector);
			FActorSpawnParameters params;
			params.Owner = this;
			AWeapon_Base* Actor = GetWorld()->SpawnActor<AWeapon_Base>(GameMode->WeaponTypes[iter], Transform, params);
			WeaponArray.Add(Actor);
		}
	}
	else
	{
		UE_LOG(LevelScriptBRLog, Warning, TEXT("!GameMode"));
	}
}

void ALevelScript_BR::SetWeaponSpawnPoints()
{
	for (TActorIterator<ATargetPoint> Iter(GetWorld()); Iter; ++Iter)
	{
		WeaponSpawnPoints.Add(*Iter);
	}
}

FTransform ALevelScript_BR::GetRandomWeaponSpawnPoint() const
{
	int32 ArraySize = WeaponSpawnPoints.Num();
	if (ArraySize > 0)
	{
		return WeaponSpawnPoints[FMath::RandRange(0, --ArraySize)]->GetTransform();
	}
	else
	{
		UE_LOG(LevelScriptBRLog, Error, TEXT("WeaponSpawnPoints.Num() == 0"));
	}
	return FTransform();
}

const TArray<AWeapon_Base*>& ALevelScript_BR::GetWeaponArray() const
{
	return WeaponArray;
}