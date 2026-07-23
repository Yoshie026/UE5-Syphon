#include "SyphonServerBridge_C.h"
#import <Syphon/Syphon.h>
#import <Metal/Metal.h>

static SyphonMetalServer* GTestServer = nil;
static id<MTLDevice> GMetalDevice = nil;
static id<MTLCommandQueue> GQueue = nil;

void Syphon_Start(const char* ServerName)
{
    if (GTestServer) return;
    GMetalDevice = MTLCreateSystemDefaultDevice();
    GQueue = [GMetalDevice newCommandQueue];
    NSString* Name = [NSString stringWithUTF8String:ServerName];
    GTestServer = [[SyphonMetalServer alloc] initWithName:Name device:GMetalDevice options:nil];
}

void Syphon_PublishTexture(void* MetalTexturePtr, int Width, int Height)
{
    if (!GTestServer || !MetalTexturePtr) return;
    id<MTLTexture> Texture = (__bridge id<MTLTexture>)MetalTexturePtr;
    id<MTLCommandBuffer> CB = [GQueue commandBuffer];
    [GTestServer publishFrameTexture:Texture onCommandBuffer:CB
                         imageRegion:NSMakeRect(0, 0, Width, Height) flipped:YES];
    [CB commit];
}

void Syphon_Stop(void)
{
    [GTestServer stop];
    GTestServer = nil; GQueue = nil; GMetalDevice = nil;
}
