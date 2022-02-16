// Empire 2021. Ben Bartschi and Jacob Bartschi


#include "EMPCombatSimulator.h"
#include "../Game/EMPCombatMapGameMode.h"
#include "Skills/EMPStatModifierCombatSkill.h"
#include "Structs/FEMPSquadData.h"
#include "EMPCombatStatics.h"

void UEMPCombatSimulator::InitializeCombatData(UEMPSquadData* squadOne, UEMPSquadData* squadTwo, AEMPCombatMapGameMode* owningCombatGameMode)
{
	CombatSquadOne = squadOne;
	CombatSquadTwo = squadTwo;
	OwningCombatGameMode = owningCombatGameMode;

	CombatRoundsRemaining = 5; // Hard set to 5 for now

	SquadOneAttackingDirection = CombatSquadTwo->CombatAreaLocation - CombatSquadOne->CombatAreaLocation;
	SquadTwoAttackingDirection = SquadOneAttackingDirection * -1;
}

void UEMPCombatSimulator::SimulateSingleRoundOfAttacks()
{
	RoundHitResults.Empty();

	// Calculate who attacks who and damage
	SimulateSquadAttacks(CombatSquadOne, CombatSquadTwo, SquadOneAttackingDirection);
	SimulateSquadAttacks(CombatSquadTwo, CombatSquadOne, SquadTwoAttackingDirection);
}

void UEMPCombatSimulator::SimulateSquadAttacks(UEMPSquadData* attackingSquad, UEMPSquadData* defendingSquad, FIntPoint attackingDirection)
{
	TArray<UEMPCombatUnitData*> attackingSquadCombatUnits = attackingSquad->CombatUnitsInSquad;
	for (UEMPCombatUnitData* unit : attackingSquadCombatUnits)
	{
		FIntPoint unitPosition = unit->CombatLocation;

		// First check space in front of unit
		UEMPCombatUnitData* potentialEnemy = defendingSquad->GetCombatUnitAtCombatLocation(unitPosition + attackingDirection);

		// Check in front and over in both directions
		if (!potentialEnemy)
		{
			potentialEnemy = defendingSquad->GetCombatUnitAtCombatLocation(unitPosition + attackingDirection + UEMPCombatStatics::GetPerpendicularDirection_Clockwise(attackingDirection));

			if (!potentialEnemy)
			{
				potentialEnemy = defendingSquad->GetCombatUnitAtCombatLocation(unitPosition + attackingDirection + UEMPCombatStatics::GetPerpendicularDirection_CounterClockwise(attackingDirection));

				if (!potentialEnemy)
				{
					// This unit has no enemies to attack, check next unit
					continue;
				}
			}
		}
		// Enemy has been found, simulate attack (Roll chance. If hits, die)

#pragma region Apply Stat Modifiers
		FEMPCombatHitResult hitResult;
		hitResult.AttackingUnit = unit;
		hitResult.DefendingUnit = potentialEnemy;

		TArray<UEMPStatModifierCombatSkill*> attackingStatMods;
		hitResult.AttackingUnit->GetStatModifierCombatSkills(attackingStatMods);
		TArray<UEMPStatModifierCombatSkill*> defendingStatMods;
		hitResult.DefendingUnit->GetStatModifierCombatSkills(defendingStatMods);

		FEMPStatModifierCombatSkillData attackingData;
		attackingData.OwningUnit = hitResult.AttackingUnit;
		attackingData.OtherUnit = hitResult.DefendingUnit;
		attackingData.bIsAttack = true;

		FEMPStatModifierCombatSkillData defendingData;
		defendingData.OwningUnit = hitResult.DefendingUnit;
		defendingData.OtherUnit = hitResult.AttackingUnit;
		defendingData.bIsAttack = false;

		float hitChance = 0.5f;
		int damage = hitResult.AttackingUnit->Damage;
		int armor = hitResult.DefendingUnit->Armor;

		for (auto mod : attackingStatMods)
		{
			mod->SetData(attackingData);
		}

		for (auto mod : defendingStatMods)
		{
			mod->SetData(defendingData);
		}

		TArray<UEMPStatModifierCombatSkill*> allStatMods; 
		allStatMods.Append(attackingStatMods); 
		allStatMods.Append(defendingStatMods);

		for (auto mod : allStatMods)
		{
			hitChance = mod->ModifyHitChance(hitChance);
			damage = mod->ModifyDamage(damage);
			armor = mod->ModifyArmor(armor);
		}
#pragma endregion Apply Stat Modifiers

		float hitRoll = FMath::RandRange(0.0f, 1.0f);
		
		if (hitRoll >= (1.0f - hitChance))
		{
			hitResult.bHitSuccessful = true;
			hitResult.DamageDealt = FMath::Max(damage - armor, 0);
		}
		else
		{
			hitResult.bHitSuccessful = false;
			hitResult.DamageDealt = 0;
		}

		RoundHitResults.Add(hitResult);
	}
}

