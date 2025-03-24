// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WarriorWeaponBase.generated.h"

class UBoxComponent;

UCLASS()
class WARRIOR_API AWarriorWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AWarriorWeaponBase();
 
protected:
 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons")
	UStaticMeshComponent* WeaponMesh;
 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons")
	UBoxComponent* WeaponCollisionBox;
 
public:
	UBoxComponent* GetWeaponCollisionBox() const { return WeaponCollisionBox;}
	
};
