
#include "LevelExit.h"

#include "Kismet/GameplayStatics.h"

#include "PlayerCharacter.h"
#include "CrustyPirateGameInstance.h"


ALevelExit::ALevelExit()
{
	PrimaryActorTick.bCanEverTick = true;
    
    BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
    SetRootComponent(BoxComp);
    
    DoorFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("DoorFlipbook"));
    DoorFlipbook->SetupAttachment(RootComponent);
    
    // Stop the door animation
    DoorFlipbook->SetPlayRate(0.0f);
    DoorFlipbook->SetLooping(false);

}

void ALevelExit::BeginPlay()
{
	Super::BeginPlay();
    
    BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ALevelExit::OverlapBegin);
    
    // Close the door (i,e., go to the first frame)
    DoorFlipbook->SetPlaybackPosition(0.0f, false);
	
}

void ALevelExit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelExit::OverlapBegin(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // Check if the actor that overlaps is the player
    APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
    if (Player && Player->IsAlive)
    {
        if (IsActive)
        {
            // Deactivate the player
            Player->Deactivate();
            
            
            IsActive = false;
            
            // Open the door
            DoorFlipbook->SetPlayRate(1.0f);
            DoorFlipbook->PlayFromStart();
            
            UGameplayStatics::PlaySound2D(GetWorld(), PlayerEnterSound);
            
            // Change levels
            GetWorldTimerManager().SetTimer(WaitTimer, this, &ALevelExit::OnWaitTimerTimeout, 1.0f, false, WaitTimeInSeconds);
        }
    }
}

void ALevelExit::OnWaitTimerTimeout()
{
    // Get the game instance
    UCrustyPirateGameInstance* MyGameInstance = Cast<UCrustyPirateGameInstance>(GetGameInstance());
    if (MyGameInstance)
    {
        MyGameInstance->ChangeLevel(LevelIndex);
    }
}

