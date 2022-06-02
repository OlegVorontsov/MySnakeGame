// Fill out your copyright notice in the Description page of Project Settings.

#include "MyDeathActor.h"
#include "MySnakeActor.h"

// Sets default values
AMyDeathActor::AMyDeathActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    //берем стандартный куб движка и делаем из него объект стены
    class UStaticMesh* WallMesh =
        ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Engine/BasicShapes/Cube")).Object;

    //загружаем материал для стены
    WallColor = ConstructorHelpers::FObjectFinderOptional<UMaterialInstance>(
        TEXT("MaterialInstanceConstant'/Game/SnakeContent/Materials/InstanceMaterials/"
             "DangerMaterial_Inst.DangerMaterial_Inst'"))
                    .Get();

    //создаем root для стены
    MyRootComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
    //назначаем root
    RootComponent = MyRootComponent;

    //создаем статичный блок для стены
    WallChank = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WallChank"));
    //загружаем в статичный блок стены куб движка
    WallChank->SetStaticMesh(WallMesh);
    //делаем так чтобы куб создавался тамже где и root
    WallChank->SetRelativeLocation(FVector(0, 0, 0));
    //задаем материал для стены
    WallChank->SetMaterial(0, WallColor);
    //присоединяем стену к root
    WallChank->SetupAttachment(MyRootComponent);

    WallChank->OnComponentBeginOverlap.AddDynamic(this, &AMyDeathActor::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AMyDeathActor::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void AMyDeathActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

//функция оверлапа в стену
void AMyDeathActor::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
    class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
    {
        AMySnakeActor* const Test = Cast<AMySnakeActor>(OtherActor);
        if (Test)
        {
            Test->HaveDamage();
        }
    }
}
