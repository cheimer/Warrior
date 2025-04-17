// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/WarriorAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "WarriorDebugHelper.h"
#include "WarriorFunctionLibrary.h"
#include "WarriorGameplayTags.h"
#include "Components/UI/HeroUIComponent.h"
#include "Components/UI/PawnUIComponent.h"
#include "Interfaces/PawnUIInterface.h"

UWarriorAttributeSet::UWarriorAttributeSet()
{
	InitCurrentHealth(1.f);
	InitMaxHealth(1.f);
	InitCurrentRage(1.f);
	InitMaxRage(1.f);
	InitAttackPower(1.f);
	InitDefensePower(1.f);
}

void UWarriorAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	CachedPawnUIInterface = CachedPawnUIInterface.IsValid() ? CachedPawnUIInterface : TWeakInterfacePtr<IPawnUIInterface>(Data.Target.GetAvatarActor());
	checkf(CachedPawnUIInterface.IsValid(),TEXT("%s didn't implement IPawnUIInterface"),*Data.Target.GetAvatarActor()->GetActorNameOrLabel());
 
	UPawnUIComponent* PawnUIComponent = CachedPawnUIInterface->GetPawnUIComponent();
	checkf(PawnUIComponent,TEXT("Couldn't extract a PawnUIComponent from %s"),*Data.Target.GetAvatarActor()->GetActorNameOrLabel());
 
	if (Data.EvaluatedData.Attribute == GetCurrentHealthAttribute())
	{
		const float NewCurrentHealth = FMath::Clamp(GetCurrentHealth(),0.f,GetMaxHealth());
 
		SetCurrentHealth(NewCurrentHealth);
 
 		PawnUIComponent->OnCurrentHealthChanged.Broadcast(GetCurrentHealth()/GetMaxHealth());
	}
 
	if (Data.EvaluatedData.Attribute == GetCurrentRageAttribute())
	{
		const float NewCurrentRage = FMath::Clamp(GetCurrentRage(),0.f,GetMaxRage());
 
		SetCurrentRage(NewCurrentRage);
		
		if (UHeroUIComponent* HeroUIComponent = CachedPawnUIInterface->GetHeroUIComponent())
		{
			HeroUIComponent->OnCurrentRageChanged.Broadcast(GetCurrentRage()/GetMaxRage());
		}
	}
 
	if (Data.EvaluatedData.Attribute == GetDamageTakenAttribute())
	{
		const float OldHealth = GetCurrentHealth();
		const float DamageDone = GetDamageTaken();
 
		const float NewCurrentHealth = FMath::Clamp(OldHealth - DamageDone,0.f,GetMaxHealth());
 
		SetCurrentHealth(NewCurrentHealth);
 
		if (GetCurrentRage() == GetMaxRage())
		{
			UWarriorFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(),WarriorGameplayTags::Player_Status_Rage_Full);
		}
		else if (GetCurrentRage() == 0.f)
		{
			UWarriorFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(),WarriorGameplayTags::Player_Status_Rage_None);
		}
		else
		{
			UWarriorFunctionLibrary::RemoveGameplayTagFromActorIfFound(Data.Target.GetAvatarActor(),WarriorGameplayTags::Player_Status_Rage_Full);
			UWarriorFunctionLibrary::RemoveGameplayTagFromActorIfFound(Data.Target.GetAvatarActor(),WarriorGameplayTags::Player_Status_Rage_None);
		}
		
		PawnUIComponent->OnCurrentHealthChanged.Broadcast(GetCurrentHealth()/GetMaxHealth());
		
		if (GetCurrentHealth() == 0.f)
		{
			UWarriorFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(),WarriorGameplayTags::Shared_Status_Dead);
		}
	}
 	
}
