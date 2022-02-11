// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "../Combat/Structs/FEMPCombatUnitData.h"
#include "../Combat/Structs/FEMPDefaultSkills.h"
#include "GameInstanceBaseEMP.generated.h"


/**
 * 
 */
UCLASS()
class EMPIREDEV_API UGameInstanceBaseEMP : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
		class UEMPSquadsDatabase* TestOnlySquadsDatabase;
	UPROPERTY(EditDefaultsOnly)
		class UEMPArmyDatabase* TestOnlyArmyDatabase;

	UFUNCTION()
	void GetDefaultSkillsForClass(EEMPCombatClass InClass, TArray<TSubclassOf<UEMPCombatSkill>>& OutSkills) const;
protected:
	void Init() override;

	UPROPERTY(EditDefaultsOnly)
		class UDataTable* DefaultCombatSkillsDataTable;

	UPROPERTY(EditDefaultsOnly)
	TMap<EEMPCombatClass, FEMPDefaultSkills> DefaultCombatSkillsMap;
};
