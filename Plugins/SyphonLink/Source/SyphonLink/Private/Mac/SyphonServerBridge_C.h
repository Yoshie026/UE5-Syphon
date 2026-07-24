// Copyright 2026 Yoshitsugu Kosaka. All Rights Reserved.
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void Syphon_Start(const char* ServerName);
void Syphon_PublishTexture(void* MetalTexturePtr, int Width, int Height);
void Syphon_Stop(void);

#ifdef __cplusplus
}
#endif
