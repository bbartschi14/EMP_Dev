// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstanceBaseEMP.h"
#include "Engine/DataTable.h"

void UGameInstanceBaseEMP::GetDefaultSkillsForClass(EEMPCombatClass InClass, TArray<TSubclassOf<UEMPCombatSkill>>& OutSkills) const
{
	if (DefaultCombatSkillsMap.Contains(InClass))
	{
		for (TSubclassOf<UEMPCombatSkill> skill : DefaultCombatSkillsMap.Find(InClass)->CombatSkills)
		{
			OutSkills.Add(skill);
		}
	}
}

void UGameInstanceBaseEMP::Init() 
{
	// Initialize data map before allowing subsystems to initialize
	DefaultCombatSkillsMap.Empty();
	DefaultCombatSkillsDataTable->ForeachRow<FEMPDefaultSkillsRow>("Add to map", [&](const FName& Key, const FEMPDefaultSkillsRow& Value) { DefaultCombatSkillsMap.Add(Value.CombatClass, Value.CombatSkills); });

	Super::Init();
}
