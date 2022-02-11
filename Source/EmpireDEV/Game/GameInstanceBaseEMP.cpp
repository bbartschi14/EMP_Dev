// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstanceBaseEMP.h"
#include "Engine/DataTable.h"

void UGameInstanceBaseEMP::GetDefaultSkillsForClass(EEMPCombatClass InClass, TArray<TSubclassOf<UEMPCombatSkill>>& OutSkills) const
{
	if (DefaultCombatSkillsData->DefaultCombatSkillsMap.Contains(InClass))
	{
		for (TSubclassOf<UEMPCombatSkill> skill : DefaultCombatSkillsData->DefaultCombatSkillsMap.Find(InClass)->CombatSkills)
		{
			OutSkills.Add(skill);
		}
	}
}

void UGameInstanceBaseEMP::Init() 
{
	Super::Init();
}
