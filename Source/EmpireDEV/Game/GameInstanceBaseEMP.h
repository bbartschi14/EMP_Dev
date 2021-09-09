// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
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

protected:
	void Init() override;


};
