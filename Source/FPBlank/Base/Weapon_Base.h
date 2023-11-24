#pragma once
#include "CoreMinimal.h"
#include "Item_Base.h"
#include "../Interface/InteractableInterface.h"
#include "Weapon_Base.generated.h"


UENUM(BlueprintType)
enum class EWeaponAction : uint8
{
	PrimaryAction,
	SecondaryAction,
	Reloading
};

class ACharacter_Base;

UCLASS()
class FPBLANK_API AWeapon_Base : public AItem_Base, public IInteractableInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly);
	TWeakObjectPtr<ACharacter_Base> Holder;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly);
	USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Animation");
	UAnimSequenceBase* AnimPawnIdle;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon|Sockets");
	TMap<FName, FTransform> SocketTransforms;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Characteristics");
	float damage;

	UPROPERTY(BlueprintReadWrite, Category = "Weapon|Characteristics");
	int32 currentAmmo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Characteristics");
	int32 maxAmmo;

public:
	AWeapon_Base();
	void SetCollisionState(bool enabled) override;
	FORCEINLINE USkeletalMeshComponent* GetSkeletalMeshComponent() const { return SkeletalMeshComponent; }
	void SetHolder(AActor* NewHolder);
	void ResetHolder();
	FORCEINLINE ACharacter_Base* GetHolder() const { return Holder.Get(); }
	FORCEINLINE UAnimSequenceBase* GetAnimPawnIdle() const { return AnimPawnIdle; }
	FORCEINLINE FTransform GetAttachSocketTransformByName(FName SocketName) const;
	FORCEINLINE float GetBaseDamage() const { return damage; }
	FORCEINLINE int32 GetCurrentAmmo() const { return currentAmmo; }
	FORCEINLINE int32 GetMaxAmmo() const { return maxAmmo; }
	FORCEINLINE bool MagazineIsFull() const { return currentAmmo == maxAmmo; }
	FORCEINLINE USceneComponent* GetParentComponent() const { return RootComponent->GetAttachParent(); }
	void OnRequestAttachment(AActor* NewHolder, FName SocketToAttach);
	virtual void ExecuteAction(EWeaponAction Action) {}
	bool IsAttachedToComponent(USceneComponent* Other);
	void StopAnimations();
	virtual void OnRep_AttachmentReplication() override;
	virtual void IOnInteraction() override;
	FName IGetItemName() override { return ItemName; }
	const FTransform GetProjectileSpawnTransform() const;
	void SetWeaponTransformByDestination(FName Destination);
	//void SetCollisionEnabled(bool enabled) { CollisionEnabled = enabled; OnRep_CollisionEnabled(); }
	UFUNCTION(Client, Reliable)
	void ResetWeaponStatClient();
};
