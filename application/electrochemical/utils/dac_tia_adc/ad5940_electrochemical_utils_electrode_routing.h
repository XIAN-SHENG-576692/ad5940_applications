#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "ad5940.h"

/**
 * @brief Alias for SWMatrixCfg_Type used to configure electrode routing in electrochemical applications.
 * 
 * This typedef provides semantic clarity when setting up the switch matrix for electrochemical sensors. 
 * It is functionally identical to SWMatrixCfg_Type and does not introduce new members or additional functionality.
 * 
 * @note
 * - For detailed configuration guidance, refer to the datasheet:
 *   - Pages 30–52: Overview of LPLoop and HSLoop configurations
 *   - Pages 74–76: Electrochemical sensor setup and switch matrix configuration
 * - See Figure 36 (Page 76) for the switch matrix routing diagram.
 * 
 * @example
 * To configure CE = CE0, RE = RE0, WE = AIN0, and route the TIA to AIN0:
 * 
 * @code
 * AD5940_ELECTROCHEMICAL_ELECTRODE_ROUTING electrode_routing = {
 *   .Dswitch  = SWD_CE0,               // Connect CE (Counter Electrode) to CE0
 *   .Pswitch  = SWP_RE0,               // Connect RE (Reference Electrode) to RE0
 *   .Nswitch  = SWN_AIN0,              // Connect WE (Working Electrode) to AIN0
 *   .TDswitch = SWT_AIN0 | SWT_TRTIA,  // Route the internal TIA output to AIN0
 * };
 * @endcode
 * 
 * @warning 
 * - The switch matrix configuration is only applicable when using **HSTIA (High-Speed TIA)**.
 * - If **LPTIA (Low-Power TIA)** is selected, the switch matrix is **not applicable**.
 *   - In LPTIA mode, CE, RE, and SE are internally fixed to **CE0**, **RE0**, and **SE0**, respectively. See datasheet Page 39 for details.
 * 
 * - Additionally, Figure 25 (Page 45) shows how to generate the bias voltage using LPDAC and HSDAC.
 *   - Since the LPDAC's electrode selection is fixed to **CE0**, **RE0**, and **SE0**, the switch matrix must also be configured accordingly to route to these same electrode_routing.
 */
typedef SWMatrixCfg_Type AD5940_ELECTROCHEMICAL_ELECTRODE_ROUTING;

#ifdef __cplusplus
}
#endif
