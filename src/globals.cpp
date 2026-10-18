#include "includes.hpp"

// Global game window handle & base
HWND hwnd = NULL;
uintptr_t gd_base = 0;
gd::PlayLayer* g_currentPlayLayer = nullptr;

bool menuVisible = false;

bool noDeathEffect = false;
bool hideEndscreen = false;
bool hideNewBest = false;
bool noEffect = false;

bool fadeLevel = false;
double fadeLevelInDuration = 0.5;
double fadeLevelOutDuration = 1.0;

bool fadeAudio = false;
double fadeAudioInDuration = 0.5;
double fadeAudioOutDuration = 1.0;

bool straightFlyP1 = false;
bool straightFlyP2 = false;
double straightFlyThresholdP1 = 0.0;
double straightFlyThresholdP2 = 0.0;

bool straightUfoP1 = false;
bool straightUfoP2 = false;
double straightUfoTargetP1 = 135.0;
double straightUfoTargetP2 = 135.0;
double straightUfoThresholdP1 = 0.0;
double straightUfoThresholdP2 = 0.0;

gd::ccColor3B layoutModeColorBackground = { 160, 160, 160 };
gd::ccColor3B layoutModeColorGround = { 160, 160, 160 };

int flipPlayer = 0;
bool flipOnDeath = false;
bool flipOnDeathBoth = false;
bool flipOnDeathP1 = true;
bool flipOnDeathP2 = true;
bool flipOnDeathLogicP2 = true;
bool flipOnDeathLogicP1 = true;
bool flipOnDeathSwift = false;
bool flipOnDeathUnfreeze = false;

bool autoUnfreeze = false;
bool autoSwift = false;
bool extraClick = false;
int  extraClickAmount = 1;

bool clickGreenDash = false;
bool clickBlackOrbs = false;
bool clickJumpPads = false;
bool clickGravityPads = false;
bool clickedJumpPad = false;

bool straightFly = false;
bool straightUfo = false;

bool maintainGravity = false;

bool autoclickerP1 = false;
bool autoclickerP2 = false;

int autoclickerEveryP1 = 1;
int autoclickerEveryP2 = 1;
int autoclickerHoldP1 = 1;
int autoclickerHoldP2 = 1;

int  autoclickerTimerP1 = 0;
int  autoclickerTimerP2 = 0;
bool autoclickerHoldingP1 = false;
bool autoclickerHoldingP2 = false;

bool autoclickerSwiftP1 = false;
bool autoclickerSwiftP2 = false;

bool noclip = false;
bool noclipP1 = true;
bool noclipP2 = true;

bool spamCheckpoints = false;
bool restartFirstFrame = false;

bool layoutMode = false;
bool blackOrbUfo = false;

bool releaseGravityOrbsPrevent = false;
bool optimizeStackedOrbs = false;

bool optimizeRingJump = false;
bool optimizeStartDashing = false;
bool optimizeStopDashing = false;

std::array<float, 3> colorToFloat(gd::ccColor3B color) {
    return {
        color.r / 255.0f,
        color.g / 255.0f,
        color.b / 255.0f
    };
}

gd::ccColor3B floatToColor(const float* col) {
    return {
        static_cast<uint8_t>(col[0] * 255.0f),
        static_cast<uint8_t>(col[1] * 255.0f),
        static_cast<uint8_t>(col[2] * 255.0f)
    };
}

const std::unordered_set<int> decoration = {
    5, 15, 16, 17, 18, 19, 20, 21, 41, 48, 49, 50, 51, 52, 53, 54, 60, 73, 80, 85, 86, 87, 97,
    106, 107, 110, 113, 114, 115, 120, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133,
    134, 136, 137, 138, 139, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 180,
    181, 182, 190, 211, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235,
    236, 237, 238, 239, 240, 241, 242, 245, 246, 259, 266, 273, 277, 278, 279, 280, 281, 282,
    283, 284, 285, 296, 297, 324, 325, 358, 373, 374, 375, 376, 377, 378, 394, 395, 396, 405,
    406, 407, 408, 409, 410, 411, 412, 413, 414, 419, 420, 448, 449, 450, 451, 452, 453, 454,
    455, 456, 457, 460, 472, 473, 474, 476, 477, 478, 479, 480, 481, 482, 485, 486, 487, 488,
    489, 490, 491, 494, 495, 496, 497, 498, 499, 500, 501, 502, 503, 504, 505, 506, 507, 508,
    509, 510, 511, 512, 513, 514, 515, 516, 517, 518, 519, 520, 521, 522, 523, 524, 525, 526,
    527, 528, 529, 530, 531, 532, 533, 534, 535, 536, 537, 538, 539, 540, 541, 542, 543, 544,
    545, 546, 547, 548, 549, 550, 551, 552, 553, 554, 555, 556, 557, 558, 559, 560, 561, 562,
    563, 564, 565, 566, 567, 568, 569, 570, 571, 572, 573, 574, 575, 576, 577, 578, 579, 580,
    581, 582, 583, 584, 585, 586, 587, 588, 589, 590, 591, 592, 593, 594, 595, 596, 597, 598,
    599, 600, 601, 602, 603, 604, 605, 606, 607, 608, 609, 610, 611, 612, 613, 614, 615, 616,
    617, 618, 619, 620, 621, 622, 623, 624, 625, 626, 627, 628, 629, 630, 631, 632, 633, 634,
    635, 636, 637, 638, 639, 640, 641, 642, 643, 644, 645, 646, 647, 648, 649, 650, 653, 654,
    655, 656, 657, 658, 659, 668, 669, 670, 671, 672, 681, 682, 683, 684, 685, 686, 687, 688,
    689, 690, 691, 692, 693, 694, 695, 696, 697, 698, 699, 700, 701, 702, 703, 704, 705, 706,
    707, 708, 713, 714, 715, 716, 719, 721, 722, 723, 724, 730, 731, 732, 733, 734, 735, 736,
    738, 739, 752, 753, 754, 755, 756, 757, 758, 759, 762, 763, 764, 765, 766, 767, 769, 770,
    771, 772, 773, 774, 775, 807, 808, 809, 810, 811, 812, 813, 814, 815, 816, 817, 818, 819,
    820, 821, 822, 823, 824, 825, 826, 827, 828, 829, 830, 831, 832, 833, 841, 842, 843, 844,
    845, 846, 847, 848, 850, 853, 854, 855, 856, 857, 859, 861, 862, 863, 867, 868, 869, 870,
    871, 872, 873, 874, 877, 878, 880, 881, 882, 883, 884, 885, 888, 889, 890, 891, 893, 894,
    895, 896, 902, 903, 904, 905, 906, 907, 908, 909, 910, 911, 916, 917, 920, 921, 923, 924,
    925, 926, 927, 928, 929, 930, 931, 932, 933, 934, 935, 936, 937, 938, 939, 940, 941, 942,
    943, 944, 945, 946, 947, 948, 949, 950, 951, 952, 953, 954, 955, 956, 957, 958, 959, 960,
    961, 964, 965, 966, 967, 968, 969, 970, 971, 972, 973, 974, 975, 976, 977, 980, 981, 982,
    983, 984, 985, 986, 987, 988, 990, 992, 997, 998, 999, 1000
};

const std::unordered_set<int> filter = {
    2015, 15, 16, 17, 221, 743, 744, 899, 900, 915, 1006, 32, 33, 29, 30, 104, 105, 717, 718,
    1007, 1520, 2903, 3029, 3030, 3031, 2999, 3606, 3612, 3009, 3010, 3014, 3015, 3021, 3020,
    1818, 1819, 1612, 1613, 3608
};
