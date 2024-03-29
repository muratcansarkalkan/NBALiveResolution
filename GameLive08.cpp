#include "plugin-std.h"
#include "Resolutions.h"

using namespace plugin;

const unsigned int RES_X = GetPrivateProfileIntW(L"DISPLAY", L"RES_X", 640, L".\\main.ini");
const unsigned int RES_Y = GetPrivateProfileIntW(L"DISPLAY", L"RES_Y", 480, L".\\main.ini");
const unsigned int INTRO = GetPrivateProfileIntW(L"BOOTUP", L"INTRO", 1, L".\\main.ini");
const float ASPECT_RATIO = static_cast<float>(RES_X) / static_cast<float>(RES_Y);

namespace live08 {

    ResolutionID ids[] = {
        { 640,  480, 32, 0 }, // 640x480x16
        { RES_X,  RES_Y, 32, 1 }, // 640x480x32
        { 1024,  768, 32, 2 }, // 800x600x16
        { 1280,  720, 32, 3 }, // 800x600x32
        { 1280, 1024, 32, 4 },
        { 1366, 768, 32, 5 },
        { 1440,  900, 32, 6 },
        { 1600,  900, 32, 7 },
        { 1600, 1200, 32, 8 },
        { 1680, 1050, 32, 9 },
        { 1920, 1080, 32, 10 },
        { 2560, 1440, 32, 11 },
        { 3440, 1440, 32, 12 }, // 640x480x16
        { 3840, 1080, 32, 13 }, // 640x480x32
        { 3840, 1200, 32, 14 }, // 800x600x16
        { 3840, 1600, 32, 15 }, // 800x600x32
    };

    DWORD* METHOD SetPerspectiveProjection08(
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
        memcpy(_this + 36, (void*)0xC3EB00, 0x40u);
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
        CallMethod<0x43C844>(_this, a2);
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

        v7 = *a1 - patch::GetFloat(0xDAC768);
        v8 = a1[1] - patch::GetFloat(0xDAC76C);
        v9 = a1[2] - patch::GetFloat(0xDAC770);
        v6 = v9;
        if (cameraclip && a2 > 2.0f && a2 < 10.0f)
            v6 = v9 + a2;
        *(float*)&v10 = sqrt(v8 * v8 + v7 * v7 + v6 * v6);
        v11 = -((patch::GetFloat(0xDAC77C) * (1.0f / *(float*)&v10 * v6)
            + patch::GetFloat(0xDAC778) * (v8 * (1.0f / *(float*)&v10))
            + patch::GetFloat(0xDAC774) * (v7 * (1.0f / *(float*)&v10)))
            * *(float*)&v10);
        if (patch::GetFloat(0xDAC758) - a2 <= v11 && patch::GetFloat(0xDAC75C) + a2 >= v11)
        {
            v5 = (patch::GetFloat(0xDAC760) * v11 + a2) * patch::GetFloat(0xDAC764);
            v12 = *(float*)&v10 * *(float*)&v10 - v11 * v11;
            if (v5 * v5 > v12 * 0.3f)
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
    // Loadbar
    float loadYPos = ((400.0f / 480.0f) * RES_Y);
    float loadXPos(float xPos) {
        return ((xPos / 640.0f) * (RES_Y * 1.33333f)) + ((RES_X - (RES_Y * 1.33333f)) / 2);
    }
    // CreationZone
    int czXPos = (int)(((362.0f / 640.0f) * (RES_Y * 1.33333f)) + ((RES_X - (RES_Y * 1.33333f)) / 2));
    int czYPos = (int)(((84.0f / 480.0f) * RES_Y));
    int czWidth = (int)((225.0f * (RES_Y / 480.0f)));
    int czHeight = (int)((303.0f * (RES_Y / 480.0f)));

    static void METHOD ProgressBarCreate(int* _t, DUMMY_ARG, DWORD* a2, int xLocation, int yLocation, int width, int height, int a7, int a8) {
        CallMethod<0x43BC94>(_t, a2, czXPos, czYPos, czWidth, czHeight, a7, a8);
    }

}

void Install_LIVE08() {
    using namespace live08;
    // resolutions
    patch::RedirectJump(0x43B341, SetPerspectiveProjection08);
    patch::RedirectJump(0x69F780, SetTestInConicalFrustum);
    for (const auto& resolution : ids) {
        patch::SetUInt(0xD21F60 + 20 * resolution.id + 4, resolution.width);
        patch::SetUInt(0xD21F60 + 20 * resolution.id + 8, resolution.height);
        patch::SetUInt(0xD21F60 + 20 * resolution.id + 12, resolution.depth);
    }
    patch::SetUInt(0x4145DC + 1, RES_Y);
    patch::SetUInt(0x4145E1 + 1, RES_X);
    patch::SetUInt(0x41F4E3 + 1, RES_Y);
    patch::SetUInt(0x41F4E8 + 1, RES_X);
    patch::SetUInt(0x66AFF0 + 1, RES_Y);
    patch::SetUInt(0x66AFF5 + 1, RES_X);
    patch::SetUInt(0xA12A12 + 1, RES_Y);
    patch::SetUInt(0xA12A17 + 1, RES_X);
    patch::SetUInt(0xE1E48C + 1, RES_Y);
    patch::SetUInt(0xE1E491 + 1, RES_X);
    // loadbar
    patch::SetFloat(0x65F110 + 6, loadYPos);
    patch::SetFloat(0x65F124 + 6, loadYPos);
    patch::SetFloat(0x65F138 + 6, loadYPos);
    patch::SetFloat(0x65F14C + 6, loadYPos);
    patch::SetFloat(0x65F160 + 6, loadYPos);
    patch::SetFloat(0x65F174 + 6, loadYPos);
    patch::SetFloat(0x65F188 + 6, loadYPos);
    patch::SetFloat(0x65F19C + 6, loadXPos(201.0f));
    patch::SetFloat(0x65F1A6 + 6, loadXPos(233.0f));
    patch::SetFloat(0x65F1B0 + 6, loadXPos(265.0f));
    patch::SetFloat(0x65F1BA + 6, loadXPos(297.0f));
    patch::SetFloat(0x65F1C4 + 6, loadXPos(329.0f));
    patch::SetFloat(0x65F1CE + 6, loadXPos(361.0f));
    patch::SetFloat(0x65F1D8 + 6, loadXPos(393.0f));
    // enable/disable intro
    if (INTRO != 1) {
        patch::SetPointer(0x56D8F5 + 1, "DUMMY");
        patch::SetPointer(0x56DAA3 + 1, "DUMMY");
    }
    // CreateZone
    patch::RedirectCall(0x66C667, ProgressBarCreate);
    patch::RedirectCall(0x66C6B7, ProgressBarCreate);
}
