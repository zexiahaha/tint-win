#include "tint_opacity.h"

int TintClampOpacityPercent(int Percent)
{
    if (Percent < TINT_MIN_OPACITY_PERCENT)
    {
        return TINT_MIN_OPACITY_PERCENT;
    }
    if (Percent > TINT_MAX_OPACITY_PERCENT)
    {
        return TINT_MAX_OPACITY_PERCENT;
    }

    return Percent;
}

BYTE TintOpacityPercentToAlpha(int Percent)
{
    int ClampedPercent = TintClampOpacityPercent(Percent);

    return (BYTE)((ClampedPercent * 255 + 50) / 100);
}
