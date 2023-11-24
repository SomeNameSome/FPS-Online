#include "Firearm.h"
#include "Particles/ParticleSystemComponent.h"
#include <DrawDebugHelpers.h>
#include "Kismet/GameplayStatics.h"
#include "Components/Shell.h"
#include "Components/Magazine.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Net/UnrealNetwork.h"
#include "Components/BoxComponent.h"
#include "../../../Base/HUD_Base.h"
#include "../../../StaticLibrary.h"
#include "../../../BattleRoyale/GameMode_BR.h"
#include "Engine/StaticMeshActor.h"
#include "Particles/ParticleSystem.h"
#include "../../../Base/Character_Base.h"
#include "Components/ProjectileComponent.h"

DEFINE_LOG_CATEGORY_STATIC(FirearmLog, All, All);


AFirearm::AFirearm()
{
	PrimaryActorTick.bCanEverTick = false;

	
	TimelineRecoilSystem = CreateDefaultSubobject<UTimelineComponent>(FName("Timeline"));
	ProjectileComponent = CreateDefaultSubobject<UProjectileComponent>(FName("ProjectileComponent"));
}

void AFirearm::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(FirearmLog, Warning, TEXT("Weapon Begin Play"));
	if (TimelineRecoilSystem)
	{
		FOnTimelineFloat funcInterp{};
		funcInterp.BindUFunction(this, FName{ TEXT("RecoilInterpProgress") });
		TimelineRecoilSystem->AddInterpFloat(RecoilCurve, funcInterp);
	}
	fireRateDelay = 60.f / fireRate;
	WeaponAnimFire->RateScale = 1.0f / fireRateDelay;
	if (PawnAnimFire)
	{
		PawnAnimFire->RateScale = 1.0f / fireRateDelay;
	}
}

void AFirearm::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFirearm::ExecuteAction(EWeaponAction Action)
{
	switch (Action)
	{
	case EWeaponAction::PrimaryAction:
	{
		GetWorldTimerManager().SetTimer(Holder->GetTimer(), this, &AFirearm::Fire, fireRateDelay, true, 0.f);
		break;
	}
	case EWeaponAction::SecondaryAction:
	{
		if (CanAiming())
		{
			Holder->Aiming();
		}
		break;
	}
	case EWeaponAction::Reloading:
	{
		HasAuthority() ? ReloadingMulticast(currentAmmo) : ReloadingServer(currentAmmo);
		Holder->AddState(ECharacterState::PlayingAnim);
		break;
	}
	}
}

void AFirearm::FireAI()
{
	if (!Holder->State(ECharacterState::Firing) || currentAmmo <= 0)
	{
		GetWorldTimerManager().ClearAllTimersForObject(this);
		return;
	}

	ProjectileComponent->Execute();

	UnreliableServerCallback(FName("FireReplication"), true);
}

void AFirearm::Fire()
{
	//UStaticLibrary::CheckRole(Holder, "AFirearm::Fire");

	if (!Holder->State(ECharacterState::Firing) || currentAmmo <= 0)
	{
		UE_LOG(FirearmLog, Warning, TEXT("Current ammo = %d"), currentAmmo);
		GetWorldTimerManager().ClearAllTimersForObject(this);
		ResetTimeline();
		if (currentAmmo <= 0)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), DryFireSound);
		}
		return;
	}
	UE_LOG(FirearmLog, Warning, TEXT("Fire()"));
	
	ProjectileComponent->Execute();

	UnreliableServerCallback(FName("FireReplication"), true);

	Holder->PlayCameraShake(CameraShakeClass);

	TimelineRecoilSystem->Play();

	currentAmmo--;

	Holder->UpdateAmmoInWeaponWidget(currentAmmo);
}

void AFirearm::FireReplication()
{
	SkeletalMeshComponent->PlayAnimation(WeaponAnimFire, false);
	UStaticLibrary::PlayMontage(Holder->GetMesh(), PawnAnimFire);

	FTransform CoordShell = SkeletalMeshComponent->GetSocketTransform(FName("Shell"));
	AShell* CurrentShell = GetWorld()->SpawnActor<AShell>(ShellClass, CoordShell);
	if (CurrentShell)
	{
		UE_LOG(FirearmLog, Warning, TEXT("Shell FireRep"));
	}
}

void AFirearm::ServerCallback_Implementation(FName FunctionCallbackName, bool Multicast)
{
	UE_LOG(FirearmLog, Warning, TEXT("ServerCallback_Implementation()"));
	if (Multicast)
	{
		MulticastCallback(FunctionCallbackName);
	}
	else
	{
		FSimpleDelegate ptrFunc;
		ptrFunc.BindUFunction(this, FunctionCallbackName);
		
		if (ptrFunc.IsBound())
		{
			UE_LOG(FirearmLog, Warning, TEXT("ptrFunc.IsBound()"));
			ptrFunc.Execute();
		}
		else
		{
			UE_LOG(FirearmLog, Warning, TEXT("!ptrFunc.IsBound()"));
		}
	}
}

void AFirearm::UnreliableServerCallback_Implementation(FName FunctionCallbackName, bool Multicast)
{
	if (Multicast)
	{
		UnreliableMulticastCallback(FunctionCallbackName);
	}
	else
	{
		FSimpleDelegate ptrFunc;
		ptrFunc.BindUFunction(this, FunctionCallbackName);

		if (ptrFunc.IsBound())
		{
			ptrFunc.Execute();
		}
		else
		{
			UE_LOG(FirearmLog, Warning, TEXT("!ptrFunc.IsBound()"));
		}
	}
}

void AFirearm::MulticastCallback_Implementation(FName FunctionCallbackName)
{
	FSimpleDelegate ptrFunc;
	ptrFunc.BindUFunction(this, FunctionCallbackName);
	if (ptrFunc.IsBound())
	{
		UE_LOG(FirearmLog, Warning, TEXT("ptrFunc.IsBound()"));
		ptrFunc.Execute();
	}
	else
	{
		UE_LOG(FirearmLog, Warning, TEXT("!ptrFunc.IsBound()"));
	}
}

void AFirearm::UnreliableMulticastCallback_Implementation(FName FunctionCallbackName)
{
	FSimpleDelegate ptrFunc;
	ptrFunc.BindUFunction(this, FunctionCallbackName);
	if (ptrFunc.IsBound())
	{
		ptrFunc.Execute();
	}
	else
	{
		UE_LOG(FirearmLog, Warning, TEXT("!ptrFunc.IsBound()"));
	}
}

void AFirearm::ReloadingServer_Implementation(int32 _currentAmmo)
{
	ReloadingMulticast(_currentAmmo);
}

void AFirearm::ReloadingMulticast_Implementation(int32 _currentAmmo)
{
	Reloading(_currentAmmo);
}

void AFirearm::RecoilInterpProgress(float value)
{
	UE_LOG(FirearmLog, Warning, TEXT("RecoilInterpProcess, %f"), value);
	Holder->AddControllerPitchInput(value);
	TimelineRecoilSystem->Stop();
}

void AFirearm::ResetTimeline()
{
	float defaultValue = RecoilCurve->FloatCurve.GetFirstKey().Value;
	RecoilCurve->FloatCurve.UpdateOrAddKey(0.f, 0.f);
	TimelineRecoilSystem->SetNewTime(0.f);
	RecoilCurve->FloatCurve.UpdateOrAddKey(0.f, defaultValue);
}

bool AFirearm::CanAiming()
{
	return SkeletalMeshComponent->DoesSocketExist("Aiming");
}