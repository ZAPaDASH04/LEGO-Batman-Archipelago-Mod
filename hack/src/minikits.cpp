/**
 * @file minikits.cpp
 * @author ZAPaDASH04 (ZAPaDASH04@gmail.com) @ZAPaDASH04
 * @brief 
 * @version 0.1
 * @date 2025-08-01
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "minikits.h"



Minikits::Minikits(DWORD BASE_ADDR) :
    BASE_ADDR(BASE_ADDR)
{
    // kitTable[{0x12, "pup4"     }] = 0;
    // kitTable[{0x12, "m_pup1"   }] = 1;
    // 12 pup3
    // 12 pup2
    // 12 pup1
    // 12 pup5
    // 13 m_pup1
    // 13 pup2
    // 13 pup1
    // 13 MIN_FO5
    kitTable = {
        {{0x12, "pup4"      },   0},
        {{0x12, "m_pup1"    },   1},
        {{0x12, "pup3"      },   2},
        {{0x12, "pup2"      },   3},
        {{0x12, "pup1"      },   4},
        {{0x12, "pup5"      },   5},
        {{0x13, "m_pup1"    },   6},
        {{0x13, "pup2"      },   7},
        {{0x13, "pup1"      },   8},
        {{0x13, "MIN_FO5"   },   9},
        {{0x1E, "m_pup3"    },  10},
        {{0x1E, "mk_1"      },  11},
        {{0x1E, "m_pup2"    },  12},
        {{0x1E, "mk_0"      },  13},
        {{0x1E, "m_pup1"    },  14},
        {{0x1E, "BRUCE_M"   },  15},
        {{0x1F, "mk_0"      },  16},
        {{0x1F, "MIKIgas"   },  17},
        {{0x1F, "mk_1"      },  18},
        {{0x1F, "M_CIV"     },  19},
        // "2B SUPKIT1
        // 2B SUPKIT2
        // 2B SUPKIT3"
        {{0x2B, "SUPKIT1"   },  20},
        {{0x2B, "SUPKIT2"   },  20},
        {{0x2B, "SUPKIT3"   },  20},
        {{0x2B, "MKIT_MH"   },  21},
        {{0x2C, "MKIT_6"    },  22},
        // "2C CARKIT1
        // 2C CARKIT2
        // 2C CARKIT3"
        {{0x2C, "CARKIT1"   },  23},
        {{0x2C, "CARKIT2"   },  23},
        {{0x2C, "CARKIT3"   },  23},
        {{0x2C, "MKIT_3"    },  24},
        {{0x2C, "MKIT_2"    },  25},
        {{0x2C, "MKIT_5"    },  26},
        {{0x2C, "MKIT_4"    },  27},
        {{0x2D, "MKIT_10"   },  28},
        {{0x2D, "SUPKIT6"   },  29},
        {{0x30, "MINI02"    },  30},
        {{0x30, "MINI01"    },  31},
        {{0x31, "MINI03"    },  32},
        {{0x31, "MINI04"    },  33},
        {{0x31, "m_pup1"    },  34},
        {{0x32, "MINI06"    },  35},
        {{0x32, "MINI08"    },  36},
        // "33 MINI09
        // 33 CARROTB
        // 33 CARROTC"
        {{0x33, "MINI09"    },  37},
        {{0x33, "CARROTB"   },  37},
        {{0x33, "CARROTC"   },  37},
        {{0x33, "m_pup1"    },  38},        
        {{0x33, "MINI10"    },  39},        
        {{0x48, "M_BOLL"    },  40},        
        {{0x48, "m_pup1"    },  41},        
        {{0x48, "DOLLAR"    },  42},        
        {{0x48, "mk_3"      },  43},      
        {{0x48, "mk_1"      },  44},      
        {{0x4A, "m_pup1"    },  45},        
        {{0x4A, "mk_1"      },  46},      
        {{0x4A, "mk_0"      },  47},      
        {{0x4B, "m_pup1"    },  48},        
        // "4B mIN_C1
        // 4B mIN_C2
        // 4B mIN_C3
        // 4B mIN_C3
        // 4B mIN_C4
        // 4B mIN_C5"
        {{0x4B, "mIN_C1"    }, 49},
        {{0x4B, "mIN_C2"    }, 49},
        {{0x4B, "mIN_C3"    }, 49},
        {{0x4B, "mIN_C3"    }, 49},
        {{0x4B, "mIN_C4"    }, 49},
        {{0x4B, "mIN_C5"    }, 49},

        // EPISODE 2
    };
}

DWORD Minikits::findKitIndex(BYTE level, const char* name) {
    Key key = {level, std::string(name, strnlen(name,8))};
    auto it = kitTable.find(key);
    if (it != kitTable.end()) {
        return it->second;
        // Use it
    }
    return -1;
}