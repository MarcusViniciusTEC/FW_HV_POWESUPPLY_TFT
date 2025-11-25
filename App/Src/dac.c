

#include "dac_types.h"

static DAC_MCP4725_ctrl_t DAC_MCP4725_ctrl;

#define VREF_DAC (float)3.30

void DAC_MCP4725_init(void)
{
    DAC_MCP4725_ctrl.DAC[DAC_CURRENT]   = MCP4725_init(&hi2c1, MCP4725A0_ADDR_A00, VREF_DAC);
    DAC_MCP4725_ctrl.DAC[DAC_VOLTAGE]   = MCP4725_init(&hi2c1, MCP4725A0_ADDR_A01, VREF_DAC);

    if(MCP4725_isConnected(&DAC_MCP4725_ctrl.DAC[DAC_CURRENT]))
    {

    }
    else
    {

    }

    if(MCP4725_isConnected(&DAC_MCP4725_ctrl.DAC[DAC_VOLTAGE]))
    {

    }
    else
    {

    }

    
}

/***********************************************************************************/

void DAC_MCP4725_set(dac_channels_t channel, uint16_t value)
{

    MCP4725_setValue(&DAC_MCP4725_ctrl.DAC[channel], value, MCP4725_FAST_MODE, MCP4725_POWER_DOWN_OFF); 
}



/***********************************************************************************/