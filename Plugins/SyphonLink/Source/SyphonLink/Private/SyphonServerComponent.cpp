#include "SyphonServerComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Camera/CameraComponent.h"
#include "CineCameraComponent.h"
#include "RenderingThread.h"
#if PLATFORM_MAC
#include "SyphonServerBridge.h"
#endif

USyphonServerComponent::USyphonServerComponent()
{
   PrimaryComponentTick.bCanEverTick = true;
}

void USyphonServerComponent::GetResolution(int32 &OutW, int32 &OutH) const
{
   switch (Resolution)
   {
   case ESyphonResolution::R720p:
      OutW = 1280;
      OutH = 720;
      break;
   case ESyphonResolution::R1080p:
      OutW = 1920;
      OutH = 1080;
      break;
   case ESyphonResolution::R1440p:
      OutW = 2560;
      OutH = 1440;
      break;
   case ESyphonResolution::R4K:
      OutW = 3840;
      OutH = 2160;
      break;
   }
}

void USyphonServerComponent::StartServer()
{
#if PLATFORM_MAC
   FSyphonServerBridge::Start(ServerName);

   int32 W, H;
   GetResolution(W, H);

   RenderTarget = NewObject<UTextureRenderTarget2D>(this);
   RenderTarget->RenderTargetFormat = RTF_RGBA8_SRGB;
   RenderTarget->InitAutoFormat(W, H);
   RenderTarget->TargetGamma = 2.2f;
   RenderTarget->UpdateResourceImmediate(true);

   PublishTarget = NewObject<UTextureRenderTarget2D>(this);
   PublishTarget->RenderTargetFormat = RTF_RGBA8_SRGB;
   PublishTarget->InitAutoFormat(W, H);
   PublishTarget->TargetGamma = 2.2f;
   PublishTarget->UpdateResourceImmediate(true);

   SceneCapture = NewObject<USceneCaptureComponent2D>(GetOwner());
   SceneCapture->RegisterComponent();
   SceneCapture->TextureTarget = RenderTarget;
   SceneCapture->CaptureSource = SCS_FinalColorLDR;
   SceneCapture->bCaptureEveryFrame = true;
   SceneCapture->bCaptureOnMovement = false;
   SceneCapture->bAlwaysPersistRenderingState = true;
   SceneCapture->ShowFlags.SetMotionBlur(false);
#endif
}

void USyphonServerComponent::StopServer()
{
#if PLATFORM_MAC
   if (SceneCapture)
   {
      SceneCapture->DestroyComponent();
      SceneCapture = nullptr;
   }
   RenderTarget = nullptr;
   PublishTarget = nullptr;
   FSyphonServerBridge::Stop();
#endif
}

void USyphonServerComponent::BeginPlay()
{
   Super::BeginPlay();
   if (bEnabled)
   {
      StartServer();
   }
}

void USyphonServerComponent::EndPlay(const EEndPlayReason::Type Reason)
{
   StopServer();
   Super::EndPlay(Reason);
}

void USyphonServerComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction *ThisTickFunction)
{
   Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
#if PLATFORM_MAC
   if (!bEnabled || !SceneCapture || !RenderTarget)
   {
      return;
   }

   if (!CameraActor)
   {
      if (bShowDebug && GEngine)
      {
         GEngine->AddOnScreenDebugMessage(1, 0.f, FColor::Red,
                                          TEXT("SyphonServer: NO CAMERA SET"));
      }
      return;
   }

   // Follow the camera COMPONENT (its real lens position), not the actor root
   FVector Loc = CameraActor->GetActorLocation();
   FRotator Rot = CameraActor->GetActorRotation();
   float FOV = 90.f;

   if (UCineCameraComponent *Cine = CameraActor->FindComponentByClass<UCineCameraComponent>())
   {
      Loc = Cine->GetComponentLocation();
      Rot = Cine->GetComponentRotation();
      FOV = Cine->GetHorizontalFieldOfView();
   }
   else if (UCameraComponent *Cam = CameraActor->FindComponentByClass<UCameraComponent>())
   {
      Loc = Cam->GetComponentLocation();
      Rot = Cam->GetComponentRotation();
      FOV = Cam->FieldOfView;
   }

   SceneCapture->SetWorldLocationAndRotation(Loc, Rot);
   SceneCapture->FOVAngle = FOV;

   if (bShowDebug && GEngine)
   {
      GEngine->AddOnScreenDebugMessage(1, 0.f, FColor::Green,
                                       FString::Printf(TEXT("SyphonServer -> %s | FOV %.1f"),
                                                       *CameraActor->GetName(), FOV));
   }

   FTextureRenderTargetResource *SrcRes = RenderTarget->GameThread_GetRenderTargetResource();
   FTextureRenderTargetResource *DstRes = PublishTarget ? PublishTarget->GameThread_GetRenderTargetResource() : nullptr;
   int32 W, H;
   GetResolution(W, H);

   ENQUEUE_RENDER_COMMAND(SyphonPublish)(
       [SrcRes, DstRes, W, H](FRHICommandListImmediate &RHICmdList)
       {
          if (!SrcRes || !DstRes)
          {
             return;
          }
          FRHITexture *Src = SrcRes->GetRenderTargetTexture();
          FRHITexture *Dst = DstRes->GetRenderTargetTexture();
          if (!Src || !Dst)
          {
             return;
          }

          RHICmdList.CopyTexture(Src, Dst, FRHICopyTextureInfo());

          void *Native = Dst->GetNativeResource();
          FSyphonServerBridge::PublishTexture(Native, W, H);
       });
#endif
}

void USyphonServerComponent::SetServerName(const FString &NewName)
{
   ServerName = NewName;
#if PLATFORM_MAC
   if (SceneCapture) // running -> restart under the new name
   {
      FSyphonServerBridge::Stop();
      FSyphonServerBridge::Start(ServerName);
   }
#endif
}