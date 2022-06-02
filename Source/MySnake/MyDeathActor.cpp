// Fill out your copyright notice in the Description page of Project Settings.

#include "MyDeathActor.h"
#include "MySnakeActor.h"

// Sets default values
AMyDeathActor::AMyDeathActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    //����� ����������� ��� ������ � ������ �� ���� ������ �����
    class UStaticMesh* WallMesh =
        ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Engine/BasicShapes/Cube")).Object;

    //��������� �������� ��� �����
    WallColor = ConstructorHelpers::FObjectFinderOptional<UMaterialInstance>(
        TEXT("MaterialInstanceConstant'/Game/SnakeContent/Materials/InstanceMaterials/"
             "DangerMaterial_Inst.DangerMaterial_Inst'"))
                    .Get();

    //������� root ��� �����
    MyRootComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
    //��������� root
    RootComponent = MyRootComponent;

    //������� ��������� ���� ��� �����
    WallChank = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WallChank"));
    //��������� � ��������� ���� ����� ��� ������
    WallChank->SetStaticMesh(WallMesh);
    //������ ��� ����� ��� ���������� ����� ��� � root
    WallChank->SetRelativeLocation(FVector(0, 0, 0));
    //������ �������� ��� �����
    WallChank->SetMaterial(0, WallColor);
    //������������ ����� � root
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

//������� �������� � �����
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
