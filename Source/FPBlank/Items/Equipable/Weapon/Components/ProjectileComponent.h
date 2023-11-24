#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectileComponent.generated.h"



class AWeapon_Base;

UCLASS(ClassGroup=(WeaponComponent), meta=(BlueprintSpawnableComponent))
class FPBLANK_API UProjectileComponent : public UActorComponent
{
	GENERATED_BODY()
		

protected:
	UPROPERTY(EditDefaultsOnly);
	float velocity;

	UPROPERTY(EditDefaultsOnly);
	float gravity;

private:
	UPROPERTY();
	AWeapon_Base* Owner;

	FPredictProjectilePathParams PredictParams;
	float damage;
	float headMultiplier;

public:	
	UProjectileComponent();
	void Execute();

protected:
	virtual void BeginPlay() override;

	void ProjectileLineTrace(FHitResult& HitResult, const FVector& InLocation, const FRotator& InRotation);

	UFUNCTION(Server, Reliable)
		void HandleHitResult(AActor* HitObject, FName HitBoneName, FVector InLocation, FVector InNormal);
};
