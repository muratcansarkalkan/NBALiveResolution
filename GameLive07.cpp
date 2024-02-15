#include "plugin-std.h"
#include "Resolutions.h"

using namespace plugin;

namespace live07 {

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
}
