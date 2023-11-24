#include "ProjectileComponent.h"
#include "../Firearm.h"
#include "../../../../Base/Character_Base.h"
#include "../../../../Base/Weapon_Base.h"
#include "../../../../Interface/DamageableInterface.h"
#include "Engine/StaticMeshActor.h"


UProjectileComponent::UProjectileComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetComponentTickEnabled(false);

	velocity = 2000.f;
	gravity = -1.f;
}

void UProjectileComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<AWeapon_Base>(GetOwner());
	if (Owner)
	{
		damage = Owner->GetBaseDamage();
	}
	if (AFirearm* FirearmOwner = Cast<AFirearm>(Owner))
	{
		headMultiplier = FirearmOwner->GetHeadMultiplier();
	}

	PredictParams.ProjectileRadius = 1.f;
	PredictParams.bTraceWithCollision = true;
	PredictParams.bTraceComplex = false;
	PredictParams.DrawDebugType = EDrawDebugTrace::None;
	PredictParams.DrawDebugTime = 5.f;
	PredictParams.SimFrequency = 10.f;
	PredictParams.MaxSimTime = 3.f;
	PredictParams.OverrideGravityZ = gravity;
	PredictParams.TraceChannel = ECollisionChannel::ECC_Visibility;
}

void UProjectileComponent::Execute()
{
	FHitResult HitResult;
	const FTransform InTransform(Owner->GetProjectileSpawnTransform());

	ProjectileLineTrace(HitResult, InTransform.GetLocation(), InTransform.GetRotation().Rotator());

	if (HitResult.bBlockingHit)
	{
		HandleHitResult(HitResult.GetActor(), HitResult.BoneName, HitResult.Location, HitResult.Normal);
	}
}

void UProjectileComponent::ProjectileLineTrace(FHitResult& HitResult, const FVector& InLocation, const FRotator& InRotation)
{
	TArray <AActor*> ActorToIgnore;
	ActorToIgnore.Add(Owner->GetHolder());

	PredictParams.StartLocation = InLocation;
	PredictParams.LaunchVelocity = InRotation.Vector() * velocity;
	PredictParams.ActorsToIgnore = ActorToIgnore;
	
	
	FPredictProjectilePathResult PredictResults;

	UGameplayStatics::PredictProjectilePath(GetWorld(), PredictParams, PredictResults);
	HitResult = PredictResults.HitResult;
}

void UProjectileComponent::HandleHitResult_Implementation(AActor* HitObject, FName HitBoneName, FVector InLocation, FVector InNormal)
{
	if (IDamageableInterface* HitPlayer = Cast<IDamageableInterface>(HitObject))
	{
		ACharacter_Base* CauserPlayer = Owner->GetHolder();
		if (!HitPlayer->IIsDying())
		{
			float totalDamage = this->damage;
			bool isHeadshot = HitBoneName == "spine_03";
		
			if (isHeadshot && headMultiplier != 0.f)
			{
				totalDamage *= headMultiplier;
			}
			
			if (CauserPlayer && !HitPlayer->_getUObject()->IsA(AStaticMeshActor::StaticClass()))
			{
				CauserPlayer->CauseDamage(isHeadshot);
			}
		
			HitPlayer->ITakeDamage(-totalDamage, InLocation, InNormal);
			if (HitPlayer->IIsDying())
			{
				CauserPlayer->AddKill();
				HitPlayer->IAddDeath();
			}
		}
	}
}