// Empire 2021. Ben Bartschi and Jacob Bartschi


#include "EMPCombatStatics.h"
#include "Structs/FEMPCombatUnitData.h"
#include "Structs/FEMPSquadData.h"

UEMPCombatUnitData* UEMPCombatStatics::FindCombatUnitAtLocation(TArray<UEMPSquadData*> squadsToCheck, FIntPoint combatLocation)
{
	for (UEMPSquadData* squad : squadsToCheck)
	{
		UEMPCombatUnitData* potentialUnit = squad->GetCombatUnitAtCombatLocation(combatLocation);
		if (potentialUnit)
		{
			return potentialUnit;
		}
	}

	return nullptr;
}

// X is UP/DOWN!!!
FIntPoint UEMPCombatStatics::RotateLocalGridCoordinate(FIntPoint localCoordinate, EEMPCombatDirection direction)
{
	FIntPoint rotatedCoordinate;

	// Rotating assumes we are in a 5x5 grid, and we are rotating around the center coordinate, (2,2)
	switch (direction)
	{
	case EEMPCombatDirection::CD_UP:
		rotatedCoordinate = localCoordinate;
		break;
	case EEMPCombatDirection::CD_RIGHT:
		rotatedCoordinate = FIntPoint(4- localCoordinate.Y, localCoordinate.X);
		break;
	case EEMPCombatDirection::CD_DOWN:
		rotatedCoordinate = FIntPoint(4 - localCoordinate.X, 4 - localCoordinate.Y);
		break;
	case EEMPCombatDirection::CD_LEFT:
		rotatedCoordinate = FIntPoint(localCoordinate.Y, 4 - localCoordinate.X);
		break;
	}

	return rotatedCoordinate;
}

FIntPoint UEMPCombatStatics::TransformGridCoordinate_LocalToGlobal(FIntPoint localCoordinate, FIntPoint areaCoordinate)
{
	return FIntPoint(areaCoordinate.X * 5 + localCoordinate.X, areaCoordinate.Y * 5 + localCoordinate.Y);
}

bool UEMPCombatStatics::IsGridCoordinateInArea(FIntPoint gridCoordinate, FIntPoint areaCoordinate)
{
	int32 minX = areaCoordinate.X * 5;
	int32 maxX = minX + 4;
	int32 minY = areaCoordinate.Y * 5;
	int32 maxY = minY + 4;

	return (gridCoordinate.X >= minX && gridCoordinate.X <= maxX && gridCoordinate.Y >= minY && gridCoordinate.Y <= maxY);
}

// X is UP/DOWN!!!
FIntPoint UEMPCombatStatics::GetPerpendicularDirection_Clockwise(FIntPoint inDirection)
{
	return FIntPoint(-inDirection.Y, inDirection.X);
}

FIntPoint UEMPCombatStatics::GetPerpendicularDirection_CounterClockwise(FIntPoint inDirection)
{
	return FIntPoint(inDirection.Y, -inDirection.X);
}

EEMPCombatDirection UEMPCombatStatics::IntPointToDirection(FIntPoint inIntPoint)
{
	check(FMath::Abs(inIntPoint.X) + FMath::Abs(inIntPoint.Y) == 1); // If the IntPoint doesn't have a length of one, it can't be a valid 4 way direction

	if (inIntPoint == FIntPoint(1, 0))
	{
		return EEMPCombatDirection::CD_UP;
	}
	else if (inIntPoint == FIntPoint(0, 1))
	{
		return EEMPCombatDirection::CD_RIGHT;
	}
	else if (inIntPoint == FIntPoint(-1, 0))
	{
		return EEMPCombatDirection::CD_DOWN;
	}
	else if (inIntPoint == FIntPoint(0, -1))
	{
		return EEMPCombatDirection::CD_LEFT;
	}
	
	return EEMPCombatDirection();
}
