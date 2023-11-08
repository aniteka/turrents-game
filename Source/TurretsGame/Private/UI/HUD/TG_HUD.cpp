// TurretGame by Team #1. AlphaNova courses

#include "UI/HUD/TG_HUD.h"
#include "UI/Widgets/TGMenuWidget.h"
#include "UI/Widgets/TGOverlayWidget.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/BackgroundBlur.h"
#include "GameMode/TGGameMode.h"
#include "GameMode/TGMenuGameMode.h"
#include "Player/TGTank.h"
#include "Player/TGTurret.h"
#include "Player/TGPlayerController.h"

void ATG_HUD::BeginPlay()
{
    Super::BeginPlay();

    SelectWidgetByGameMode();
}

void ATG_HUD::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (UpdateOwnerPawnVar() && OverlayWidget)
    {
        SetPercentBar(OwnerPawn->GetShootDelayPercent(), OverlayWidget->ShootDelayBarImage);
        SetPercentBar(OwnerPawn->GetSpeedPercent(), OverlayWidget->SpeedBarImage);
    }
}

void ATG_HUD::Pause(bool bPaused)
{
    bool bWidgets = OverlayWidget && OverlayWidget->ResumeButton && OverlayWidget->MenuButton && OverlayWidget->PauseText &&
                    OverlayWidget->PauseBlur && OverlayWidget->InstructionImage && OverlayWidget->InstructionBackgroundImage;
    if (!bWidgets) return;

    if (bPaused)
    {
        OverlayWidget->ResumeButton->SetVisibility(ESlateVisibility::Visible);
        OverlayWidget->MenuButton->SetVisibility(ESlateVisibility::Visible);
        OverlayWidget->PauseText->SetVisibility(ESlateVisibility::Visible);
        OverlayWidget->PauseBlur->SetVisibility(ESlateVisibility::Visible);
        OverlayWidget->InstructionImage->SetVisibility(ESlateVisibility::Visible);
        OverlayWidget->InstructionBackgroundImage->SetVisibility(ESlateVisibility::Visible);
    }
    else
    {
        OverlayWidget->ResumeButton->SetVisibility(ESlateVisibility::Hidden);
        OverlayWidget->MenuButton->SetVisibility(ESlateVisibility::Hidden);
        OverlayWidget->PauseText->SetVisibility(ESlateVisibility::Hidden);
        OverlayWidget->PauseBlur->SetVisibility(ESlateVisibility::Hidden);
        OverlayWidget->InstructionImage->SetVisibility(ESlateVisibility::Hidden);
        OverlayWidget->InstructionBackgroundImage->SetVisibility(ESlateVisibility::Hidden);
    }
}

void ATG_HUD::AddMenuWidget()
{
    if (!GetOwningPlayerController()) return;

    MenuWidget = CreateWidget<UTGMenuWidget>(GetOwningPlayerController(), MenuWidgetClass);
    if (!MenuWidget) return;

    MenuWidget->AddToViewport();
}

void ATG_HUD::AddOverlayWidget()
{
    if (!GetOwningPlayerController()) return;

    OverlayWidget = CreateWidget<UTGOverlayWidget>(GetOwningPlayerController(), OverlayWidgetClass);
    if (!OverlayWidget) return;

    OverlayWidget->AddToViewport();
}

void ATG_HUD::OnResumeButtonClicked() 
{
    Pause(false);

    auto TGPlayerController = Cast<ATGPlayerController>(GetOwningPlayerController());
    if (!TGPlayerController) return;

    TGPlayerController->SetInputModeGameOnly();
}

void ATG_HUD::OnMenuButtonClicked()
{
    auto TGPlayerController = Cast<ATGPlayerController>(GetOwningPlayerController());
    if (!TGPlayerController) return;

    TGPlayerController->GoToMenu();
}

void ATG_HUD::SelectWidgetByGameMode() 
{
    if (!GetWorld()) return;

    const auto GameModeMenu = GetWorld()->GetAuthGameMode<ATGMenuGameMode>();
    if (GameModeMenu)
    {
        AddMenuWidget();
    }
    else
    {
        const auto GameMode = GetWorld()->GetAuthGameMode<ATGGameMode>();
        if (GameMode)
        {
            AddOverlayWidget();
            OverlayWidgetCreateHandle();

            if (OverlayWidget && OverlayWidget->ResumeButton && OverlayWidget->MenuButton)
            {
                OverlayWidget->ResumeButton->OnClicked.AddDynamic(this, &ATG_HUD::OnResumeButtonClicked);
                OverlayWidget->MenuButton->OnClicked.AddDynamic(this, &ATG_HUD::OnMenuButtonClicked);
            }
        }
    }
}

void ATG_HUD::OverlayWidgetCreateHandle()
{
    ATGTank* PlayerTank = Cast<ATGTank>(GetOwningPawn());
    if (PlayerTank)
    {
        EnableBar(OverlayWidget->HealthBarImage, true);
        EnableBar(OverlayWidget->ShootDelayBarImage, true);
        EnableBar(OverlayWidget->SpeedBarImage, true);

        SetPercentBar(PlayerTank->GetHealthPercent(), OverlayWidget->HealthBarImage);
    }
    else
    {
        ATGTurret* PlayerTurret = Cast<ATGTurret>(GetOwningPawn());
        if (!PlayerTurret) return;

        EnableBar(OverlayWidget->HealthBarImage, true);
        EnableBar(OverlayWidget->ShootDelayBarImage, true);
    }
}

void ATG_HUD::EnableBar(UImage* BarToEnable, bool bEnable)
{
    if (!OverlayWidget || !BarToEnable) return;
    (bEnable) ? BarToEnable->SetVisibility(ESlateVisibility::Visible) : BarToEnable->SetVisibility(ESlateVisibility::Hidden);
}

void ATG_HUD::SetPercentBar(float Percent, UImage* BarToChange)
{
    if (!OverlayWidget || !BarToChange || BarToChange->GetVisibility() != ESlateVisibility::Visible) return;

    auto DymMaterial = BarToChange->GetDynamicMaterial();
    if (!DymMaterial) return;

    DymMaterial->SetScalarParameterValue(FName(TEXT("ProgressAlpha")), Percent);
}

bool ATG_HUD::UpdateOwnerPawnVar()
{
    OwnerPawn = (!OwnerPawn) ? Cast<ATGBasePawn>(GetOwningPawn()) : OwnerPawn;
    return OwnerPawn ? true : false;
}
