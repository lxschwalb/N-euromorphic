#pragma once

#define V_THR       -50e-3
#define V_RST       -70e-3
#define V_ACT        50e-3
#define SPIKE_MAG    10

const float w_max = 1.5*(V_THR - V_RST);
const float l_max = 5 * w_max;
const float d_max = 1.f;
const float r_max = 1.f;
const float middle_c = 261.6256;