// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "MyPawnCamera.generated.h"

UCLASS()
class MYSNAKE_API AMyPawnCamera : public APawn
{
    GENERATED_BODY()

public:
    // Sets default values for this pawn's properties
    AMyPawnCamera();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UPROPERTY(EditAnywhere)
    class UBoxComponent* MyRootComponent;

    UPROPERTY(EditAnywhere)
    class USpringArmComponent* CameraSpring;

    UPROPERTY(EditAnywhere)
    class UCameraComponent* MyCamera;

    class AMySnakeActor* MySnakePlayer;
    class ASnakeHeadTrigger* HeadTrigger;

    //функция добавляет змейку на экране
    UFUNCTION(BlueprintCallable, Category = "SnakePawn")
    void AddSnakeToMap();

    //направление движения змейки
    FVector2D WSAD;

    //функция для управления змейкой
    void FMove(float ButtonValue);

    //переменные диапазона создания поля яблок/бонусов
    float MinY = -1500.f;
    float MaxY = 1500.f;
    float MinX = -800.f;
    float MaxX = 800.f;
    //точка по оси Z для создания яблок/бонусов
    float SpawnZ = 50.f;

    //функция создания яблок в случайных местах карты
    void AddRandomApple();
    //функция создания бонусов в случайных местах карты
    void AddRandomBonus();

    //задержка для создания яблок
    float DelaySpawnApple = 1.f;
    //задержка для создания бонусов
    float DelaySpawnBonus = 10.f;

    //накопители вермени
    float BuferTimeApple = 0;
    float BuferTimeBonus = 0;

    //переменная игрового меню
    int32 GameMode = 0;

    //простая функция возвращающая значение GameMode
    UFUNCTION(BlueprintCallable, Category = "SnakePawn")
    int32 GetGameMode() const { return GameMode; }

    //функция возвращает количество съеденных яблок
    UFUNCTION(BlueprintCallable, Category = "SnakePawn")
    int32 GetScore();

    //функция возвращает количество жизней змейки
    UFUNCTION(BlueprintCallable, Category = "SnakePawn")
    int32 GetLives();

    //функция возвращает количество съеденных бонусов на жизнь
    UFUNCTION(BlueprintCallable, Category = "SnakePawn")
    int32 GetBonusLivesCount();

    //переменная для убавления жизней змейки
    UPROPERTY(EditAnywhere)
    int32 LivesGetter = 0;

    //переменная для прибавления жизней змейки за счет бонуса
    UPROPERTY(EditAnywhere)
    int32 LivesAdd;

    //переменная для задания паузы в игре
    bool GamePause = false;

    //функция перевода игры в паузу
    UFUNCTION(BlueprintCallable, Category = "SnakePawn")
    void GameOnPause();

    //простая функция возвращающая статус паузы в blueprint
    UFUNCTION(BlueprintCallable, Category = "SnakePawn")
    int32 GetGamePause() const { return GamePause; }

    //функция получения урона змейкой
    void SnakeDestroy();
};
