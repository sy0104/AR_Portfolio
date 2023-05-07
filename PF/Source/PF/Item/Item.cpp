// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "../Manager/InventoryManager.h"
#include "../Player/PlayerCharacter.h"
#include "../UMG/MainHUDBase.h"
#include "../PFGameModeBase.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

	mBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	mMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	SetRootComponent(mBox);
	mMesh->SetupAttachment(mBox);

	const FString& Path = TEXT("StaticMesh'/Game/Stylized_Egypt/Meshes/props/SM_wood_box_01.SM_wood_box_01'");

	UStaticMesh* Mesh = LoadObject<UStaticMesh>(nullptr, *Path);

	if (IsValid(Mesh))
		mMesh->SetStaticMesh(Mesh);

	
	mBox->SetCollisionProfileName(TEXT("PlayerTrigger"));

	mItemID = EItemID::PT_MP_Potion;
}

void AItem::BeginPlay()
{
	Super::BeginPlay();
	
	mBox->OnComponentBeginOverlap.AddDynamic(this, &AItem::PickItem);
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItem::SetMesh(const FString& Path)
{
	UStaticMesh* Mesh = LoadObject<UStaticMesh>(nullptr, *Path);

	if (IsValid(Mesh))
		mMesh->SetStaticMesh(Mesh);
}

void AItem::PickItem(class UPrimitiveComponent* OverlappedComp,
	class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	PrintViewport(1.f, FColor::Red, TEXT("PickItem"));

	// Inventory에 아이템 추가
	FItemDataInfo* Item = UInventoryManager::GetInst(GetWorld())->GetItemInfo(mItemID);
	UInventoryManager::GetInst(GetWorld())->AddItem(Item);

	if (Item->ItemType == EItemType::Potion)
	{
		APFGameModeBase* GameMode = Cast<APFGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		UMainHUDBase* MainHUD = GameMode->GetMainHUD();

		switch (Item->ID)
		{
		case EItemID::PT_HP_Potion:
			MainHUD->SetHPPotionCount(Item->ItemCount);
			break;
		case EItemID::PT_MP_Potion:
			MainHUD->SetMPPotionCount(Item->ItemCount);
			break;
		case EItemID::PT_Attack_Potion:
			MainHUD->SetAttackPotionCount(Item->ItemCount);
			break;
		case EItemID::PT_Armor_Potion:
			MainHUD->SetArmorPotionCount(Item->ItemCount);
			break;
		}
	}

	Destroy();
}

