#include "plugin-std.h"
#include "Resolutions.h"
#include "AdjustWSUI.h"

using namespace plugin;

// Call GetPrivateProfileInt to retrieve the integer value
const unsigned int RES_X = GetPrivateProfileIntW(L"DISPLAY", L"RES_X", 640, L".\\main.ini");
const unsigned int RES_Y = GetPrivateProfileIntW(L"DISPLAY", L"RES_Y", 480, L".\\main.ini");
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

    // Changes resolution after exiting game but remains as sample ASM injection
    void __declspec(naked) OnSetArrangeWindow3() {
        __asm {
            push RES_Y
            push RES_X
            mov  ecx, 0x5F85DF
            jmp  ecx
        }
    }

    static void METHOD SetViewPortMovie1(float* _t, DUMMY_ARG, DWORD* a2, int xOffset, int yOffset, int w, int h, int nearP, int farP) {
        CallMethod<0x6ED274>(_t, a2, xOffset, yOffset, RES_X, RES_Y, nearP, farP);
    }

    DWORD METHOD FEAptInterface_Render(DWORD* t, DUMMY_ARG, char a1)
    {
        int v2; // [esp+Ch] [ebp-94h] BYREF
        float v3[17]; // [esp+10h] [ebp-90h] BYREF
        float v4; // [esp+54h] [ebp-4Ch]
        char v5[64]; // [esp+60h] [ebp-40h] BYREF
        float invX;
        float invY;
        float v6;
        float v9;
        float v10;

        v2 = CallAndReturn<int, 0x7B0C70>(); // 480
        v4 = CallAndReturn<int, 0x7B0CA0>(); // 640
        invX = (double)v2 * 0.0015625f; // 640
        invY = (double)v4 * 0.0020833334f; // 480
        v10 = -0.5f;
        if (v2 / v4 > 1.3333334)
        {
            v9 = invY;
            invX = (double)v2 * 0.0011709601f;
            v10 = 107.0 * invX - 0.5;
        }
        v3[0] = invX;
        v3[5] = invY;
        memset(&v3[1], 0, 16);
        memset(&v3[6], 0, 16);
        v3[10] = 1.0f;
        v3[11] = 0.0f;
        v3[12] = v10;
        v3[13] = -0.5f;
        v3[14] = -100.0f;
        v3[15] = 1.0f;
        //DWORD* address = (DWORD*)patch::GetPointer(0xC49D70);
        void* camera = *raw_ptr<void*>(*(void**)0xC49D70, 28);
        CallMethod<0x6E536A>(camera, &v2, v5);
        CallMethod<0x6ECE34>(camera, &v2, v3);
        if (a1)
        {
            CallMethod<0x6ECD87>(camera, (DWORD*)&v2, 0xFFFFFFFF, 5);
        }
        Call<0x7665D0>();
        return CallMethodAndReturn<DWORD, 0x6ECE34>(camera, &v2, v5);
    }

    int METHOD BroadcastMouseInput(int* _this)
    {
        int result; // eax
        int* v3; // edi
        int v4; // ebx
        int v5; // ebx
        int q6; // ebp
        int v7; // [esp+14h] [ebp-18h]
        int v8; // [esp+24h] [ebp-8h]
        int v12;

        int* pMouseDevice = *raw_ptr<int*>(_this, 0xC);
        result = *pMouseDevice;

        if (pMouseDevice)
        {
            v3 = CallAndReturn<int*, 0x6160E0>();
            v4 = CallVirtualMethodAndReturn<int, 2>(pMouseDevice, 3); // pMouseDevice->PeekInput(3)
            v12 = CallVirtualMethodAndReturn<int, 2>(pMouseDevice, 4);
            int* pBroadcast = *raw_ptr<int*>(v3, 0x4);
            int height = CallAndReturn<int, 0x7B0CA0>();
            int width = CallAndReturn<int, 0x7B0C70>();
            double fWidth = (double)width;
            double fHeight = (double)height;
            if ((fWidth / fHeight) > 1.3333334f)
            {
                v7 = (int)(fWidth * 0.0011709601f * 107.0f);
                width -= 2 * v7;
                v4 -= v7;
            }
            v5 = 640 * v4 / width;
            v8 = 480 * v12 / height;
            if (CallVirtualMethodAndReturn<int, 14>(pMouseDevice, 1))
            {
                CallVirtualMethod<1>(v3, 130, 0, v5, v8, 0);
            }
            else if (CallVirtualMethodAndReturn<int, 15>(pMouseDevice, 1))
            {
                CallMethod<0x611E00>(v3, 131, 0, v5, v8, 0);
            }
            if (CallVirtualMethodAndReturn<int, 14>(pMouseDevice, 0))
            {
                CallVirtualMethod<1>(v3, 132, 0, v5, v8, 0);
            }
            else if (CallVirtualMethodAndReturn<int, 15>(pMouseDevice, 0))
            {
                CallMethod<0x611E00>(v3, 133, 0, v5, v8, 0);
            }
            q6 = CallVirtualMethodAndReturn<int, 2>(pMouseDevice, 0) != 0;
            if (CallVirtualMethodAndReturn<int, 2>(pMouseDevice, 2))
                q6 |= 4u;
            if (CallVirtualMethodAndReturn<int, 2>(pMouseDevice, 1))
                q6 |= 2u;
            return CallVirtualMethodAndReturn<int, 1>(v3, 128, 0, v5, v8, q6);
        }
        return result;
    }
}

void Install_LIVE2005() {
    using namespace live2005;
    patch::RedirectJump(0x6EC921, SetPerspectiveProjection05);
    patch::RedirectJump(0x728050, SetTestInConicalFrustum);
    // Changes size of menu, intro.
    patch::SetUInt(0x741678 + 1, RES_Y);
    patch::SetUInt(0x74167D + 1, RES_X);
    // Changes size of external window.
    patch::SetUInt(0x4150EC + 1, RES_Y);
    patch::SetUInt(0x4150F1 + 1, RES_X);
    // Changes resolution after exiting game
    patch::SetUInt(0x5F85D5 + 1, RES_Y);
    patch::SetUInt(0x5F85DA + 1, RES_X);
    patch::SetUInt(0x5F2BA6 + 1, RES_Y);
    patch::SetUInt(0x5F2BAB + 1, RES_X);
    // modify resolution addresses
    for (const auto& resolution : ids) {
        patch::SetUInt(0xBDF758 + 20 * resolution.id + 4, resolution.width);
        patch::SetUInt(0xBDF758 + 20 * resolution.id + 8, resolution.height);
        patch::SetUInt(0xBDF758 + 20 * resolution.id + 12, resolution.depth);
    };
    patch::RedirectJump(0x4C5560, FEAptInterface_Render);
    patch::RedirectJump(0x617660, BroadcastMouseInput);

    std::string pathA = ".\\assets\\05WSUI"; // Replace with the actual path 'a'
    std::string pathB = ".\\sgsm"; // Replace with the actual path 'b'

    adjustWSUI::adjustWidescreenUI(ASPECT_RATIO, pathA, pathB);

    // Loadbar
    // Movies
}
