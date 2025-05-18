#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "ad5940.h"

/**
 * @ref LPTIACfg_Type
 */
typedef struct
{
    uint32_t LpAmpPwrMod;
    #if defined(CHIPSEL_M355)
    uint32_t LpAmpSel;
    #endif
}
AD5940_ELECTROCHEMICAL_LPDACfg_Type;

/**
 * @ref HSDACCfg_Type
 */
typedef struct
{
    uint32_t ExcitBufGain;      /**< Select from  EXCITBUFGAIN_2, EXCITBUFGAIN_0P25 */     
    uint32_t HsDacGain;         /**< Select from  HSDACGAIN_1, HSDACGAIN_0P2 */
    BoolFlag GainCalEn;         /**< Enable Gain calibration */
    BoolFlag OffsetCalEn;       /**< Enable offset calibration */
}
AD5940_ELECTROCHEMICAL_HSDACCfg_Type;

/**
 * @ref LPTIACfg_Type
 */
typedef struct
{
    uint32_t LpTiaRtia;         /**< Refer to page 37 of the datasheet. 
                                     Ensure the received current does not exceed I_max. 
                                     Use values defined in @ref LPTIARTIA_Const. */
    uint32_t LpTiaRf;
    uint32_t LpTiaRload;        /**< Do not use LPTIARLOAD_SHORT */
}
AD5940_ELECTROCHEMICAL_LPTIACfg_Type;

/**
 * @ref HSTIACfg_Type
 */
typedef struct
{
    uint32_t HstiaRtiaSel;      /**< RTIA selection @ref HSTIARTIA_Const. Refer to pages 50 and 52 of the datasheet for more details. */
    uint32_t ExtRtia;           /**< Value of external RTIA*/
    uint32_t HstiaCtia;         /**< Set internal CTIA value from 1 to 32 pF. Refer to page 52 of the datasheet for more details.*/
    BoolFlag DiodeClose;        /**< Close the switch for internal back to back diode */
    uint32_t HstiaDeRtia;       /**< DE0 node RTIA selection @ref HSTIADERTIA_Const. Refer to Figure 28 (page 50) and Figure 36 (page 76) of the datasheet for more details.*/
    uint32_t HstiaDeRload;      /**< DE0 node Rload selection @ref HSTIADERLOAD_Const. Refer to Figure 28 (page 50) and Figure 36 (page 76) of the datasheet for more details.*/
    #if defined(CHIPSEL_M355)
    uint32_t HstiaDe1Rtia;      /**< (ADuCM355 only, ignored on AD594x)DE1 node RTIA selection @ref HSTIADERTIA_Const */
    uint32_t HstiaDe1Rload;     /**< (ADuCM355 only)DE1 node Rload selection @ref HSTIADERLOAD_Const */
    #endif
}
AD5940_ELECTROCHEMICAL_HSTIACfg_Type;

/**
 * @ref DSPCfg_Type
 */
typedef struct
{
    uint32_t ADCPga;                    /**< ADC PGA settings, select from @ref ADCPGA */
    struct {
        uint32_t ADCAvgNum;             /**< Average filter is enabled when DFT source is @ref DFTSRC_AVG in function @ref AD5940_DFTCfgS. This average filter is only used by DFT engine. */
        uint32_t ADCRate;             /**< ADC Core sample rate */
        uint32_t ADCSinc2Osr;
        uint32_t ADCSinc3Osr;
        BoolFlag BpNotch;               /**< Bypass Notch filter in SINC2+Notch block, so only SINC2 is used. ADCFILTERCON.BIT4 */
        BoolFlag BpSinc3;               /**< Bypass SINC3 Module */
        BoolFlag DFTClkEnable;          /**< Enable DFT clock */
        BoolFlag Sinc2NotchClkEnable;   /**< Enable SINC2+Notch clock */
        BoolFlag Sinc2NotchEnable;      /**< Enable SINC2+Notch block */
        BoolFlag Sinc3ClkEnable;        /**< Enable SINC3 clock */
    } ADCFilterCfg;                     /**< @ref ADCFilterCfg_Type
                                             ADC filter configuration include SINC3/SINC2/Notch/Average(for DFT only) */
    ADCDigComp_Type ADCDigCompCfg;      /**< ADC digital comparator */
    DFTCfg_Type DftCfg;                 /**< DFT configuration include data source, DFT number and Hanning Window */
    StatCfg_Type StatCfg;               /**< Statistic block */
}
AD5940_ELECTROCHEMICAL_DSPCfg_Type;

#ifdef __cplusplus
}
#endif
