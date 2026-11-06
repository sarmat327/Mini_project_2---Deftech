#include "components/c_ldr.h"
#include "drivers/d_adc.h"
#include "drivers/d_timer.h"
#include "config.h"

void C_LDR_Init(C_LDR_t *ldr, uint32_t channel)
{
    if(ldr == NULL)
    {
        return;
    }

    ldr->channel = channel;
    ldr->raw_value = D_ADC_Read(channel);
    ldr->filtered_value = (float)ldr->raw_value;
    ldr->timestamp = D_Timer_GetTick();
    ldr->initialized = true;

}

void C_LDR_Process(C_LDR_t *ldr)
{
    if (ldr == NULL || !ldr->initialized)
    {
        return;
    }

    uint32_t now = D_Timer_GetTick();

    if ((uint32_t)(now - ldr->timestamp) >= LDR_SAMPLE_PERIOD_MS)
    {
        ldr->timestamp = now;

        ldr->raw_value = D_ADC_Read(ldr->channel);

        ldr->filtered_value += LDR_FILTER_ALPHA * ((float)ldr->raw_value - ldr->filtered_value);
    }
}

uint16_t C_LDR_GetRaw(C_LDR_t *ldr)
{
    if (ldr == NULL || !ldr->initialized)
    {
        return 0;
    }

    return ldr->raw_value;
}

float C_LDR_GetFiltered(C_LDR_t *ldr)
{
    if (ldr == NULL || !ldr->initialized)
    {
        return 0.0f;
    }

    return ldr->filtered_value;
}