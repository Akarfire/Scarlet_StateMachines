// Fill out your copyright notice in the Description page of Project Settings.


#include "SSM_StateBase.h"
#include "SSM_StateMachine.h"

// Constructor
USSM_StateBase::USSM_StateBase() {}

// Called from the state machine
void USSM_StateBase::STATEMACHINE_SetStateMachine(USSM_StateMachine* InStateMachine)
{
    if (StateMachine)
        StateMachine->RemoveState(GetStateID());

    StateMachine = InStateMachine;

    STATEMACHINE_OnSetStateMachine();
}