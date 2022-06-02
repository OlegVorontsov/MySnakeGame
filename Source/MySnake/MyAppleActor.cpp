// Fill out your copyright notice in the Description page of Project Settings.

#include "MyAppleActor.h"

// Sets default values
AMyAppleActor::AMyAppleActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    //создаем root компонент дл€ €блока
    MyRootComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootApple"));
    //назначаем root компонент
    RootComponent = MyRootComponent;

    //берем стандартную сферу движка
    AppleMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Engine/BasicShapes/Sphere")).Object;

    //уменьшение сферы дл€ €блока
    FVector ScaleApple = FVector(0.5f, 0.5f, 0.5f);

    //создаем компонент дл€ еды
    EatChank = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Eat"));

    //загружаем сферу €блока в компонент еды
    EatChank->SetStaticMesh(AppleMesh);

    //загружаем цвет дл€ €блока из движка
    AppleColor = ConstructorHelpers::FObjectFinderOptional<UMaterialInstance>(
        TEXT("MaterialInstanceConstant'/Game/SnakeContent/Materials/InstanceMaterials/"
             "M_Metal_Gold_Inst.M_Metal_Gold_Inst'"))
                     .Get();

    //задаем новый размер дл€ €блока
    EatChank->SetRelativeScale3D(ScaleApple);
    //задаем начальную позицию дл€ €блока
    EatChank->SetRelativeLocation(FVector(0, 0, 0));
    //задаем цвет дл€ €блока
    EatChank->SetMaterial(0, AppleColor);
    //крепим €блоко к root
    EatChank->SetupAttachment(MyRootComponent);
    //включаем симул€цию физики
    EatChank->SetSimulatePhysics(true);

    //добавл€ем реакцию на коллизии
    EatChank->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    EatChank->SetCollisionObjectType(ECC_WorldDynamic);
    EatChank->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
}

// Called when the game starts or when spawned
void AMyAppleActor::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void AMyAppleActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    TimeBufer += DeltaTime;
    if (TimeBufer > StepDelay)
    {
        Destroy(true, true);
    }
}
