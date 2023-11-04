#pragma once

UENUM(BlueprintType)
enum class EGameType : uint8
{
    EGT_PlayTank UMETA(DisplayName = "Play Tank"),
    EGT_PlayTurret UMETA(DisplayName = "Play Turret")
};
