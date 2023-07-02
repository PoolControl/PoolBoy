#include <ArduinoJson.h>
String Serialize()
{
char output[128];
DynamicJsonDocument doc(1024);
doc["sensor"] = "gps";
doc["time"]   = 1351824120;
doc["data"][0] = 48.756080;
doc["data"][1] = 2.302038;
serializeJson(doc, output,sizeof(output));
return output;
}

