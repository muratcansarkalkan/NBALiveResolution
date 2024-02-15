#pragma once
#pragma once

struct ResolutionInfo { int flags; int width; int height; int depth; };
struct ResolutionID { int width; int height; int depth; int id; };

const size_t MAX_NUM_RESOLUTIONS = 36;

extern ResolutionInfo resolutions[MAX_NUM_RESOLUTIONS];
extern ResolutionID resolutionIDs[MAX_NUM_RESOLUTIONS];
extern size_t NumResolutions;
extern unsigned int OnOptionResolution1_RetAddr;