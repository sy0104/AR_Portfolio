// Fill out your copyright notice in the Description page of Project Settings.


#include "PFGameInstance.h"
#include "Manager/InventoryManager.h"

UPFGameInstance::UPFGameInstance()
	: mItemTable(nullptr)
	, mInventory(nullptr)
	, mMonsterSkillInfoTable(nullptr)
{
	// Item Data Table
	static ConstructorHelpers::FObjectFinder<UDataTable> ItemTable(
		TEXT("DataTable'/Game/Item/ItemDataTable.ItemDataTable'"));

	if (ItemTable.Succeeded())
	{
		mItemTable = ItemTable.Object;
		UInventoryManager::GetInst(this)->SetItemTable(mItemTable);
	}

	// Monster Data Table
	static ConstructorHelpers::FObjectFinder<UDataTable> MonsterTable(
		TEXT("DataTable'/Game/Monster/MonsterTable.MonsterTable'"));

	if (MonsterTable.Succeeded())
		mMonsterTable = MonsterTable.Object;

	//// Monster Skill Table
	//static ConstructorHelpers::FObjectFinder<UDataTable> SkillfInfoTable(
	//	TEXT("DataTable'/Game/Skill/DTMonsterSkill.DTMonsterSkill'"));
	//
	//if (SkillfInfoTable.Succeeded())
	//	mMonsterSkillInfoTable = SkillfInfoTable.Object;

	// Player Skill Table
	static ConstructorHelpers::FObjectFinder<UDataTable> PlayerSkillfInfoTable(
		TEXT("DataTable'/Game/Skill/DTPlayerSkill.DTPlayerSkill'"));

	if (PlayerSkillfInfoTable.Succeeded())
		mPlayerSkillInfoTable = PlayerSkillfInfoTable.Object;
}

UPFGameInstance::~UPFGameInstance()
{
}

void UPFGameInstance::Init()
{
	Super::Init();
}

const FMonsterTableInfo* UPFGameInstance::FindMonsterTable(const FName& Name)
{
	return mMonsterTable->FindRow<FMonsterTableInfo>(Name, TEXT(""));
}

const FSkillData* UPFGameInstance::FindMonsterSkillTable(const FName& Name)
{
	return mMonsterSkillInfoTable->FindRow<FSkillData>(Name, TEXT(""));
}

const FSkillData* UPFGameInstance::FindPlayerSkillTable(const FName& Name)
{
	return mPlayerSkillInfoTable->FindRow<FSkillData>(Name, TEXT(""));
}
