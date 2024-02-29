#include "plugin-std.h"
#include "Resolutions.h"

using namespace plugin;

const unsigned int RES_X = GetPrivateProfileIntW(L"DISPLAY", L"RES_X", 640, L".\\main.ini");
const unsigned int RES_Y = GetPrivateProfileIntW(L"DISPLAY", L"RES_Y", 480, L".\\main.ini");
const float ASPECT_RATIO = static_cast<float>(RES_X) / static_cast<float>(RES_Y);

namespace live07 {

    ResolutionID ids[] = {
        { 640,  480, 32, 0 }, // 640x480x16
        { RES_X,  RES_Y, 32, 1 }, // 640x480x32
        { 1024,  768, 32, 2 }, // 800x600x16
        { 1280,  720, 32, 3 }, // 800x600x32
        { 1280, 1024, 32, 4 },
        { 1366,  768, 32, 5 },
        { 1440,  900, 32, 6 },
        { 1600,  900, 32, 7 },
        { 1920, 1080, 32, 8 },
        { 2560, 1440, 32, 9 },
    };

    DWORD* METHOD SetPerspectiveProjection07(
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
        memcpy(_this + 36, (void*)0xBBC7B0, 0x40u);
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
        CallMethod<0x439FEA>(_this, a2);
        return a2;
    }

