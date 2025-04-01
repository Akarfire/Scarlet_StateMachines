// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SSM_StateMachine.h"
#include "ScarletStateMachines_Utilities.generated.h"

/**
 * 
 */
UCLASS()
class SCARLETSTATEMACHINES_API UScarletStateMachines_Utilities : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	// Creates a new state machine of a given class
	UFUNCTION(BlueprintCallable, Category="ScarletStateMachines")
	static USSM_StateMachine* CreateStateMachine(TSubclassOf<USSM_StateMachine> StateMachineClass, UObject* Owner, bool AutoInit = true);

};
