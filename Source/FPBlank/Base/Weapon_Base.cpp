#include "Weapon_Base.h"
#include "Components/BoxComponent.h"
#include "Character_Base.h"
#include "Net/UnrealNetwork.h"
#include "Engine/LevelScriptActor.h"


DEFINE_LOG_CATEGORY_STATIC(WeaponLog, All, All);

AWeapon_Base::AWeapon_Base()
{
	PrimaryActorTick.bCanEverTick = false;
	SetActorTickEnabled(false);
	bReplicates = true;
	Holder = nullptr;

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(FName("SkeletalMeshComponent"));
	SetRootComponent(SkeletalMeshComponent);
	SkeletalMeshComponent->SetSimulatePhysics(true);
	SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	SkeletalMeshComponent->SetGenerateOverlapEvents(false);
	SkeletalMeshComponent->AnimClass = UAnimInstance::StaticClass();

	BoxCollisionOutside->SetupAttachment(SkeletalMeshComponent);
	BoxCollisionInside->SetupAttachment(SkeletalMeshComponent);

	SocketTransforms.Add("hand_r_wsock");
	SocketTransforms.Add("clavicle_l_wsock");
	SocketTransforms.Add("clavicle_r_wsock");
}

void AWeapon_Base::SetCollisionState(bool enabled)
{
	Super::SetCollisionState(enabled);
	
	if (enabled)
	{
		SkeletalMeshComponent->SetSimulatePhysics(true);
		SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	}
	else
	{
		SkeletalMeshComponent->SetSimulatePhysics(false);
		SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	UE_LOG(WeaponLog, Error, TEXT("SetupWeaponCollision"));
}

bool AWeapon_Base::IsAttachedToComponent(USceneComponent* Other)
{
	return RootComponent->IsAttachedTo(Other);
}

void AWeapon_Base::ResetWeaponStatClient_Implementation()
{
	currentAmmo = maxAmmo;
}

void AWeapon_Base::StopAnimations()
{
	SkeletalMeshComponent->Stop();
}

const FTransform AWeapon_Base::GetProjectileSpawnTransform() const
{
	return Holder->GetPlayerViewPoint();
}

void AWeapon_Base::IOnInteraction()
{
	SetCollisionState(false);
}

void AWeapon_Base::SetHolder(AActor* NewHolder)
{
	Holder = Cast<ACharacter_Base>(NewHolder);
	if (Holder.IsValid() == false)
	{
		UE_LOG(WeaponLog, Fatal, TEXT("AWeapon_Base::SetHolder !Holder"));
	}
	if (GetOwner())
	{
		if (GetOwner()->IsA(APlayerController::StaticClass()) == false)
		{
			SetOwner(Holder.Get());
		}
	}
	else
	{
		UE_LOG(WeaponLog, Error, TEXT("!GetOwner SetHolder"));
	}
}

void AWeapon_Base::ResetHolder()
{
	if (GetOwner() == Holder)
	{
		SetOwner(GetWorld()->GetLevelScriptActor());
	}
	Holder.Reset();
}

FTransform AWeapon_Base::GetAttachSocketTransformByName(FName SocketName) const
{
	const FTransform* FoundElement = SocketTransforms.Find(SocketName);
	if (!FoundElement)
	{
		UE_LOG(WeaponLog, Fatal, TEXT("FoundElement"));
	}
	return *FoundElement;
}

void AWeapon_Base::SetWeaponTransformByDestination(FName Destination)
{
	SetActorRelativeTransform(GetAttachSocketTransformByName(Destination));
}

void AWeapon_Base::OnRep_AttachmentReplication()
{
	UE_LOG(WeaponLog, Display, TEXT("OnRep_AttachmentReplication"));

	AActor* NewHolder = GetAttachmentReplication().AttachParent;
	FName NewSocket = GetAttachmentReplication().AttachSocket;

	OnRequestAttachment(NewHolder, NewSocket);

	if (Holder.IsValid())
	{
		Holder->AttachWeapon(this, NewSocket);
	}

	Super::OnRep_AttachmentReplication();
}

void AWeapon_Base::OnRequestAttachment(AActor* NewHolder, FName SocketToAttach)
{
	if (NewHolder)
	{
		if (SkeletalMeshComponent->IsCollisionEnabled())
		{
			SetCollisionState(false);
		}
		if (Holder.IsValid() == false)
		{
			SetHolder(NewHolder);
		}
	}
	else
	{
		if (SkeletalMeshComponent->IsCollisionEnabled() == false)
		{
			SetCollisionState(true);
		}
		if (Holder.IsValid())
		{
			ResetHolder();
		}
	}
}