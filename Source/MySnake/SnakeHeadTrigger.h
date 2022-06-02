// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/SphereComponent.h"

#include "SnakeHeadTrigger.generated.h"

UCLASS()
class MYSNAKE_API ASnakeHeadTrigger : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ASnakeHeadTrigger();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    class USphereComponent* MyRootComponent;

    //проверка реакции головы змейки на коллизии
    UPROPERTY(VisibleAnywhere, Category = "Trigger Sphere")
    class USphereComponent* TriggerSphere;

    UFUNCTION()
    void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
        class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
