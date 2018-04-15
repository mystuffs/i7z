#include "intel.h"

int get_intel_model (int modelnumber) {    
    switch (modelnumber) {
        case 0x1E:
        case 0x1F:
        case 0x1A:
        case 0x2E:
            return INTEL_NEHALEM;
        case 0x25:
        case 0x2C:
        case 0x2F:
            return INTEL_WESTMERE;
        case 0x2A:
        case 0x2D:
            return INTEL_SANDYBRIDGE;
        case 0x3A:
        case 0x3E:
            return INTEL_IVYBRIDGE;
        case 0x3C:
        case 0x3F:
        case 0x45:
        case 0x46:
            return INTEL_HASWELL;
        case 0x3D:
        case 0x47:
        case 0x4F:
        case 0x56:
            return INTEL_BROADWELL;
        case 0x4E:
        case 0x5E:
        case 0x55:
            return INTEL_SKYLAKE;
        case 0x8E:
        case 0x9E:
            return INTEL_KABYLAKE;
        case 0x66:
            return INTEL_CANNONLAKE;
                }
    return -1;
}
