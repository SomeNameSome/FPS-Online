#include "Item_Base.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "HUD_Base.h"
#include "../Primary/CharacterPrimary.h"

DEFINE_LOG_CATEGORY_STATIC(Item_BaseLog, All, All);

AItem_Base::AItem_Base()
{
	PrimaryActorTick.bCanEverTick = false;
	SetActorTickEnabled(false);

	BoxCollisionOutside = CreateDefaultSubobject<UBoxComponent>(FName("BoxCollisionOutside"));
	BoxCollisionOutside->SetIsReplicated(false);
	BoxCollisionOutside->bHiddenInGame = false;
	BoxCollisionOutside->SetRelativeScale3D(FVector(10.f, 10.f, 1.f));
	BoxCollisionOutside->SetUsingAbsoluteRotation(true);
	BoxCollisionOutside->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BoxCollisionOutside->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	BoxCollisionOutside->SetGenerateOverlapEvents(true);
	BoxCollisionOutside->SetComponentTickEnabled(false);

	BoxCollisionInside = CreateDefaultSubobject<UBoxComponent>(FName("BoxCollisionInside"));
	BoxCollisionInside->SetIsReplicated(false);
	BoxCollisionInside->bHiddenInGame = false;
	BoxCollisionInside->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BoxCollisionInside->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	BoxCollisionInside->SetGenerateOverlapEvents(false);
	BoxCollisionInside->SetComponentTickEnabled(false);

	FScriptDelegate delegate;
	delegate.BindUFunction(this, FName("OnOverlapBegin"));
	BoxCollisionOutside->OnComponentBeginOverlap.Add(delegate);
	delegate.BindUFunction(this, FName("OnOverlapEnd"));
	BoxCollisionOutside->OnComponentEndOverlap.Add(delegate);
}

void AItem_Base::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(Item_BaseLog, Warning, TEXT("OnOverlapBegin"));
	ACharacterPrimary* Character = Cast<ACharacterPrimary>(OtherActor);
	if (Character)
	{
		if (Character->IsLocallyControlled() && Character->GetCharacterHUD())
		{
			Character->BeginOverlapWithInteractable();
		}
	}
}

void AItem_Base::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(Item_BaseLog, Warning, TEXT("OnOverlapEnd"));
	ACharacterPrimary* Character = Cast<ACharacterPrimary>(OtherActor);
	if (Character)
	{
		if (Character->IsLocallyControlled() && Character->GetCharacterHUD())
		{
			Character->EndOverlapWithInteractable();
		}
	}
}

void AItem_Base::SetCollisionState(bool enabled)
{
	if (enabled)
	{
		BoxCollisionInside->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		BoxCollisionInside->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		BoxCollisionInside->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
		BoxCollisionInside->SetGenerateOverlapEvents(true);
		BoxCollisionInside->bHiddenInGame = false;
		BoxCollisionInside->SetVisibility(true);
		BoxCollisionInside->Activate();


		BoxCollisionOutside->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		BoxCollisionOutside->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		BoxCollisionOutside->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
		BoxCollisionOutside->SetGenerateOverlapEvents(true);
		BoxCollisionOutside->bHiddenInGame = false;
		BoxCollisionOutside->SetVisibility(true);
		BoxCollisionOutside->Activate();
	}
	else
	{
		BoxCollisionInside->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		BoxCollisionInside->SetComponentTickEnabled(false);
		BoxCollisionInside->SetGenerateOverlapEvents(false);
		BoxCollisionInside->bHiddenInGame = true;
		BoxCollisionInside->SetVisibility(false);
		BoxCollisionInside->Deactivate();

		BoxCollisionOutside->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		BoxCollisionOutside->SetComponentTickEnabled(false);
		BoxCollisionOutside->SetGenerateOverlapEvents(false);
		BoxCollisionOutside->bHiddenInGame = true;
		BoxCollisionOutside->SetVisibility(false);
		BoxCollisionOutside->Deactivate();
	}
}