static char* getCurrentLocalTimeString()
{
  time_t now = time(NULL);
  return ctime(&now);
}
static void printCurrentTime()
{
  Serial.print("Current time: ");
  Serial.print(getCurrentLocalTimeString());
}