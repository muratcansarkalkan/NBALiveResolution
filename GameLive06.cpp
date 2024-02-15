#include "plugin-std.h"
#include "Resolutions.h"

using namespace plugin;

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
        int savedregs; // [esp+0h] [ebp+0h] BYREF
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
}
