// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstance.h"
#include "MySnakeActor.h"

#include "BonusSpeedHigher.generated.h"

UCLASS()
class MYSNAKE_API ABonusSpeedHigher : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ABonusSpeedHigher();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    //объ€вл€ем сферу коллизии дл€ бонуса
    class USphereComponent* MyRootComponent;

    //объ€вл€ем объект дл€ бонуса
    class UStaticMesh* BonusHigherMesh;

    //объ€вл€ем объект бонуса
    UPROPERTY(EditAnyWhere)
    class UStaticMeshComponent* BonusHigherChank;

    //объ€вл€ем цвет дл€ бонуса
    class UMaterialInstance* BonusHigherColor;

    //функци€ сбора бонусов
    void CollectBonusHigher();

    //ограничение времени жизни бонусов
    float StepDelay = 10.f;
    //накопитель вермени
    float TimeBufer = 0;
};
