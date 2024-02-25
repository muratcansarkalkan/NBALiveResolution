#include "plugin-std.h"
#include "Resolutions.h"

using namespace plugin;

const unsigned int RES_X = GetPrivateProfileIntW(L"MAIN", L"RES_X", 640, L".\\wndmode.ini");
const unsigned int RES_Y = GetPrivateProfileIntW(L"MAIN", L"RES_Y", 480, L".\\wndmode.ini");
const float ASPECT_RATIO = static_cast<float>(RES_X) / static_cast<float>(RES_Y);

namespace live06 {

    ResolutionID ids[] = {
        { 640,  480, 32, 0 }, // 640x480x16
        { 800,  600, 32, 1 }, // 640x480x32
        { 1024,  768, 32, 2 }, // 800x600x16
        { 1280,  720, 32, 3 }, // 800x600x32
        { 1280, 1024, 32, 4 },
        { 1366,  768, 32, 5 },
        { 1440,  900, 32, 6 },
        { 1600,  900, 32, 7 },
        { 1920, 1080, 32, 8 },
        { 2560, 1440, 32, 9 },
    };
    DWORD* METHOD SetPerspectiveProjection06(
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
        float h; // [esp+Ch] [ebp+Ch]

        _this[15] = 0.0f;
        _this[6] = fovY;
        // _this[7] = aspectRatio;
        _this[8] = nearClip;
        _this[9] = farClip * 2.0f;
        memcpy(_this + 36, (void*)0xBE9DC0, 0x40u);
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
        CallMethod<0x702DF1>(_this, a2);
        return a2;
    }

    int METHOD SetTestInConicalFrustum(float* _this, DUMMY_ARG, float* a2, float radius, bool cameraclip)
    {
        float v6;
        float v7;
        float v8;
        float v9;
        float v10;
        int a2a;
        float cameraclipa;
        float cameraclipb;

        v8 = *a2 - _this[4];
        v9 = a2[1] - _this[5];
        v10 = a2[2] - _this[6];
        v7 = v10;
        if (cameraclip && radius > 2.0f && radius < 10.0f)
            v7 = v10 + radius;
        a2a = sqrt(v9 * v9 + v8 * v8 + v7 * v7);
        cameraclipa = -((1.0f / a2a * v7 * _this[9] + v9 * (1.0f / a2a) * _this[8] + v8 * (1.0f / a2a) * _this[7]) * a2a);
        if (*_this - radius <= cameraclipa && radius + _this[1] >= cameraclipa)
        {
            v6 = (cameraclipa * _this[2] + radius) * _this[3];
            cameraclipb = a2a * a2a - cameraclipa * cameraclipa;
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

    DWORD METHOD FEAptInterface_Render(DWORD* t, DUMMY_ARG, char a1, int a2)
    {
        int v3;
        float v4[17];
        float v5;
        char v6[64];
        float invX;
        float invY;
        float v7;
        float v9;
        float v10;

        v3 = CallAndReturn<int, 0x7D0D10>(); // 480
        v5 = CallAndReturn<int, 0x7D0D40>(); // 640
        invX = (double)v3 * 0.0015625f; // 640
        invY = (double)v5 * 0.0020833334f; // 480
        v10 = -0.5f;
        if (v3 / v5 > 1.3333334f)
        {
            v9 = invY;
            invX = (double)v3 * 0.0011709601f;
            v10 = 107.0 * invX - 0.5;
        }
        v4[0] = invX;
        v4[5] = invY;
        memset(&v4[1], 0, 16);
        memset(&v4[6], 0, 16);
        v4[10] = 1.0f;
        v4[11] = 0.0f;
        v4[12] = v10;
        v4[13] = -0.5f;
        v4[14] = -100.0f;
        v4[15] = 1.0f;
        //DWORD* address = (DWORD*)patch::GetPointer(0xC49D70);
        void* camera = *raw_ptr<void*>(*(void**)0xCB2A6C, 28);
        CallMethod<0x6FBF14>(camera, &v3, v6);
        CallMethod<0x702731>(camera, &v3, v4);
        if (a1)
        {
            CallMethod<0x702684>(camera, &v3, 0xFFFFFFFF, 1);
        }
        Call<0x7ABA00>();
        return CallMethodAndReturn<DWORD, 0x702731>(camera, &v3, v6);
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
            v3 = CallAndReturn<int*, 0x624AC0>();
            v4 = CallVirtualMethodAndReturn<int, 2>(pMouseDevice, 3); // pMouseDevice->PeekInput(3)
            v12 = CallVirtualMethodAndReturn<int, 2>(pMouseDevice, 4);
            int* pBroadcast = *raw_ptr<int*>(v3, 0x4);
            int height = CallAndReturn<int, 0x7D0D40>();
            int width = CallAndReturn<int, 0x7D0D10>();
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
                CallMethod<0x620D10>(v3, 131, 0, v5, v8, 0);
            }
            if (CallVirtualMethodAndReturn<int, 14>(pMouseDevice, 0))
            {
                CallVirtualMethod<1>(v3, 132, 0, v5, v8, 0);
            }
            else if (CallVirtualMethodAndReturn<int, 15>(pMouseDevice, 0))
            {
                CallMethod<0x620D10>(v3, 133, 0, v5, v8, 0);
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

void Install_LIVE06() {
    using namespace live06;
    patch::RedirectJump(0x70221E, SetPerspectiveProjection06);
    patch::RedirectJump(0x73E3C0, SetTestInConicalFrustum);
    for (const auto& resolution : ids) {
        patch::SetUInt(0xC4CF38 + 20 * resolution.id + 4, resolution.width);
        patch::SetUInt(0xC4CF38 + 20 * resolution.id + 8, resolution.height);
        patch::SetUInt(0xC4CF38 + 20 * resolution.id + 12, resolution.depth);
    }
    patch::SetUInt(0x414C6C + 1, RES_Y);
    patch::SetUInt(0x414C71 + 1, RES_X);
    patch::SetUInt(0x41EAB0 + 1, RES_Y);
    patch::SetUInt(0x41EAB5 + 1, RES_X);
    patch::SetUInt(0x600216 + 1, RES_Y);
    patch::SetUInt(0x60021B + 1, RES_X);
    patch::SetUInt(0x606685 + 1, RES_Y);
    patch::SetUInt(0x60668A + 1, RES_X);
    patch::SetUInt(0x6F2EC5 + 1, RES_Y);
    patch::SetUInt(0x6F2ECA + 1, RES_X);
    patch::SetUInt(0x755E48 + 1, RES_Y);
    patch::SetUInt(0x755E4D + 1, RES_X);
    patch::SetUInt(0x7D1D6D + 1, RES_Y);
    patch::SetUInt(0x7D1D72 + 1, RES_X);
    patch::SetUInt(0x7D1DA2 + 1, RES_Y);
    patch::SetUInt(0x7D1DA7 + 1, RES_X);
    patch::SetUInt(0xC4CEDC, RES_X);
    patch::SetUInt(0xC4CEE0, RES_Y);
    patch::RedirectJump(0x4BC4F0, FEAptInterface_Render);
    patch::RedirectJump(0x626600, BroadcastMouseInput);
}
