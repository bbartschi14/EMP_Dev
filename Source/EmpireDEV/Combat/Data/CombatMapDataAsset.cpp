// Empire 2021. Ben Bartschi and Jacob Bartschi


#include "CombatMapDataAsset.h"
#include "../../Level/EMPCombatMapGrid.h"
#include "../Objectives/CombatObjective.h"

void UCombatMapDataAsset::CopyFromGrid(AEMPCombatMapGrid* InGrid)
{
	GridSize = InGrid->GetGridDimension();
	SpawnPoints = InGrid->GetSpawnPoints();

	CombatObjectives.Empty();
	for (UCombatObjective* objective : InGrid->GetCombatObjectives())
	{
		const FName DesiredName = objective->GetFName();
		UCombatObjective* newObjective = DuplicateObject(objective, this, DesiredName);
		CombatObjectives.Add(newObjective);
	}
}
