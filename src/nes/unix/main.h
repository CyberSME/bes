#ifndef _MAIN_H_
#define _MAIN_H_

#define TV_WIDTH 292

void NstPlayGame();
void NstPlayNsf();
void NstStopNsf();
void NstScheduleQuit();
void NstStopPlaying();
void NstReset();
bool NstIsPlaying();
bool NstIsLoaded();
void NstLoadGame(const char* filename);
void NstLaunchConfig();

void get_screen_res();
// AWH void ToggleFullscreen();
void FlipFDSDisk();

#endif
