#if PLATFORM_MAC
#include "SyphonServerBridge.h"
#import <Syphon/Syphon.h>
#import <Metal/Metal.h>

static SyphonMetalServer* GTestServer = nil;
static id<MTLDevice> GMetalDevice = nil;
static id<MTLCommandQueue> GQueue = nil;

void FSyphonServerBridge::Start(const FString& ServerName)
{
    if (GTestServer) return;

    GMetalDevice = MTLCreateSystemDefaultDevice();
    GQueue = [GMetalDevice newCommandQueue];

    NSString* Name = [NSString stringWithUTF8String:TCHAR_TO_UTF8(*ServerName)];
    GTestServer = [[SyphonMetalServer alloc] initWithName:Name
                                                   device:GMetalDevice
                                                  options:nil];
}

void FSyphonServerBridge::PublishTexture(void* MetalTexturePtr, int Width, int Height)
{
    if (!GTestServer || !MetalTexturePtr) return;
    id<MTLTexture> Texture = (__bridge id<MTLTexture>)MetalTexturePtr;
    id<MTLCommandBuffer> CB = [GQueue commandBuffer];
    [GTestServer publishFrameTexture:Texture
                     onCommandBuffer:CB
                         imageRegion:NSMakeRect(0, 0, Width, Height)
                             flipped:YES];
    [CB commit];
}

void FSyphonServerBridge::Stop()
{
    [GTestServer stop];
    GTestServer = nil;
    GQueue = nil;
    GMetalDevice = nil;
}
#endif
