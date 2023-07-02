#include "PoolBoy_Json.h" 

static uint8_t telemetry_payload[100];
static char* getTelemetryPayload(float tempCelsius)
{  
 


   az_span temp_span = az_span_create(telemetry_payload, sizeof(telemetry_payload));
  temp_span = az_span_copy(temp_span, AZ_SPAN_FROM_STR("{ \"Temp\": "));
  
  
  
  temp_span = az_span_copy(temp_span, AZ_SPAN_FROM_STR("\""));
  (void)az_span_u32toa(temp_span, tempCelsius, &temp_span);
  temp_span = az_span_copy(temp_span, AZ_SPAN_FROM_STR("."));
  
  
  
    float temp = tempCelsius - (long)(tempCelsius);  
  long p = 1;
  for (int i=0; i< 2; i++) p*=10;
  long DecimalPart = p * temp;

 
 
 
 
 
 
 (void)az_span_u32toa(temp_span, DecimalPart, &temp_span);
    temp_span = az_span_copy(temp_span, AZ_SPAN_FROM_STR("\""));



  
  //(void)az_span_u32toa(temp_span, tempCelsius, &temp_span);
  temp_span = az_span_copy(temp_span, AZ_SPAN_FROM_STR(" }"));
  temp_span = az_span_copy_u8(temp_span, '\0');
  return (char*)telemetry_payload;

}