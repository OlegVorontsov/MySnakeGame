// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPawnCamera.h"
#include "MySnakeActor.h"
#include "MyAppleActor.h"
#include "BonusSpeedHigher.h"
#include "BonusSpeedLower.h"
#include "BonusLife.h"
#include "MyDeathActor.h"

DEFINE_LOG_CATEGORY_STATIC(MyPawnCameraLog, All, All);

// Sets default values
AMyPawnCamera::AMyPawnCamera()
{
    // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    MyRootComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("RootModel"));
    RootComponent = MyRootComponent;

    CameraSpring = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpring"));
    CameraSpring->SetRelativeLocation(FVector(0, 0, 0));
    CameraSpring->AddRelativeRotation(FRotator(-90.f, 0, 0));
    CameraSpring->TargetArmLength = 1700.0f;
    CameraSpring->bDoCollisionTest = false;
    CameraSpring->SetupAttachment(MyRootComponent);

    MyCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    MyCamera->SetupAttachment(CameraSpring, USpringArmComponent::SocketName);

    //перехватываем управление от 0 игрока
    AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void AMyPawnCamera::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void AMyPawnCamera::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    //если игровой режим > 0 то добавляем яблоки/бонусы
    if (GameMode > 0)
    {
        BuferTimeApple += DeltaTime;
        BuferTimeBonus += DeltaTime;

        if (BuferTimeApple > DelaySpawnApple)
        {
            //проверям в паузе ли игра
            if (!GamePause)
            {
                AddRandomApple();
                BuferTimeApple = 0;
            }
        }
        if (BuferTimeBonus > DelaySpawnBonus)
        {
            //проверям в паузе ли игра
            if (!GamePause)
            {
                AddRandomBonus();
                BuferTimeBonus = 0;
            }
        }
    }
}

// Called to bind functionality to input
void AMyPawnCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    InputComponent->BindAction("Pause", IE_Pressed, this, &AMyPawnCamera::GameOnPause);

    //получаем команды при нажатии кнопок W S A D
    InputComponent->BindAxis("KeyMapMove", this, &AMyPawnCamera::FMove);
}

//функция добавляет змейку на карту
void AMyPawnCamera::AddSnakeToMap()
{
    //получаем стартовую позицию где расположено начало игры (RootModel)
    FVector StartPoint = GetActorLocation();
    //получаем поворот нашей пешки
    FRotator StartPointRotation = GetActorRotation();

    //если карта существует то ставим туда нашу змейку
    if (GetWorld())
    {
        MySnakePlayer = GetWorld()->SpawnActor<AMySnakeActor>(StartPoint, StartPointRotation);

        GameMode = 1;
        //указываем что создаваемая змейка - пешка
        MySnakePlayer->WhoPawn = this;

        MySnakePlayer->Lives = MySnakePlayer->Lives + LivesAdd - LivesGetter;
        // UE_LOG(MyPawnCameraLog, Display, TEXT("Lives: %d"), GetLives());
    }
}

//функция обрабатывает клавиши нажатые игроком
void AMyPawnCamera::FMove(float ButtonValue)
{
    //получаем целое число от кнопки
    int32 Key = ButtonValue;

    //если игровой режим > 0 то получаем команды от игрока
    if (GameMode > 0)
    {
        //проверям в паузе ли игра. если да останавливаем змейку
        if (!GamePause)
        {
            //проверяем что нажал игрок
            switch (Key)
            {
                // W
            case 1:
                if (WSAD.X != 1) //проверка на движение в обратную сторону
                {
                    WSAD = FVector2D(0, 0);
                    WSAD.X = -1;
                }
                break;
                // S
            case 2:
                if (WSAD.X != -1)
                {
                    WSAD = FVector2D(0, 0);
                    WSAD.X = 1;
                }
                break;
                // A
            case 3:
                if (WSAD.Y != -1)
                {
                    WSAD = FVector2D(0, 0);
                    WSAD.Y = 1;
                }
                break;
                // D
            case 4:
                if (WSAD.Y != 1)
                {
                    WSAD = FVector2D(0, 0);
                    WSAD.Y = -1;
                }
                break;
            }
            //проверяем не умерла ли змейка
            if (MySnakePlayer)
            {
                MySnakePlayer->DirectionMoveSnake = WSAD;
            }
        }
        else
        {
            MySnakePlayer->DirectionMoveSnake = FVector2D(0, 0);
        }
    }
}

