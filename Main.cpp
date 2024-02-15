#include "plugin-std.h"
#include "Resolutions.h"
#include "Games.h"

using namespace plugin;

void NBAResolution0508() {
        switch (FM::GetEntryPoint()) {
        case 0xCD8005: // NBA Live 2005 1.0 NOCD
        {
            Install_LIVE2005();
            break;
        }
    case 0x40109F:
    {
        if (patch::GetFloat(0xBD832C) == 1.3333334f) { // NBA Live 06 1.0 NOCD
            Install_LIVE06();
            break;
        }
        else if (patch::GetFloat(0xBBBC3C) == 1.3333334f) { // NBA Live 06 1.0 NOCD
            Install_LIVE07();
            break;
        }
        else if (patch::GetFloat(0xC3DF84) == 1.3333334f) { // NBA Live 08 1.0 NOCD
            Install_LIVE08();
            break;
        }
    }
    }
}

class NBALive {
public:
    NBALive() {
        NBAResolution0508();
    }
} screenResolution;
