#ifndef Keyboard_H
#define Keyboard_H

class Building_Base;

namespace keyboard
{
// statuses:
// 0 - quit
int simple(); // enter progresses, otherwise, no action
int earth(); // main gameplay screen, captures keystrokes
int battle(); // battle screen, captures keystrokes

// buildings
// 1 - escape to main
// 2 - 11: select item # 0-9
int building(Building_Base building);

} // namespace keyboard

#endif
