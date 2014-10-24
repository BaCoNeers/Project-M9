#ifndef DEFINITIONS_H
#define DEFINITIONS_H

int steps = 0;
void waitForStart_II()
{
  while (true)
  {
    getJoystickSettings(joystick);
    steps++;
    if(steps >= 100)
    {
   steps = 0;
    eraseDisplay();
    nxtDisplayCenteredTextLine(2, "Waiting for");
    nxtDisplayCenteredTextLine(3, "start!");
  }
   /* if (!joystick.StopPgm)
      break;*/
  }
  return;
}

void DrawDebug()
{

}

#endif
