// Copyright 2026 Yoshitsugu Kosaka. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SyphonServerComponent.generated.h"

UENUM(BlueprintType)
enum class ESyphonResolution : uint8
{
   R720p UMETA(DisplayName = "1280 x 720"),
   R1080p UMETA(DisplayName = "1920 x 1080"),
   R1440p UMETA(DisplayName = "2560 x 1440"),
   R4K UMETA(DisplayName = "3840 x 2160")
};

UCLASS(ClassGroup = (Rendering), meta = (BlueprintSpawnableComponent))
class SYPHONLINK_API USyphonServerComponent : public UActorComponent
{
   GENERATED_BODY()
public:
   USyphonServerComponent();

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Syphon")
   FString ServerName = TEXT("UE5 Output");

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Syphon")
   bool bEnabled = true;

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Syphon")
   TObjectPtr<AActor> CameraActor;

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Syphon")
   ESyphonResolution Resolution = ESyphonResolution::R1080p;

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Syphon")
   bool bShowDebug = false;

   UFUNCTION(BlueprintCallable, Category = "Syphon")
   void StartServer();
   UFUNCTION(BlueprintCallable, Category = "Syphon")
   void StopServer();

   UFUNCTION(BlueprintCallable, Category = "Syphon")
   void SetCamera(AActor *NewCamera) { CameraActor = NewCamera; }

   UFUNCTION(BlueprintCallable, Category = "Syphon")
   void SetServerName(const FString &NewName);

   virtual void BeginPlay() override;
   virtual void EndPlay(const EEndPlayReason::Type Reason) override;
   virtual void TickComponent(float DeltaTime, ELevelTick TickType,
                              FActorComponentTickFunction *ThisTickFunction) override;

private:
   void GetResolution(int32 &OutW, int32 &OutH) const;

   UPROPERTY()
   TObjectPtr<class USceneCaptureComponent2D> SceneCapture;
   UPROPERTY()
   TObjectPtr<class UTextureRenderTarget2D> RenderTarget;
   UPROPERTY()
   TObjectPtr<class UTextureRenderTarget2D> PublishTarget;
};