bool UEMPCombatSimulator::AnimateAttacks(float animationTime)
{
	for (FEMPCombatHitResult hitResult : RoundHitResults)
	{
		OwningCombatGameMode->OnCombatUnitAttackAnimate.Broadcast(hitResult.AttackingUnit, hitResult.DefendingUnit, animationTime);
	}

	return RoundHitResults.Num() > 0;
}

void UEMPCombatSimulator::ResolveDamage()
{
	for (FEMPCombatHitResult hitResult : RoundHitResults)
	{
		OwningCombatGameMode->ResolveHit(hitResult);
	}
}

void UEMPCombatSimulator::UpdateCombatMovement(float animationTime)
{
	TArray<UEMPCombatUnitData*> SquadOneUnitsToAdvance;
	TArray<UEMPCombatUnitData*> SquadTwoUnitsToAdvance;

	FIntPoint rotatedLocalOrigin = UEMPCombatStatics::RotateLocalGridCoordinate(FIntPoint(0, 0), UEMPCombatStatics::IntPointToDirection(SquadOneAttackingDirection));

	// This coordinate (originGridCoordinate) serves as the origin for the rest of the calculations. The two attacking squads should fit in a 10x5 rectangle, 
	// originating at this origin with 10 units in the SquadOneAttackingDirection, and 5 units in its perpendicular clockwise
	//				 _ _ _ _ _
	//				|_ _ _ _ _|
	//				|_ _ _ _ _|
	//				|_ _ _ _ _| SquadTwo
	//				|_ _ _ _ _|
	//				|_ _ _ _ _|
	//				|_ _ _ _ _|
	//				|_ _ _ _ _| 
	//				|_ _ _ _ _| SquadOneAttackingDirection ^^^
	//				|_ _ _ _ _|
	//				|x _ _ _ _|
	//        Origin ^

	FIntPoint originGridCoordinate = UEMPCombatStatics::TransformGridCoordinate_LocalToGlobal(rotatedLocalOrigin, CombatSquadOne->CombatAreaLocation);
	//UE_LOG(LogTemp, Warning, TEXT("Origin: %s, Direction: %s"), *originGridCoordinate.ToString(), *SquadOneAttackingDirection.ToString());
	// 
	// Check each column
	for (int lineIndex = 0; lineIndex < 5; lineIndex++)
	{
		FIntPoint lineIndexOffset = UEMPCombatStatics::GetPerpendicularDirection_Clockwise(SquadOneAttackingDirection) * lineIndex;

		UEMPCombatUnitData* mostAdvancedUnit = nullptr;
		bool bIsSpaceAvailableToAdvance = false;
		bool bIsSpaceContested = false;

		// Check from the furthest spot and go back towards the origin
		for (int i = 9; i >= 0; i--)
		{
			mostAdvancedUnit = CombatSquadOne->GetCombatUnitAtCombatLocation((originGridCoordinate + SquadOneAttackingDirection * i) + lineIndexOffset);

			if (mostAdvancedUnit)
			{
				UEMPCombatUnitData* potentialUnitInFront = CombatSquadTwo->GetCombatUnitAtCombatLocation(mostAdvancedUnit->CombatLocation + SquadOneAttackingDirection);
				UEMPCombatUnitData* potentialUnitTwoSpacesInFront = CombatSquadTwo->GetCombatUnitAtCombatLocation(mostAdvancedUnit->CombatLocation + SquadOneAttackingDirection * 2);

				bIsSpaceAvailableToAdvance = potentialUnitInFront == nullptr;
				bIsSpaceContested = potentialUnitTwoSpacesInFront != nullptr;
				break;
			}
		}

		// Handle if no units in column
		bool bHasUnitInColumn = mostAdvancedUnit != nullptr;
		if (!bHasUnitInColumn)
		{
			// Let other team try to advance
			SquadTwoUnitsToAdvance.Append(GetUnitsInLine(CombatSquadTwo, originGridCoordinate, SquadOneAttackingDirection, lineIndex));
		}

		if (bIsSpaceAvailableToAdvance)
		{
			if (!bIsSpaceContested)
			{
				// Advance units in both squads
				SquadOneUnitsToAdvance.Append(GetUnitsInLine(CombatSquadOne, originGridCoordinate, SquadOneAttackingDirection, lineIndex));
				SquadTwoUnitsToAdvance.Append(GetUnitsInLine(CombatSquadTwo, originGridCoordinate, SquadOneAttackingDirection, lineIndex));

			}
			else if (bIsSpaceContested)
			{
				// If contested, roll for advancement (rule can change in future)
				float moveRoll = FMath::RandRange(0.0f, 1.0f);
				if (moveRoll > 0.5f)
				{
					// Move squad one
					SquadOneUnitsToAdvance.Append(GetUnitsInLine(CombatSquadOne, originGridCoordinate, SquadOneAttackingDirection, lineIndex));
				}
				else {
					// Move squad two
					SquadTwoUnitsToAdvance.Append(GetUnitsInLine(CombatSquadTwo, originGridCoordinate, SquadOneAttackingDirection, lineIndex));
				}
			}
		}
	}

	for (UEMPCombatUnitData* unit : SquadOneUnitsToAdvance)
	{
		OwningCombatGameMode->MoveCombatUnitToLocation(unit, unit->CombatLocation + SquadOneAttackingDirection, animationTime);
	}

	for (UEMPCombatUnitData* unit : SquadTwoUnitsToAdvance)
	{
		OwningCombatGameMode->MoveCombatUnitToLocation(unit, unit->CombatLocation + SquadTwoAttackingDirection, animationTime);
	}
}

