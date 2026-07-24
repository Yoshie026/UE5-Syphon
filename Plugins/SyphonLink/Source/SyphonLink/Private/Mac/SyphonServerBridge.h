// Copyright 2026 Yoshitsugu Kosaka. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"

#if PLATFORM_MAC
class FSyphonServerBridge
{
public:
   static void Start(const FString &ServerName);
   static void PublishTexture(void *MetalTexturePtr, int Width, int Height);
   static void Stop();
};
#endif