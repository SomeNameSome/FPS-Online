#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Interface/InteractableInterface.h"
#include "../Interface/InventoryInterface.h"
#include "InventoryComponent.generated.h"



class AItem_Base;
class AWeapon_Base;


UCLASS(ClassGroup=(CharacterComponent), meta=(BlueprintSpawnableComponent))
class FPBLANK_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()


protected:
	UPROPERTY();
	TArray<AItem_Base*> ConsumableArray;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_WeaponArray);
	TArray<AWeapon_Base*> WeaponArray;

private:
	TScriptInterface<IInventoryInterface> Owner;

public:
	UInventoryComponent();
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
	void AddItem(TScriptInterface<IInteractableInterface>& InteractableActor);
	void RemoveItem(int32 itemIndex);
	void SwapWeapon(int32 firstIndex, int32 secondIndex);
	void SetWeaponArray(TArray<AWeapon_Base*>& _WeaponArray);
	void DebugInfo();
	bool CanChangeWeapon(AWeapon_Base* _CurrentWeapon, int32 otherWeaponIndex);
	//reliable
	AWeapon_Base* GetWeaponAttachedTo(FName SocketName);
	AWeapon_Base* TryGetWeaponByIndex(int32 index);
	
protected:
	virtual void BeginPlay() override;

private:
	void CheckOwner();
	void AddConsumable(AItem_Base* NewItem);
	int32 GetIndexByWeapon(AWeapon_Base* Weapon) noexcept;

	UFUNCTION()
	void OnRep_WeaponArray();
	
	UFUNCTION(Server, Reliable)
	void AddWeaponToArray(AWeapon_Base* NewWeapon);

	UFUNCTION(Server, Reliable)
	void RemoveWeaponFromArray(int32 itemIndex);

	UFUNCTION(Server, Reliable)
		void SwapWeaponServer(int32 firstIndex, int32 secondIndex);


};
