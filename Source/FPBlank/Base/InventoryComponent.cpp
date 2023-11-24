#include "InventoryComponent.h"
#include "Item_Base.h"
#include "../Base/Weapon_Base.h"
#include "Net/UnrealNetwork.h"
#include "../StaticLibrary.h"
#include "../Widget/InventoryWidget.h"

DEFINE_LOG_CATEGORY_STATIC(InventoryLog, All, All);


UInventoryComponent::UInventoryComponent()
{
	const int8 maxWeaponNum = 3;
	PrimaryComponentTick.bCanEverTick = false;
	SetComponentTickEnabled(false);
	SetIsReplicated(true);
	WeaponArray.Init(nullptr, maxWeaponNum);
}

void UInventoryComponent::BeginPlay()
{
	UStaticLibrary::CheckRole(GetOwner(), "UInventoryComponent::BeginPlay");
	Super::BeginPlay();
	Owner = GetOwner();
}

void UInventoryComponent::DebugInfo()
{
	UStaticLibrary::ClearPrintDebugLog();
	for (int32 iter = 0; iter < WeaponArray.Num(); iter++)
	{
		if (WeaponArray[iter])
		{
			FString debug = FString::FromInt(iter) + " " + WeaponArray[iter]->GetActorLabel();
			GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Blue, debug);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Blue, "!WeaponArray");
		}
	}
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UInventoryComponent, WeaponArray, COND_AutonomousOnly, REPNOTIFY_Always);
}

void UInventoryComponent::AddItem(TScriptInterface<IInteractableInterface>& InteractableActor)
{
	InteractableActor->IOnInteraction();
	if (AWeapon_Base* NewWeapon = Cast<AWeapon_Base>(InteractableActor.GetObject()))
	{
		AddWeaponToArray(NewWeapon);
	}
	else
	{
		AddConsumable(Cast<AItem_Base>(InteractableActor.GetObject()));
	}
}

void UInventoryComponent::RemoveItem(int32 itemIndex)
{
	Owner->IOnRemoveWeapon(TryGetWeaponByIndex(itemIndex));
	RemoveWeaponFromArray(itemIndex);
}

void UInventoryComponent::RemoveWeaponFromArray_Implementation(int32 itemIndex)
{
	FDetachmentTransformRules rules(EDetachmentRule::KeepWorld, true);
	WeaponArray[itemIndex]->DetachFromActor(rules);
	WeaponArray[itemIndex]->OnRequestAttachment(nullptr, NAME_None);
	WeaponArray[itemIndex] = nullptr;
}

void UInventoryComponent::SetWeaponArray(TArray<AWeapon_Base*>& _WeaponArray)
{
	for (int8 iter = 0; iter < _WeaponArray.Num(); iter++)
	{
		if (WeaponArray.IsValidIndex(iter))
		{
			WeaponArray[iter] = _WeaponArray[iter];
		}
		else
		{
			WeaponArray.Add(_WeaponArray[iter]);
		}
	}
	OnRep_WeaponArray();
}

void UInventoryComponent::OnRep_WeaponArray()
{
	UStaticLibrary::CheckRole(GetOwner(), "UInventoryComponent::OnRep_WeaponArray");

	for (AWeapon_Base* Weapon : WeaponArray)
	{
		if (Weapon)
		{
			if (!Weapon->IsAttachedTo(GetOwner()))
			{
				UE_LOG(InventoryLog, Display, TEXT("!Weapon->IsAttachedTo(this)"));

				CheckOwner();
				if (Owner->IIsLocallyControlled())
				{
					Owner->IOnAddWeapon(Weapon);
				}
			}
		}
		else
		{
			UE_LOG(InventoryLog, Warning, TEXT("!Weapon"));
		}
	}
}

void UInventoryComponent::AddWeaponToArray_Implementation(AWeapon_Base* NewWeapon)
{
	for (AWeapon_Base*& Weapon : WeaponArray)
	{
		if (Weapon == nullptr)
		{
			Weapon = NewWeapon;
			OnRep_WeaponArray();
			return;
		}
	}
	WeaponArray.Add(NewWeapon);
	OnRep_WeaponArray();
}

bool UInventoryComponent::CanChangeWeapon(AWeapon_Base* _CurrentWeapon, int32 otherWeaponIndex)
{
	if (WeaponArray.IsValidIndex(otherWeaponIndex))
	{
		AWeapon_Base* OtherWeapon = TryGetWeaponByIndex(otherWeaponIndex);
		bool isTheSameWeapon = _CurrentWeapon == OtherWeapon;
		if ((!isTheSameWeapon && OtherWeapon) || (isTheSameWeapon && !Owner->ICurrentWeaponInArm() && _CurrentWeapon))
		{
			return true;
		}
	}
	return false;
}

AWeapon_Base* UInventoryComponent::GetWeaponAttachedTo(FName SocketName)
{
	for (AWeapon_Base* Weapon : WeaponArray)
	{
		if (Weapon)
		{
			if (Weapon->GetAttachParentSocketName() == SocketName)
			{
				return Weapon;
			}
		}
	}
	return nullptr;
}

void UInventoryComponent::SwapWeapon(int32 firstIndex, int32 secondIndex)
{
	if (WeaponArray.IsValidIndex(firstIndex) && WeaponArray.IsValidIndex(secondIndex))
	{
		if (WeaponArray[firstIndex] || WeaponArray[secondIndex])
		{
			UE_LOG(InventoryLog, Display, TEXT("WeaponArray.Swap(firstIndex, secondIndex) %d %d"), firstIndex, secondIndex);
			Owner->IGetIventoryWidget()->SwapWeapons(firstIndex, secondIndex);
			SwapWeaponServer(firstIndex, secondIndex);
		}
	}
}

void UInventoryComponent::SwapWeaponServer_Implementation(int32 firstIndex, int32 secondIndex)
{
	WeaponArray.Swap(firstIndex, secondIndex);
}

AWeapon_Base* UInventoryComponent::TryGetWeaponByIndex(int32 index)
{
	if (WeaponArray.IsValidIndex(index))
	{
		if (WeaponArray[index])
		{
			return WeaponArray[index];
		}
	}
	UE_LOG(InventoryLog, Warning, TEXT("TryGetWeaponByIndex(int32 index) nullptr"));
	return nullptr;
}

void UInventoryComponent::AddConsumable(AItem_Base* NewItem)
{
	ConsumableArray.Add(NewItem);
}

int32 UInventoryComponent::GetIndexByWeapon(AWeapon_Base* Weapon) noexcept
{
	return WeaponArray.Find(Weapon);
}

void UInventoryComponent::CheckOwner()
{
	if (!Owner)
		Owner = GetOwner();
}