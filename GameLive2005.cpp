#include "plugin-std.h"
#include "Resolutions.h"

using namespace plugin;

// WIP
const unsigned int RES_X = 1920;
const unsigned int RES_Y = 1080;
const float ASPECT_RATIO = static_cast<float>(RES_X) / static_cast<float>(RES_Y);

namespace live2005 {

    ResolutionID ids[] = {
    { 640,  480, 32, 0 }, // 640x480x16
    { 800,  600, 32, 1 }, // 640x480x32
    { 1024,  768, 32, 2 }, // 800x600x16
    { 1280,  720, 32, 3 }, // 800x600x32
    { 1280, 1024, 32, 4 }, // 1024x768x16
    { 1366,  768, 32, 5 }, // 1024x768x32
    { 1440,  900, 32, 6 }, // 1280x1024x16
    { 1600,  900, 32, 7 }, // 1280x1024x32
    { 1920, 1080, 32, 8 }, // 1600x1200x16
    { 2560, 1440, 32, 9 }, // 1600x1200x32
    };

    DWORD* METHOD SetPerspectiveProjection05(
        float* _this, DUMMY_ARG,
        DWORD* a2,
        float fovY,
        float aspectRatio,
        float nearClip,
        float farClip)
    {
        long double fovR; // st7
        double w; // st6
        double dist; // st7
        int savedregs; // [esp+0h] [ebp+0h] BYREF
        float h; // [esp+Ch] [ebp+Ch]

        _this[15] = 0.0f;
        _this[6] = fovY;
        // _this[7] = aspectRatio;
        _this[8] = nearClip;
        _this[9] = farClip * 2.0f;
        memcpy(_this + 36, (void*)0xB837F0, 0x40u);
        fovR = (double)(1.0f / tan(fovY * 0.0087266462f));
        w = (double)*((unsigned int*)_this + 2); // mGeometry.mWidth
        h = (float)*((unsigned int*)_this + 3); // mGeometry.mHeight
        _this[7] = ((float)w / h); // aspectRatio
        _this[47] = (float)-1.0f; // mViewMatrix.m44[2][3]
        _this[36] = (float)(0.5f * w / ((float)w / h) * fovR);  // mViewMatrix.m44[0][0]
        _this[41] = (float)(-(h * 0.5f * fovR)); // mViewMatrix.m44[1][1]
        _this[44] = (float)(w * -0.5f - (double)*(int*)_this); // mViewMatrix.m44[2][0]
        _this[45] = (float)((h * -0.5f - (double)*((int*)_this + 1))); // mViewMatrix.m44[2][1]
        dist = nearClip - farClip;
        _this[46] = (float)(nearClip / dist - 1.0f); // mViewMatrix.m44[2][2]
        _this[50] = (float)(nearClip * farClip / dist); // mViewMatrix.m44[3][2]
        CallMethod<0x6ED4F4>(_this, a2);
        return a2;
    }

    void __declspec(naked) OnSetArrangeScreen() {
        __asm {
            push RES_Y
            push RES_X
            mov  ecx, 0x741682
            jmp  ecx
        }
    }

    void __declspec(naked) OnSetArrangeWindow() {
        __asm {
            push RES_Y
            push RES_X
            mov  ecx, 0x4150F6
            jmp  ecx
        }
    }
    void OnSetWidescreen(int* t, const char setting) {
        if (*(bool*)0x5562F6) // RMHUD::Screen::bWidescreen
            Call<0x547830>(t, "1");
    }

    int METHOD SetTestInConicalFrustum(float* _this, DUMMY_ARG, float* a2, float radius, bool cameraclip)
    {
        float v6; // [esp+0h] [ebp-1Ch]
        float v7; // [esp+Ch] [ebp-10h]
        float v8; // [esp+10h] [ebp-Ch]
        float v9; // [esp+14h] [ebp-8h]
        float v10; // [esp+18h] [ebp-4h]
        float a2a; // [esp+20h] [ebp+4h]
        float cameraclipa; // [esp+28h] [ebp+Ch]
        float cameraclipb; // [esp+28h] [ebp+Ch]

        v8 = *a2 - _this[4];
        v9 = a2[1] - _this[5];
        v10 = a2[2] - _this[6];
        v7 = v10;
        if (cameraclip && radius > 2.0f && radius < 10.0f)
            v7 = v10 + radius;
        a2a = sqrt(v9 * v9 + v8 * v8 + v7 * v7);
        cameraclipa = -((1.0f / a2a * v7 * _this[9] + v9 * (1.0f / a2a) * _this[8] + v8 * (1.0f / a2a) * _this[7]) * a2a);
        // by default, it returns 0 but there are exceptions
        // first one is nearplane case, second one is farplane case
        if (*_this - radius <= cameraclipa && radius + _this[1] >= cameraclipa)
        {
            v6 = (cameraclipa * _this[2] + radius) * _this[3];
            cameraclipb = a2a * a2a - cameraclipa * cameraclipa;
            // original one didn't have 0.5 multiplier
            if (v6 * v6 > cameraclipb * 0.33333f)
            {
                if (!cameraclip)
                    return 1;
                if (radius >= 10.0f)
                {
                    radius = radius + 5.0f;
                }
                else if (0.25f * v6 > cameraclipb)
                {
                    return 1;
                }
                if (a2a >= (double)radius)
                    return 1;
            }
        }
        return 0;
    }
    static void METHOD OnGetProjection(void* t, DUMMY_ARG, DWORD *a2, DWORD *a3, float *fovY, float *aspect, float *nearPlane, float *farPlane) {
        CallMethod<0x6ECCD6>(t, a2, a3, fovY, aspect, nearPlane, farPlane);
    }

    static void METHOD ResolutionSet(int xRight, int yBottom, char* a3, char* a4, int a5, char a6) {
        CallMethod<0x5F2950>(1366, 768, a3, a4, a5, a6);
    }

    int METHOD TestInConicalFrustum(void *t, DUMMY_ARG, void *vector, float radius, bool cameraclip) {
        return CallMethodAndReturn<int, 0x728050>(t, vector, radius, cameraclip);
    }

    static void METHOD EnableGeometry(void *t, DUMMY_ARG, int a2) {
        CallMethod<0x732590>(t, a2);
    }
}

void Install_LIVE2005() {
    using namespace live2005;
    patch::RedirectJump(0x6EC921, SetPerspectiveProjection05);
    //patch::RedirectCall(0x728C4F, TestInConicalFrustum);
    //patch::RedirectCall(0x727FAB, OnGetProjection);
    patch::RedirectJump(0x728050, SetTestInConicalFrustum);
    //patch::RedirectCall(0x7BEF63, EnableGeometry);
    //patch::RedirectCall(0x41E8AA, ResolutionSet);
    //patch::RedirectCall(0x41F02E, ResolutionSet);
    // modify resolution addresses
    for (const auto& resolution : ids) {
        patch::SetUInt(0xBDF758 + 20 * resolution.id + 4, resolution.width);
        patch::SetUInt(0xBDF758 + 20 * resolution.id + 8, resolution.height);
        patch::SetUInt(0xBDF758 + 20 * resolution.id + 12, resolution.depth);
    }
    //patch::RedirectJump(0x741678, OnSetArrangeScreen);
    //patch::RedirectJump(0x4150EC, OnSetArrangeWindow);
}
