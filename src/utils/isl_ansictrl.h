#ifndef ISC_ANSICTRL_H
#define ISC_ANSICTRL_H
/*
    This header suplies some ANSI control character string.
    It can make the logger's output more colorful.
    Written by invefa.
*/


#define ANSI_ESC        "\033["
#define ANSI_END        "m"

#define ANSI_RST        ANSI_ESC "0m"
#define ANSI_HIL        ANSI_ESC "1m"
#define ANSI_FAINT      ANSI_ESC "2m"
#define ANSI_ITALIC     ANSI_ESC "3m"

#define ANSI_UNDERLNE   ANSI_ESC "4m"
#define ANSI_NUNDERLNE  ANSI_ESC "24m"

#define ANSI_SLOW_BLINK ANSI_ESC "5m"
#define ANSI_FAST_BLINK ANSI_ESC "6m"
#define ANSI_INV        ANSI_ESC "7m"
#define ANSI_FADE       ANSI_ESC "8m"

#define ANSI_BLK    ANSI_ESC "30m"
#define ANSI_GRY    ANSI_ESC "1;30m"
#define ANSI_RED    ANSI_ESC "31m"
#define ANSI_HIR    ANSI_ESC "1;31m"
#define ANSI_GRE    ANSI_ESC "32m"
#define ANSI_HIG    ANSI_ESC "1;32m"
#define ANSI_YEL    ANSI_ESC "33m"
#define ANSI_HIY    ANSI_ESC "1;33m"
#define ANSI_BLU    ANSI_ESC "34m"
#define ANSI_HIB    ANSI_ESC "1;34m"
#define ANSI_MAG    ANSI_ESC "35m"
#define ANSI_HIM    ANSI_ESC "1;35m"
#define ANSI_CLY    ANSI_ESC "36m"
#define ANSI_HIC    ANSI_ESC "1;36m"
#define ANSI_WHI    ANSI_ESC "37m"
#define ANSI_HIW    ANSI_ESC "1;37m"

#define ANSI_BBLK   ANSI_ESC "40m"
#define ANSI_BGRY   ANSI_ESC "1;40m"
#define ANSI_BRED   ANSI_ESC "41m"
#define ANSI_BHIR   ANSI_ESC "1;41m"
#define ANSI_BGRE   ANSI_ESC "42m"
#define ANSI_BHIG   ANSI_ESC "1;42m"
#define ANSI_BYEL   ANSI_ESC "43m"
#define ANSI_BHIY   ANSI_ESC "1;43m"
#define ANSI_BBLU   ANSI_ESC "44m"
#define ANSI_BHIB   ANSI_ESC "1;44m"
#define ANSI_BMAG   ANSI_ESC "45m"
#define ANSI_BHIM   ANSI_ESC "1;45m"
#define ANSI_BCLY   ANSI_ESC "46m"
#define ANSI_BHIC   ANSI_ESC "1;46m"
#define ANSI_BWHI   ANSI_ESC "47m"
#define ANSI_BHIW   ANSI_ESC "1;47m"


#define ANSI_RST_SET(_cstring)          ANSI_ESC "0m" _cstring ANSI_RST
#define ANSI_HIL_SET(_cstring)          ANSI_ESC "1m" _cstring ANSI_RST
#define ANSI_FAINT_SET(_cstring)        ANSI_ESC "2m" _cstring ANSI_RST
#define ANSI_ITALIC_SET(_cstring)       ANSI_ESC "3m" _cstring ANSI_RST

#define ANSI_UNDERLNE_SET(_cstring)     ANSI_ESC "4m"  _cstring ANSI_RST
#define ANSI_NUNDERLNE_SET(_cstring)    ANSI_ESC "24m" _cstring ANSI_RST

