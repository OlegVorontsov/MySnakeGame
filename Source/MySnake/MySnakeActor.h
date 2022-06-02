// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/AudioComponent.h"

#include "MySnakeActor.generated.h"

UCLASS()
class MYSNAKE_API AMySnakeActor : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AMySnakeActor();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    int32 SnakeSize = 50;
    float StepSnake = 50.f;

    UPROPERTY(EditAnyWhere, BlueprintReadWrite)
    TArray<UStaticMeshComponent*> SnakeBody;

    class USphereComponent* MyRootComponent;

    UPROPERTY(EditAnyWhere, BlueprintReadWrite)
    class USphereComponent* TriggerSphere;

    //функция попадания в TriggerSphere
    UFUNCTION()
    void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
        class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    void CreateSnakeBody();

    //кол-во видимых частей змейки
    UPROPERTY(EditAnywhere)
    int32 VisibleBodyChank = 3;

    //функция определяет сколько элементов видно
    void SetVisibleChank();

    //направление движения змейки
    UPROPERTY(EditAnywhere)
    FVector2D DirectionMoveSnake;

    //задержка в движении
    float StepDelay = 0.5f;

    //накопитель времени
    float TimeBufer = 0;

    //функция движения змейки по координатам
    void MoveSnake();

    //переменная для подсчета съеденных яблок
    int32 Score = 0;

    //переменная для подсчета жизней змейки
    UPROPERTY(EditAnywhere)
    int32 Lives = 5;

    //переменная для подсчета количество съеденных бонусов на жизнь
    UPROPERTY(EditAnywhere)
    int32 BonusLivesCount = 0;

    //кто является пешкой в игре
    class AMyPawnCamera* WhoPawn;

    //функция получения урона змейкой
    void HaveDamage();

    //объвляем звук поедания яблока
    UPROPERTY(EditAnyWhere)
    UAudioComponent* OnEatAppleSound;

    //объвляем звук получения бонуса
    UPROPERTY(EditAnyWhere)
    UAudioComponent* OnGetBonusSound;

    //объвляем звук появления змейки
    UPROPERTY(EditAnyWhere)
    UAudioComponent* SnakeAppearSound;
};