void UEMPCombatSimulator::ResetCombatUnitsAfterCombat(float animationTime)
{
	FIntPoint squadOneRotatedLocalOrigin = UEMPCombatStatics::RotateLocalGridCoordinate(FIntPoint(0, 0), UEMPCombatStatics::IntPointToDirection(SquadOneAttackingDirection));
	FIntPoint squadOneGlobalOrigin = UEMPCombatStatics::TransformGridCoordinate_LocalToGlobal(squadOneRotatedLocalOrigin, CombatSquadOne->CombatAreaLocation);
	RetreatUnits(CombatSquadOne, squadOneGlobalOrigin, SquadOneAttackingDirection, animationTime);

	FIntPoint squadTwoRotatedLocalOrigin = UEMPCombatStatics::RotateLocalGridCoordinate(FIntPoint(0, 0), UEMPCombatStatics::IntPointToDirection(SquadTwoAttackingDirection));
	FIntPoint squadTwoGlobalOrigin = UEMPCombatStatics::TransformGridCoordinate_LocalToGlobal(squadTwoRotatedLocalOrigin, CombatSquadTwo->CombatAreaLocation);
	RetreatUnits(CombatSquadTwo, squadTwoGlobalOrigin, SquadTwoAttackingDirection, animationTime);

	OwningCombatGameMode->OnCombatEnded.Broadcast(CombatSquadOne, CombatSquadTwo);
}

void UEMPCombatSimulator::RetreatUnits(UEMPSquadData* squad, FIntPoint squadOrigin, FIntPoint attackingDirection, float animationTime)
{
	for (int lineIndex = 0; lineIndex < 5; lineIndex++)
	{
		FIntPoint lineIndexOffset = UEMPCombatStatics::GetPerpendicularDirection_Clockwise(attackingDirection) * lineIndex;
		UEMPCombatUnitData* mostAdvancedUnit = nullptr;

		int32 spacesToRetreat = 0;
		// Check from the furthest spot and go back towards the origin
		for (int i = 9; i >= 5; i--)
		{
			mostAdvancedUnit = squad->GetCombatUnitAtCombatLocation((squadOrigin + attackingDirection * i) + lineIndexOffset);

			if (mostAdvancedUnit)
			{
				spacesToRetreat = i - 4;
				break;
			}
		}

		for (UEMPCombatUnitData* unit : GetUnitsInLine(squad, squadOrigin, attackingDirection, lineIndex))
		{
			// Move backwards the number of spaces to retreat
			OwningCombatGameMode->MoveCombatUnitToLocation(unit, unit->CombatLocation - (attackingDirection * spacesToRetreat), animationTime);
		}
	}
}

TArray<UEMPCombatUnitData*> UEMPCombatSimulator::GetUnitsInLine(UEMPSquadData* squad, FIntPoint origin, FIntPoint attackingDirection, int lineIndex)
{
	TArray<UEMPCombatUnitData*> combatUnitsInLine;
	FIntPoint lineIndexOffset = UEMPCombatStatics::GetPerpendicularDirection_Clockwise(attackingDirection) * lineIndex;

	for (int i = 0; i < 10; i++)
	{
		FIntPoint locationToCheck = (origin + attackingDirection * i) + lineIndexOffset;
		UEMPCombatUnitData* unit = squad->GetCombatUnitAtCombatLocation(locationToCheck);
		if (unit)
		{
			combatUnitsInLine.Add(unit);
		}
	}

	return combatUnitsInLine;
}

void UEMPCombatSimulator::PerformMoraleRolls(float animationTime)
{
	TArray<UEMPSquadData*> squads = TArray<UEMPSquadData*>{ CombatSquadOne, CombatSquadTwo };
	MoraleRolls.Empty();

	for (UEMPSquadData* squad : squads)
	{
		int32 moraleRoll = FMath::RandRange(0, 100);
		OwningCombatGameMode->OnMoraleRoll.Broadcast(squad, moraleRoll, animationTime);
		MoraleRolls.Add(moraleRoll);
	}
}

bool UEMPCombatSimulator::ResolveMoraleRolls(float animationTime)
{
	TArray<UEMPSquadData*> squads = TArray<UEMPSquadData*>{ CombatSquadOne, CombatSquadTwo };
	bool bIsAnimating = false;

	for (int i = 0; i < squads.Num(); i++)
	{
		if (MoraleRolls[i] > squads[i]->GetSquadMorale() || squads[i]->CombatUnitsInSquad.Num() == 0)
		{
			bIsAnimating = true;
			// Retreat squad
			OwningCombatGameMode->RemoveSquadFromCombat(squads[i], animationTime);
		}
	}

	return bIsAnimating;
}