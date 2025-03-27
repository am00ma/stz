#pragma once

#include "types.h"

void osfail(void);
i32  osread(i32, char*, i32);
b32  oswrite(i32, char*, i32);
void oom(void);