    int SetTestInConicalFrustum(float* a1, DUMMY_ARG, float a2, bool cameraclip)
    {
         float v5; // [esp+0h] [ebp-1Ch]
         float v6; // [esp+Ch] [ebp-10h]
         float v7; // [esp+10h] [ebp-Ch]
         float v8; // [esp+14h] [ebp-8h]
         float v9; // [esp+18h] [ebp-4h]
         int v10; // [esp+20h] [ebp+4h]
         float v11; // [esp+28h] [ebp+Ch]
         float v12; // [esp+28h] [ebp+Ch]

        v7 = *a1 - patch::GetFloat(0xCF0614);
        v8 = a1[1] - patch::GetFloat(0xCF0618);
        v9 = a1[2] - patch::GetFloat(0xCF061C);
        v6 = v9;
        if (cameraclip && a2 > 2.0f && a2 < 10.0f)
            v6 = v9 + a2;
        *(float*)&v10 = sqrt(v8 * v8 + v7 * v7 + v6 * v6);
        v11 = -((patch::GetFloat(0xCF0628) * (1.0f / *(float*)&v10 * v6)
            + patch::GetFloat(0xCF0624) * (v8 * (1.0f / *(float*)&v10))
            + patch::GetFloat(0xCF0620) * (v7 * (1.0f / *(float*)&v10)))
            * *(float*)&v10);
        if (patch::GetFloat(0xCF0604) - a2 <= v11 && patch::GetFloat(0xCF0608) + a2 >= v11)
        {
            v5 = (patch::GetFloat(0xCF060C) * v11 + a2) * patch::GetFloat(0xCF0610);
            v12 = *(float*)&v10 * *(float*)&v10 - v11 * v11;
            if (v5 * v5 > v12 * 0.33333f)
            {
                if (!cameraclip)
                    return 1;
                if (a2 >= 10.0f)
                {
                    a2 = a2 + 5.0f;
                }
                else if (0.5f * 0.5f * v5 > v12)
                {
                    return 1;
                }
                if (*(float*)&v10 >= (double)a2)
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

        DWORD* getResolution = (DWORD*)patch::GetPointer(0xD42568);
        DWORD* address = (DWORD*)patch::GetPointer(0xD42584);
        v3 = CallMethodAndReturn<int, 0x435953>((void*)getResolution);
        v5 = CallMethodAndReturn<int, 0x435956>(getResolution);
        invX = (double)v3 * 0.0015625f;
        invY = (double)v5 * 0.0020833334f;
        v10 = -0.5f;
        if (v3 / v5 > 1.3333334f)
        {
            v9 = invY;
            invX = (double)v3 * 0.0011709601f;
            v10 = 107.0f * invX - 0.5f;
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
        memcpy(v6, (const void*)(address + 80), sizeof(v6));;
        CallMethod<0x43A472>(address, &v3, v6);
        CallMethod<0x439024>(address, &v3, v4);
        if (a1)
        {
            v3 = -1;
            CallMethod<0x438F77>(address, &v3, 0xFFFFFFFF, 1);
        }
        Call<0x88B440>();
        return CallMethodAndReturn<DWORD, 0x439024>(address, &v3, v6);
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

        DWORD* getResolution = (DWORD*)patch::GetPointer(0xD42568);

        int* pMouseDevice = *raw_ptr<int*>(_this, 0xC);
        result = *pMouseDevice;

        if (pMouseDevice)
        {
            v3 = CallAndReturn<int*, 0x65C090>();
            v4 = CallVirtualMethodAndReturn<int, 2>(pMouseDevice, 3);
            v12 = CallVirtualMethodAndReturn<int, 2>(pMouseDevice, 4);
            int* pBroadcast = *raw_ptr<int*>(v3, 0x4);
            int height = CallMethodAndReturn<int, 0x435956>(getResolution);
            int width = CallMethodAndReturn<int, 0x435953>(getResolution);
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
                CallVirtualMethod<1>(v3, 133, 0, v5, v8, 0);
            }
            else if (CallVirtualMethodAndReturn<int, 15>(pMouseDevice, 1))
            {
                CallMethod<0x657D30>(v3, 134, 0, v5, v8, 0);
            }
            if (CallVirtualMethodAndReturn<int, 14>(pMouseDevice, 0))
            {
                CallVirtualMethod<1>(v3, 135, 0, v5, v8, 0);
            }
            else if (CallVirtualMethodAndReturn<int, 15>(pMouseDevice, 0))
            {
                CallMethod<0x657D30>(v3, 136, 0, v5, v8, 0);
            }
            q6 = CallVirtualMethodAndReturn<int, 2>(pMouseDevice, 0) != 0;
            if (CallVirtualMethodAndReturn<int, 2>(pMouseDevice, 2))
                q6 |= 4u;
            if (CallVirtualMethodAndReturn<int, 2>(pMouseDevice, 1))
                q6 |= 2u;
            return CallVirtualMethodAndReturn<int, 1>(v3, 131, 0, v5, v8, q6);
        }
        return result;
    }
}

void Install_LIVE07() {
    using namespace live07;
    patch::RedirectJump(0x438B11, SetPerspectiveProjection07);
    patch::RedirectJump(0x67BE80, SetTestInConicalFrustum);
    for (const auto& resolution : ids) {
        patch::SetUInt(0xC65CA0 + 20 * resolution.id + 4, resolution.width);
        patch::SetUInt(0xC65CA0 + 20 * resolution.id + 8, resolution.height);
        patch::SetUInt(0xC65CA0 + 20 * resolution.id + 12, resolution.depth);
    }
    patch::RedirectJump(0x4EAFA0, FEAptInterface_Render);
    patch::RedirectJump(0x65D820, BroadcastMouseInput);
    patch::SetUInt(0x414CFC + 1, RES_Y);
    patch::SetUInt(0x414D01 + 1, RES_X);
    patch::SetUInt(0x41E310 + 1, RES_Y);
    patch::SetUInt(0x41E315 + 1, RES_X);
    patch::SetUInt(0x42C47D + 1, RES_Y);
    patch::SetUInt(0x42C482 + 1, RES_X);
    patch::SetUInt(0x64AB40 + 1, RES_Y);
    patch::SetUInt(0x64AB45 + 1, RES_X);
    patch::SetUInt(0x9D91C2 + 1, RES_Y);
    patch::SetUInt(0x9D91C7 + 1, RES_X);
}
