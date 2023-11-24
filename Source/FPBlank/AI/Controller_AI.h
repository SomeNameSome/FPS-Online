#pragma once
#include "CoreMinimal.h"
#include "AIController.h"
#include "Controller_AI.generated.h"


class ACharacter_AI;
class UPawnSensingComponent;
class AWeapon_Base;
class ACharacter_Base;

UCLASS()
class FPBLANK_API AController_AI : public AAIController
{
	GENERATED_BODY()
	
public:
	UPROPERTY();
	FTimerHandle PawnSensingTimer;

	UPROPERTY(VisibleDefaultsOnly);
	UPawnSensingComponent* PawnSensingComponent;

	UPROPERTY(BlueprintReadWrite);
	TWeakObjectPtr<ACharacter_Base> TargetPlayer;

	UPROPERTY(BlueprintReadOnly);
	TWeakObjectPtr<ACharacter_AI> ControlledAI;

	UPROPERTY(EditDefaultsOnly, Category = "AI|BehaviorTree");
	UBehaviorTree* BT_Calm;

	UPROPERTY(EditDefaultsOnly, Category = "AI|BehaviorTree");
	UBehaviorTree* BT_Aggressive;

	UPROPERTY(EditDefaultsOnly, Category = "AI|BehaviorTree");
	UBehaviorTree* BT_Pursuit;

	UPROPERTY();
	AWeapon_Base* AIWeapon;

	UPROPERTY(BlueprintReadOnly);
	bool pawnInSight;

public:
	AController_AI();

	void StopAllLogic();

	void SpawnWeapon();

	UFUNCTION()
	void OnSeePawn(APawn* _Pawn);

	virtual void OnPossess(APawn* InPawn) override;

protected:
	void BeginPlay() override;

protected:
	UFUNCTION()
	void OnPawnLost();
};
