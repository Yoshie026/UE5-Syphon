// Copyright 2026 Yoshitsugu Kosaka. All Rights Reserved.
#include "SyphonServerBridge.h"
#if PLATFORM_MAC
#include "SyphonServerBridge_C.h"

void FSyphonServerBridge::Start(const FString& ServerName) { Syphon_Start(TCHAR_TO_UTF8(*ServerName)); }
void FSyphonServerBridge::PublishTexture(void* MetalTexturePtr, int Width, int Height) { Syphon_PublishTexture(MetalTexturePtr, Width, Height); }
void FSyphonServerBridge::Stop() { Syphon_Stop(); }
#endif
