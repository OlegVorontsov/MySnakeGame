// Fill out your copyright notice in the Description page of Project Settings.

#include "BonusLife.h"
#include "MyPawnCamera.h"

DEFINE_LOG_CATEGORY_STATIC(BonusLifeLog, All, All);

// Sets default values
ABonusLife::ABonusLife()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    //создаем root компонент дл€ бонуса
    MyRootComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootBonus"));
    //назначаем root компонент
    RootComponent = MyRootComponent;

    //берем стандартную сферу движка
    BonusLifeMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Engine/BasicShapes/Sphere")).Object;

    //создаем компонент дл€ бонуса
    BonusLifeChank = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bonus"));

    //загружаем сферу бонуса в компонент
    BonusLifeChank->SetStaticMesh(BonusLifeMesh);

    //загружаем цвет дл€ бонуса из движка
    BonusLifeColor = ConstructorHelpers::FObjectFinderOptional<UMaterialInstance>(
        TEXT("MaterialInstanceConstant'/Game/SnakeContent/Materials/InstanceMaterials/"
             "M_Tech_Hex_Tile_Inst.M_Tech_Hex_Tile_Inst'"))
                         .Get();

    //задаем начальную позицию дл€ бонуса
    BonusLifeChank->SetRelativeLocation(FVector(0, 0, 0));
    //задаем цвет бонуса
    BonusLifeChank->SetMaterial(0, BonusLifeColor);
    //крепим бонус к root
    BonusLifeChank->SetupAttachment(MyRootComponent);
    //включаем симул€цию физики
    BonusLifeChank->SetSimulatePhysics(true);

    //добавл€ем реакцию на коллизии
    BonusLifeChank->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    BonusLifeChank->SetCollisionObjectType(ECC_WorldDynamic);
    BonusLifeChank->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
}

// Called when the game starts or when spawned
void ABonusLife::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void ABonusLife::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    TimeBufer += DeltaTime;
    if (TimeBufer > StepDelay)
    {
        Destroy(true, true);
    }
    CollectBonusLife();
}

//функци€ сбора бонуса
void ABonusLife::CollectBonusLife()
{
    //объвл€ем массив куда будем заносить всех с кем столкнулись
    TArray<AActor*> CollectedActors;
    //перебираем всех актеров с кем столкнулись
    GetOverlappingActors(CollectedActors);

    //перебираем массив и увеличиваем скорость змейки
    for (int32 i = 0; i < CollectedActors.Num(); ++i)
    {
        AMySnakeActor* const Test = Cast<AMySnakeActor>(CollectedActors[i]);
        if (Test)
        {
            //прибавл€ем жизнь змейки
            Test->Lives++;

            //увеличиваем переменную подсчета бонусов
            Test->BonusLivesCount++;

            //звук получени€ бонуса
            Test->OnGetBonusSound->Play();

            //уничтожаем бонус
            Destroy(true, true);
            break; // останавливаем цикл так как нашли бонус
        }
    }
}
