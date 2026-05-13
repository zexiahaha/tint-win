#ifndef TINT_VIEW_H
#define TINT_VIEW_H

#include "tint_app.h"

void TintSetSelectionText(
                          tint_app_state *State,
                          LPCWSTR Title,
                          LPCWSTR Process
                          );

void TintSetStatusText(
                       tint_app_state *State,
                       LPCWSTR Text
                       );

void TintSetOpacityPercent(
                           tint_app_state *State,
                           int Percent
                           );

#endif
