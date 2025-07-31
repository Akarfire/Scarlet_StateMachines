// Fill out your copyright notice in the Description page of Project Settings.


#include "SSM_StateMachine.h"

// Default constructor
USSM_StateMachine::USSM_StateMachine() {}


// Call this when the state machine is created
void USSM_StateMachine::InitStateMachine()
{
    OnInitStateMachine();
}


// Call this every time the state machine should be updated (for example every frame)
void USSM_StateMachine::UpdateStateMachine(float DeltaTime)
{
    bool TransitionHappened = false;

    if (BufferedNewState != 0)
    {
        StateTransition(BufferedNewState);
        TransitionHappened = true;

        BufferedNewState = 0;
    }

    else if (TransitionMap.Contains(ActiveState) && IsValid(States[ActiveState]))
    {
        if (!States[ActiveState]->IsLocked())
        {
            for (auto& Transition: TransitionMap[ActiveState])
            {

                if (Transition.ConditionDelegate.IsBound() && Transition.ConditionDelegate.Execute())
                {
                    StateTransition(Transition.TargetState);
                    TransitionHappened = true;
                    break;
                }
            }
        }   
    }

    if (!TransitionHappened)
        States[ActiveState]->UpdateState(DeltaTime);

    OnUpdateStateMachine(DeltaTime);
}


// Sets a new active state
void USSM_StateMachine::StateTransition(uint8 InNewState)
{
    uint8 PreviousState = ActiveState;

    if (ActiveState != 0)
        States[ActiveState]->ExitState();

    ActiveState = InNewState;
    States[ActiveState]->EnterState();

    OnStateChanged.Broadcast(PreviousState, InNewState);
}

// Adds a new possible state
void USSM_StateMachine::AddNewStateExisting(uint8 InStateID, USSM_StateBase* InState)
{
    States.Add(InStateID, InState);
    InState->STATEMACHINE_SetStateMachine(this);
}

// Creates a new state from
void USSM_StateMachine::AddNewState(uint8 InStateID, TSubclassOf<USSM_StateBase> InStateClass)
{
    USSM_StateBase* NewState = NewObject<USSM_StateBase>(this, InStateClass);
    AddNewStateExisting(InStateID, NewState);
}

// Removes a state from the state machine
void USSM_StateMachine::RemoveState(uint8 InStateID)
{
    if (InStateID == ActiveState)
        ActiveState = 0;

    if (States.Contains(InStateID))
        States.Remove(InStateID);
}

// Returs a pointer to the requested state object
USSM_StateBase* USSM_StateMachine::GetState(uint8 InStateID)
{
    if (States.Contains(InStateID))
        return States[InStateID];

    return nullptr;
}


// TRANSITIONS

// Registers a new transition between states
void USSM_StateMachine::RegisterTransitionSourced(const FStateTransition& InStateTransition)
{
    FStateTransition NewTransition;

    NewTransition.TargetState = InStateTransition.TargetState;
    NewTransition.ConditionFunctionName = InStateTransition.ConditionFunctionName;
    NewTransition.CondtionFunctionOwner = InStateTransition.CondtionFunctionOwner;
    
    if (!NewTransition.CondtionFunctionOwner)
        NewTransition.CondtionFunctionOwner = this;

    NewTransition.ConditionDelegate.BindUFunction(NewTransition.CondtionFunctionOwner, InStateTransition.ConditionFunctionName);

    if (TransitionMap.Contains(InStateTransition.OriginState))
        TransitionMap[InStateTransition.OriginState].Add(NewTransition);

    else
        TransitionMap.Add(InStateTransition.OriginState, {NewTransition});
}


// Registers a new transition between states, condition function is located inside the state machine
void USSM_StateMachine::RegisterMultipleTransitions(const TArray<FStateTransition>& InStateTransitions)
{
    for (auto& Transition: InStateTransitions)
        RegisterTransitionSourced(Transition);
}

// Automatically finds and registers existing local transition condition functions with the specified naming convention
// Default naming: "Condition_State1_State2"
void USSM_StateMachine::AutoTransitionRegistration(const TArray<FString> StateNames, const FString& ConditionFunctionNamePrefix, const FString& ConditionFunctionNameStateConnector)
{
    for (uint8 State1 = 0; State1 < StateNames.Num(); State1++)
        for (uint8 State2 = 0; State2 < StateNames.Num(); State2++)
        {
            FString ConditionFunctionName = ConditionFunctionNamePrefix + StateNames[State1] + ConditionFunctionNameStateConnector + StateNames[State2];

            RegisterTransitionLocal(State1, State2, FName(ConditionFunctionName));
        }
}