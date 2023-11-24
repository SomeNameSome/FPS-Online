#pragma once
#include "CoreMinimal.h"
#include "../Base/Character_Base.h"
#include "Character_AI.generated.h"



class AWeapon_Base;

UCLASS()
class FPBLANK_API ACharacter_AI : public ACharacter_Base
{
	GENERATED_BODY()
	
protected:
	UPROPERTY();
	FTimerHandle RespawnTimer;

	UPROPERTY(VisibleDefaultsOnly);
	USceneComponent* SafeMode;

	UPROPERTY(VisibleDefaultsOnly);
	USceneComponent* BattleMode;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Weapon);
	TWeakObjectPtr<AWeapon_Base> CurrentWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Animation");
	UAnimMontage* AnimChangeBattleMode;

public:
	ACharacter_AI();
	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) final;
	virtual void Tick(float DeltaTime) override;
	void Restart() override;
	void ITakeDamage(float damage, FVector InLocation, FVector InNormal) override;

protected:
	virtual void BeginPlay() override;
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void DestroyCharacter() override;
	void AttachWeaponToActor();

//	Behavior Tree Function
public:
	UFUNCTION(BlueprintCallable, Category = "Action")
	void Reloading();

	UFUNCTION(BlueprintCallable, Category = "Action")
	void Fire();

	UFUNCTION(BlueprintCallable, Category = "Action")
	void StopFire();

	UFUNCTION(BlueprintCallable, Category = "Action")
	void Run();

	UFUNCTION(BlueprintCallable, Category = "Action")
	void StopRun();

	UFUNCTION(BlueprintCallable, Category = "Action")
	void ChangeBattleMode();
public:
	UFUNCTION()
	void SetWeaponFromPC(AWeapon_Base* Weapon);
	UFUNCTION()
	void OnRep_Weapon();
};