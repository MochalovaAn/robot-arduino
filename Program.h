#ifndef PROGRAM_H
#define PROGRAM_H

#include <Arduino.h>

#define COMMAND_MAX 1024
#define NAME_MAX 4

#define PROGRAM_NOTEXT 0
#define PROGRAM_STOP 1
#define PROGRAM_RUN 2

typedef struct command_t
{
  char name[NAME_MAX];
  float value;
} Command;

class ProgramClass
{
private:
  Command _commands[COMMAND_MAX];
  size_t _count;
  size_t _cycle;
  size_t _line;
  int _state;
  unsigned long _timer;
  bool _pause;

public:
  bool setAsText(const String &text);
  bool setAsText(const char *text);
  String getAsText();

  bool execute();
  bool run();
  bool stop(int force);

  Command *commands() { return &_commands[0]; }
  long count() { return _count; }
  long line() { return _line + 1; }
  long cycle() { return _cycle; }
  long state() { return _state; }
  long timer() { return _timer; }
  long pause() { return _pause; }
  bool isRunning() { return _state == PROGRAM_RUN; }
};

extern ProgramClass Program;

#endif
