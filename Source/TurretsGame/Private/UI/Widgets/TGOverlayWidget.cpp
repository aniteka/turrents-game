// TurretGame by Team #1. AlphaNova courses

#include "UI/Widgets/TGOverlayWidget.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/BackgroundBlur.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

void UTGOverlayWidget::NativeConstruct()
{
    Super::NativeConstruct();

    HideAllComponents();
    BindButtonsHovered();
}

void UTGOverlayWidget::HideAllComponents()
{
    bool bWidgetsValid = HealthBarImage && ShootDelayBarImage && SpeedBarImage && EnemyHealthBarImage && PauseBlur && PauseText &&
                         ResumeButton && MenuButton && InstructionImage && InstructionBackgroundImage;
    if (!bWidgetsValid) return;

    HealthBarImage->SetVisibility(ESlateVisibility::Hidden);
    ShootDelayBarImage->SetVisibility(ESlateVisibility::Hidden);
    SpeedBarImage->SetVisibility(ESlateVisibility::Hidden);
    EnemyHealthBarImage->SetVisibility(ESlateVisibility::Hidden);
    PauseBlur->SetVisibility(ESlateVisibility::Hidden);
    PauseText->SetVisibility(ESlateVisibility::Hidden);
    ResumeButton->SetVisibility(ESlateVisibility::Hidden);
    MenuButton->SetVisibility(ESlateVisibility::Hidden);
    InstructionImage->SetVisibility(ESlateVisibility::Hidden);
    InstructionBackgroundImage->SetVisibility(ESlateVisibility::Hidden);
}

void UTGOverlayWidget::BindButtonsHovered()
{
    if (!ResumeButton || !MenuButton) return;

    ResumeButton->OnHovered.AddDynamic(this, &UTGOverlayWidget::OnResumeButtonHovered);
    MenuButton->OnHovered.AddDynamic(this, &UTGOverlayWidget::OnMenuButtonHovered);
}

void UTGOverlayWidget::PlayHoveredSound()
{
    if (!HoveredSound) return;
    UGameplayStatics::PlaySound2D(this, HoveredSound);
}

void UTGOverlayWidget::OnResumeButtonHovered()
{
    PlayHoveredSound();
}

void UTGOverlayWidget::OnMenuButtonHovered()
{
    PlayHoveredSound();
}
