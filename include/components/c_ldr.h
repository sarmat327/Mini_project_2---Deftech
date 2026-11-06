#pragma once

#include <stdint.h>
#include <stdbool.h>

/* LDR CONFIGURATION */
#define LDR_SAMPLE_PERIOD_MS    100U
#define LDR_FILTER_ALPHA        0.1f

/* LDR OBJECT */
typedef struct
{
    uint32_t channel;

    uint16_t raw_value;
    float filtered_value;

    uint32_t timestamp;

    bool initialized;

} C_LDR_t;

/* PUBLIC FUNCTIONS */
void C_LDR_Init(C_LDR_t *ldr, uint32_t channel);
void C_LDR_Process(C_LDR_t *ldr);

uint16_t C_LDR_GetRaw(C_LDR_t *ldr);
float C_LDR_GetFiltered(C_LDR_t *ldr);