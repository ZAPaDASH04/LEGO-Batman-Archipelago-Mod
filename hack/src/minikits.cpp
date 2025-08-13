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

        // Episode 1

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
        {{0x4B, "mIN_C1"    },  49},
        {{0x4B, "mIN_C2"    },  49},
        {{0x4B, "mIN_C3"    },  49},
        {{0x4B, "mIN_C3"    },  49},
        {{0x4B, "mIN_C4"    },  49},
        {{0x4B, "mIN_C5"    },  49},

        // Episode 2

        {{0x5B, "pup124"    },  50},
        {{0x5B, "MINI_GA"   },  51},
        {{0x5B, "MINI_G2"   },  52},
        {{0x5B, "MINI_C1"   },  53},
        {{0x5B, "MINI_C2"   },  53},
        {{0x5B, "MINI_C3"   },  53},
        {{0x5B, "MINI_G1"   },  54},
        {{0x5C, "MINI_MO"   },  55},
        {{0x5C, "MINI_KI"   },  56},
        {{0x5C, "MINI_FA"   },  57},
        {{0x5C, "pup46"     },  58},
        {{0x5C, "pup2"      },  59},
        {{0x74, "MK_RAMP"   },  60},
        {{0x74, "MK_01"     },  61},
        {{0x74, "MK_BOAT"   },  62},
        {{0x74, "mk_0"      },  63},
        {{0x75, "MK_ICE"    },  64},
        {{0x75, "MK_DRAG"   },  65},
        {{0x75, "PIPE_MK"   },  66},
        {{0x75, "BUOYMK2"   },  67},
        {{0x75, "BUOYMK3"   },  67},
        {{0x75, "BUOYMK1"   },  67},
        {{0x77, "mk_1"      },  68},
        {{0x77, "MK_DB"     },  69},
        {{0x66, "MINI1"     },  70},
        {{0x66, "MINI_CO"   },  71},
        {{0x67, "MINI_CA"   },  72},
        {{0x67, "pup1"      },  73},
        {{0x67, "MINI_B"    },  74},
        {{0x68, "MINI4"     },  75},
        {{0x68, "MINI_BO"   },  76},
        {{0x68, "MINI_LI"   },  77},
        {{0x68, "MINI_DM"   },  78},
        {{0x68, "MINI_PI"   },  79},
        {{0x87, "m_pup1"    },  80},
        {{0x87, "mk_0"      },  81},     
        {{0x87, "m_pup2"    },  82},
        {{0x87, "ShopMK"    },  83},
        {{0x88, "Totemmk"   },  84},
        {{0x88, "Boulder"   },  85},
        {{0x88, "mk_0"      },  86},
        {{0x89, "MKLIL"     },  87},
        {{0x89, "mk_1"      },  88},
        {{0x89, "MK01"      },  89},
        {{0x8E, "mk_2"      },  90},
        {{0x8E, "mk_3"      },  91},
        {{0x8E, "mk_1"      },  92},
        {{0x8E, "m_pup1A"   },  93},
        {{0x8E, "m_pup1B"   },  93},
        {{0x8E, "m_pup1C"   },  93},
        {{0x8E, "mk_0"      },  94},
        {{0x8F, "mk_2"      },  95},
        {{0x8F, "mk_1"      },  96},
        {{0x8F, "mk_0"      },  97},
        {{0x8F, "m_pup1"    },  98},
        {{0x8F, "m_pup2"    },  99},

        // Episode 3

        {{0xA0, "mk_0"      }, 100},
        {{0xA0, "m_pup1"    }, 101},
        {{0xA0, "mk_2"      }, 102},
        {{0xA0, "mk_1"      }, 103},
        {{0xA1, "m_pup1"    }, 104},
        {{0xA1, "mk_0"      }, 105},
        {{0xA1, "mk_1"      }, 106},
        {{0xA1, "mk_2"      }, 107},
        {{0xA2, "mk_0"      }, 108},
        {{0xA2, "m_pup1"    }, 109},
        {{0xA6, "MINI_1"    }, 110},
        {{0xA6, "MINI_2"    }, 111},
        {{0xA6, "MINI_3"    }, 112},
        {{0xA6, "mk_0"      }, 113},
        {{0xA7, "MINI_9"    }, 114},
        {{0xA7, "MINI_4"    }, 115},
        {{0xA7, "MINI_6"    }, 116},
        {{0xA8, "MINI_7"    }, 117},
        {{0xA8, "MINI_8"    }, 118},
        {{0xA8, "MINI_10"   }, 119},
        {{0xAC, "m_pup6"    }, 120},
        {{0xAC, "m_pup2"    }, 121},
        {{0xAC, "m_pupER"   }, 122},
        {{0xAC, "m_pup4"    }, 123},
        {{0xAD, "mk_0"      }, 124},
        {{0xAD, "mk_2"      }, 125},
        {{0xAE, "GAR_MK1"   }, 126},
        {{0xAE, "GAR_MK2"   }, 126},
        {{0xAE, "GAR_MK3"   }, 126},
        {{0xAD, "m_BAR"     }, 127}, 
        {{0xAE, "m_pup1"    }, 128},
        {{0xAE, "m_pup3"    }, 129},
        {{0xB8, "m_pup1"    }, 130},
        {{0xB8, "m_roof"    }, 131},
        {{0xB8, "m_tur1"    }, 132},
        {{0xB8, "m_tur2"    }, 132},
        {{0xB8, "m_tur3"    }, 132},
        {{0xB9, "AQUARIU"   }, 133},
        {{0xB9, "COP_KI"    }, 134},
        {{0xBA, "W_KIT"     }, 135},
        {{0xBA, "HI_KIT"    }, 136},
        {{0xBA, "m_pup2"    }, 137},
        {{0xBB, "m_pup1"    }, 138},
        {{0xB8, "m_jkr"     }, 139},
        {{0xC6, "mK_G1"     }, 140},
        {{0xC6, "mK_G2"     }, 140},
        {{0xC6, "mK_G3"     }, 140},
        {{0xC6, "mk_0"      }, 141},
        {{0xC6, "mKSONAR"   }, 142},
        {{0xC6, "mk_1"      }, 143},
        {{0xC6, "m_pup1"    }, 144},
        {{0xC7, "mk_JOKE"   }, 145},
        {{0xC7, "m_pup1"    }, 146},
        {{0xC7, "mk_SONR"   }, 147},
        {{0xC8, "mk_0"      }, 148},
        {{0xC8, "mk_CRAT"   }, 149},

        // Episode 4
        
        

        // Episode 5
        

        // Episode 6
        
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