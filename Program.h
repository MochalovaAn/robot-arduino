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
  size_t _line;
  int _state;
  unsigned long _timer;

public:
  Program(AccelStepper *stepper)
      : _stepper(stepper), _count(0), _line(0), _state(PROGRAM_NONE), _timer(0) {}

  bool setAsText(const String &text)
  {
    return setAsText(text.c_str());
  }

  bool setAsText(const char *text)
  {
    Command *command;
    size_t i;

    stop(1);
    _count = 0;

    while (*text != '\0' && _count < COMMAND_MAX)
    {
      command = &_commands[_count++];
      memset(command, 0, sizeof(Command));

      while (*text == ' ' || *text == '\t')
        text++;

      i = 0;
      while (i < 16 && *text != ' ' && *text != '\t' && *text != '\r' && *text != '\n' && *text != '\0')
      {
        command->name[i++] = *text;
        text++;
      }

      while (*text == ' ' || *text == '\t')
        text++;

      command->value = atof(text);

      while (*text != '\r' && *text != '\n' && *text != '\0')
        text++;
      if (*text == '\r')
        text++;
      if (*text == '\n')
        text++;
    }

    _state = PROGRAM_STOP;

    Serial.printf("Program: Text changed, lines %d\n", _count);

    return true;
  }

  String getAsText()
  {
    String text = "";
    Command *command;

    for (size_t i = 0; i < _count; i++)
    {
      command = &_commands[i];
      text += command->name;
      text += " ";
      text += String(command->value);
      text += "\r\n";
    }

    return text;
  }

  bool run()
  {
    bool timeIsOwer;
    Command *command;

    // если программа не запущена - выход

    if (_state != PROGRAM_RUN)
      return false;

    // если двигатель еще крутится и его не надо останавливать по таймеру - выход

    timeIsOwer = (_timer != 0) && (millis() > _timer);

    if (_stepper->isRunning() && !timeIsOwer)
      return true;

    // сбрасываем сработавший таймер - он запускается только для одной команды

    if (timeIsOwer)
    {
      Serial.println("Program: Timeout");
      _timer = 0;
    }

    // если это последняя команда - конец программы

    if (_line == _count)
    {
      _stepper->setCurrentPosition(_stepper->currentPosition());
      _state = PROGRAM_STOP;

      Serial.println("Program: End");
      return false;
    }

    // пребираем команды, пока одна из них не запустит двигатель, конец программы будет
    // обработан следующим вызовом run()

    while (_line < _count)
    {
      command = &_commands[_line];
      _line++;

      Serial.printf("Program: Line %d, %s %f\n", _line, command->name, command->value);

      switch (command->name[0])
      {
      case 's':
        switch (command->name[1])
        {
        case 'p': // speed
          _stepper->setMaxSpeed(command->value);
          break;

        case 't':
          switch (command->name[2])
          {
          case 'o': // stop
            int force = (int)command->value;
            force ? _stepper->setCurrentPosition(_stepper->currentPosition()) : _stepper->stop();
            return true;

          case 'e': // step
            _stepper->move((long)command->value);
            return true;
          }
          break;
        }
        break;

      case 'a': // acceleration
        _stepper->setAcceleration(command->value);
        break;

      case 'm': // move
        _stepper->move((long)command->value);
        return true;

      case 'r':
        switch (command->name[1])
        {
        case 'o': // rotate
          _stepper->move((long)(2048 / command->value));
          return true;

        case 'e':
          switch (command->name[2])
          {
          case 's': // reset
            _stepper->setCurrentPosition((long)command->value);
            break;

          case 'p': // repeat
            _line = 0;
            break;
          }
          break;
        }
        break;

      case 't': // timer
        _timer = millis() + (unsigned long)command->value;
        break;
      }
    }

    return true;
  }

  bool start()
  {
    if (_state != PROGRAM_STOP)
      return false;

    _stepper->setCurrentPosition(0);

    _state = PROGRAM_RUN;
    _line = 0;
    _timer = 0;

    Serial.println("Program: Start");

    return true;
  }

  bool stop(int force)
  {
    if (_state != PROGRAM_RUN)
      return false;

    force ? _stepper->setCurrentPosition(_stepper->currentPosition()) : _stepper->stop();
    _state = PROGRAM_STOP;

    Serial.println("Program: Stop");

    return true;
  }

  Command *commands() { return &_commands[0]; }
  long line() { return _line + 1; }
  long state() { return _state; }
  bool isRunning() { return _state == 1; }
};

#endif