#define ANSI_SLOW_BLINK_SET(_cstring)   ANSI_ESC "5m" _cstring ANSI_RST
#define ANSI_FAST_BLINK_SET(_cstring)   ANSI_ESC "6m" _cstring ANSI_RST
#define ANSI_INV_SET(_cstring)          ANSI_ESC "7m" _cstring ANSI_RST
#define ANSI_FADE_SET(_cstring)         ANSI_ESC "8m" _cstring ANSI_RST

#define ANSI_BLK_SET(_cstring)  ANSI_ESC "30m"   _cstring ANSI_RST
#define ANSI_GRY_SET(_cstring)  ANSI_ESC "1;30m" _cstring ANSI_RST
#define ANSI_RED_SET(_cstring)  ANSI_ESC "31m"   _cstring ANSI_RST
#define ANSI_HIR_SET(_cstring)  ANSI_ESC "1;31m" _cstring ANSI_RST
#define ANSI_GRE_SET(_cstring)  ANSI_ESC "32m"   _cstring ANSI_RST
#define ANSI_HIG_SET(_cstring)  ANSI_ESC "1;32m" _cstring ANSI_RST
#define ANSI_YEL_SET(_cstring)  ANSI_ESC "33m"   _cstring ANSI_RST
#define ANSI_HIY_SET(_cstring)  ANSI_ESC "1;33m" _cstring ANSI_RST
#define ANSI_BLU_SET(_cstring)  ANSI_ESC "34m"   _cstring ANSI_RST
#define ANSI_HIB_SET(_cstring)  ANSI_ESC "1;34m" _cstring ANSI_RST
#define ANSI_MAG_SET(_cstring)  ANSI_ESC "35m"   _cstring ANSI_RST
#define ANSI_HIM_SET(_cstring)  ANSI_ESC "1;35m" _cstring ANSI_RST
#define ANSI_CLY_SET(_cstring)  ANSI_ESC "36m"   _cstring ANSI_RST
#define ANSI_HIC_SET(_cstring)  ANSI_ESC "1;36m" _cstring ANSI_RST
#define ANSI_WHI_SET(_cstring)  ANSI_ESC "37m"   _cstring ANSI_RST
#define ANSI_HIW_SET(_cstring)  ANSI_ESC "1;37m" _cstring ANSI_RST

#define ANSI_BBLK_SET(_cstring) ANSI_ESC "40m"   _cstring ANSI_RST
#define ANSI_BGRY_SET(_cstring) ANSI_ESC "1;40m" _cstring ANSI_RST
#define ANSI_BRED_SET(_cstring) ANSI_ESC "41m"   _cstring ANSI_RST
#define ANSI_BHIR_SET(_cstring) ANSI_ESC "1;41m" _cstring ANSI_RST
#define ANSI_BGRE_SET(_cstring) ANSI_ESC "42m"   _cstring ANSI_RST
#define ANSI_BHIG_SET(_cstring) ANSI_ESC "1;42m" _cstring ANSI_RST
#define ANSI_BYEL_SET(_cstring) ANSI_ESC "43m"   _cstring ANSI_RST
#define ANSI_BHIY_SET(_cstring) ANSI_ESC "1;43m" _cstring ANSI_RST
#define ANSI_BBLU_SET(_cstring) ANSI_ESC "44m"   _cstring ANSI_RST
#define ANSI_BHIB_SET(_cstring) ANSI_ESC "1;44m" _cstring ANSI_RST
#define ANSI_BMAG_SET(_cstring) ANSI_ESC "45m"   _cstring ANSI_RST
#define ANSI_BHIM_SET(_cstring) ANSI_ESC "1;45m" _cstring ANSI_RST
#define ANSI_BCLY_SET(_cstring) ANSI_ESC "46m"   _cstring ANSI_RST
#define ANSI_BHIC_SET(_cstring) ANSI_ESC "1;46m" _cstring ANSI_RST
#define ANSI_BWHI_SET(_cstring) ANSI_ESC "47m"   _cstring ANSI_RST
#define ANSI_BHIW_SET(_cstring) ANSI_ESC "1;47m" _cstring ANSI_RST

#endif