//функция создания яблок в случайных местах карты
void AMyPawnCamera::AddRandomApple()
{
    //задание поворота для яблока
    FRotator StartPointRotation = FRotator(0, 0, 0);

    //получаем случайную точку по Х для создания яблока
    float SpawnX = FMath::FRandRange(MinX, MaxX);
    //получаем случайную точку по Y для создания яблока
    float SpawnY = FMath::FRandRange(MinY, MaxY);

    //создаем точку для создания яблока
    FVector StartPoint = FVector(SpawnX, SpawnY, SpawnZ);

    //если игрок существует то создаем яблоки
    if (MySnakePlayer)
    {
        //проверяем существует ли карта
        if (GetWorld())
        {
            GetWorld()->SpawnActor<AMyAppleActor>(StartPoint, StartPointRotation);
        }
    }
}

//функция создания бонусов в случайных местах карты
void AMyPawnCamera::AddRandomBonus()
{
    //задание поворота для бонуса
    FRotator StartPointRotation = FRotator(0, 0, 0);

    //получаем случайные точки по Х для создания бонусов
    float SpawnXFirst = FMath::FRandRange(MinX, MaxX);
    float SpawnXSecond = FMath::FRandRange(MinX, MaxX);
    float SpawnXThird = FMath::FRandRange(MinX, MaxX);

    //получаем случайные точки по Y для создания бонусов
    float SpawnYFirst = FMath::FRandRange(MinY, MaxY);
    float SpawnYSecond = FMath::FRandRange(MinY, MaxY);
    float SpawnYThird = FMath::FRandRange(MinY, MaxY);

    //создаем точки для создания бонусов
    FVector StartPointFirst = FVector(SpawnXFirst, SpawnYFirst, SpawnZ);
    FVector StartPointSecond = FVector(SpawnXSecond, SpawnYSecond, SpawnZ);
    FVector StartPointThird = FVector(SpawnXThird, SpawnYThird, SpawnZ);

    //если игрок существует то создаем бонус
    if (MySnakePlayer)
    {
        //проверяем существует ли карта
        if (GetWorld())
        {
            GetWorld()->SpawnActor<ABonusSpeedHigher>(StartPointFirst, StartPointRotation);
            GetWorld()->SpawnActor<ABonusSpeedLower>(StartPointSecond, StartPointRotation);
            GetWorld()->SpawnActor<ABonusLife>(StartPointThird, StartPointRotation);
        }
    }
}

//функция возвращает количество съеденных яблок
int32 AMyPawnCamera::GetScore()
{
    if (MySnakePlayer)
    {
        return MySnakePlayer->Score;
    }
    return 0;
}

//функция возвращает количество съеденных бонусов на жизнь
int32 AMyPawnCamera::GetBonusLivesCount()
{
    if (MySnakePlayer)
    {
        return MySnakePlayer->BonusLivesCount;
    }
    return 0;
}

//функция возвращает количество жизней змейки
int32 AMyPawnCamera::GetLives()
{
    if (MySnakePlayer)
    {
        return MySnakePlayer->Lives;
    }
    return 0;
}

//функция перевода игры в паузу
void AMyPawnCamera::GameOnPause()
{
    GamePause = !GamePause;
}

//функция получения урона змейкой
void AMyPawnCamera::SnakeDestroy()
{
    //уничтожаем змейку если она существует и если не осталось жизней
    if ((MySnakePlayer) && (GetLives() <= 0))
    {
        //переводим игру в режим начала игры
        GameMode = 0;
        MySnakePlayer->Destroy(true, true);
    }
    else
    {
        LivesGetter++;
        LivesAdd = MySnakePlayer->BonusLivesCount;
        MySnakePlayer->Destroy(true, true);
        AddSnakeToMap();
    }
}
