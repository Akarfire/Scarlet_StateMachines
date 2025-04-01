// Fill out your copyright notice in the Description page of Project Settings.


#include "ScarletStateMachines_Utilities.h"

// Creates a new state machine of a given class
USSM_StateMachine* UScarletStateMachines_Utilities::CreateStateMachine(TSubclassOf<USSM_StateMachine> StateMachineClass, UObject* Owner, bool AutoInit)
{
    USSM_StateMachine* NewStateMachine = NewObject<USSM_StateMachine>(Owner, StateMachineClass);

    if (NewStateMachine && AutoInit)
        NewStateMachine->InitStateMachine();

    return NewStateMachine;
}