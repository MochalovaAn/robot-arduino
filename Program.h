#ifndef PROGRAM_H
#define PROGRAM_H

#include <AccelStepper.h>

#define COMMAND_MAX 1024

#define PROGRAM_NONE 0
#define PROGRAM_STOP 1
#define PROGRAM_RUN 2

typedef struct command_t
{
  char name[16];
  float value;
} Command;

class Program
{
private:
  AccelStepper *_stepper;
  Command _commands[COMMAND_MAX];
  size_t _count;
  size_t _cycle;
  size_t _line;
  int _state;
  unsigned long _timer;

public:
  Program(AccelStepper *stepper);

  bool setAsText(const String &text);
  bool setAsText(const char *text);
  String getAsText();

  bool execute();
  bool run();
  bool stop(int force);

  Command *commands() { return &_commands[0]; }
  long line() { return _line + 1; }
  long cycle() { return _cycle; }
  long state() { return _state; }
  bool isRunning() { return _state == PROGRAM_RUN; }
};

#endif
