#pragma once
#include "CoreMinimal.h"
#include "../Base/Character_Base.h"
#include "../Interface/InteractableInterface.h"
#include "../Interface/InventoryInterface.h"
#include "CharacterPrimary.generated.h"


class UCameraComponent;
class USkeletalMeshComponent;
class USpringArmComponent;
class AWeapon_Base;
class UInventoryComponent;


UCLASS()
class FPBLANK_API ACharacterPrimary final : public ACharacter_Base, public IInventoryInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly);
	UCameraComponent* First_PCamera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly);
	UCameraComponent* Third_PCamera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly);
	USpringArmComponent* SpringArm;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly);
	UInventoryComponent* Inventory;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite);
	UAnimMontage* AnimChangeBattleMode;

	UPROPERTY(BlueprintReadOnly);
	TWeakObjectPtr<AWeapon_Base> CurrentWeapon;

	UPROPERTY();
	TScriptInterface<IInteractableInterface> InteractableActor;

private:
	int8 amountOverlaps;

public:
	// Sets default values for this character's properties
	ACharacterPrimary();
	~ACharacterPrimary();
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void Restart() override;
	void AddKill() override;
	void IAddDeath() override final;
	void ITakeDamage(float damage, FVector InLocation, FVector InNormal) override final;
	void Aiming() override;
	void StopAiming();
	void BeginOverlapWithInteractable();
	void EndOverlapWithInteractable();
	void IOnAddWeapon(AWeapon_Base* NewWeapon) override;
	void IOnRemoveWeapon(AWeapon_Base* WeaponToRemove) override;
	bool ICurrentWeaponInArm() override;
	UInventoryWidget* IGetIventoryWidget() override;
	bool IIsLocallyControlled() const override { return IsLocallyControlled(); }
	FORCEINLINE UInventoryComponent* GetInventoryComponent() const { return Inventory; }
	void SetWeaponArrayInInventory(TArray<AWeapon_Base*>& _WeaponArray);
	void AttachWeapon(AWeapon_Base* WeaponToAttach, FName Destination) override;

protected:
	virtual void BeginPlay() override;
	void DestroyCharacter() override;
	void MoveForward(float axis);
	void MoveRight(float axis);
	void RMB_Action();
	void RMB_StopAction();
	void LMB_Action();
	void LMB_StopAction();
	void ChangeView();
	void ShowInventory();
	void ShowScore();
	void HideScore();
	void PauseMenu();
	void Reloading();
	void ChangeWeapon(int32 index);


	UFUNCTION(Server, Reliable)
	void SetCharacterMovementSpeedRPC(bool shouldRunning);


	UFUNCTION()
	void ChangeBattleMode();

	UFUNCTION()
	void Run();

	UFUNCTION()
	void StopRun();

	UFUNCTION()
	void Interact();

	UFUNCTION(Server, Reliable)
	void ChangeWeaponServer(AWeapon_Base* NewWeapon);

	UFUNCTION(NetMulticast, Reliable)
	void  ChangeWeaponMulticast(AWeapon_Base* OldWeapon, AWeapon_Base* NewWeapon);

	UFUNCTION(Client, Reliable)
	void StopAllAction();
	
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
	bool CanRun() const;


private:
	bool CheckCurrentWeapon();
	bool CheckHUD();
	UFUNCTION(Server, Reliable)
	void OnAddWeaponServer(AWeapon_Base* NewWeapon, FName Destination);

	void OnAttachWeaponLocal(bool destinationIsArmedSocket, int32 weaponAmmo);

	FORCEINLINE bool CanChangeWeapon() const;
	FORCEINLINE bool CanFire() const;
	FORCEINLINE bool CanReloading() const;
	FName GetFreeUnarmedSocket();

	void InteractionLineTrace();
	
	UFUNCTION()
	void AnimMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
		void ChangeBattleModeNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

	UFUNCTION()
	void  ChangeWeaponNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);
};
