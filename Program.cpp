#include "Program.h"
#include "Stepper.h"

bool ProgramClass::setAsText(const String &text)
{
  return setAsText(text.c_str());
}

bool ProgramClass::setAsText(const char *text)
{
  Command *command;
  size_t i;

  stop(1);
  _count = 0;

  while (*text != '\0' && _count < COMMAND_MAX)
  {
    command = &_commands[_count++];
    memset(command, 0, sizeof(Command));

    // пропускаем пробелы в начале строки
    while (*text == ' ' || *text == '\t')
      text++;

    // выделяем команду до пробела/конца строки, из команды берем только первые 3 символа
    i = 0;
    while (*text != ' ' && *text != '\t' && *text != '\r' && *text != '\n' && *text != '\0')
    {
      if (i < NAME_MAX - 1)
        command->name[i++] = *text;
      text++;
    }

    // пропускаем пробелы между командой и параметром
    while (*text == ' ' || *text == '\t')
      text++;

    // получаем параметр
    command->value = atof(text);

    // пропускаем все остальное до конца строки
    while (*text != '\r' && *text != '\n' && *text != '\0')
      text++;
    if (*text == '\r')
      text++;
    if (*text == '\n')
      text++;
  }

  _state = _count ? PROGRAM_STOP : PROGRAM_NOTEXT;

  Serial.printf("Program: Text changed, lines %d\n", _count);

  return true;
}

String ProgramClass::getAsText()
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

bool ProgramClass::execute()
{
  bool timeIsOwer;
  Command *command;
  size_t watchdog = 0;

  // если программа не запущена - выход

  if (_state != PROGRAM_RUN)
    return false;

  // если двигатель еще крутится и его не надо останавливать по таймеру - выход

  timeIsOwer = (_timer != 0) && (millis() > _timer);

  if ((Stepper.isRunning() || _pause) && !timeIsOwer)
    return true;

  // сбрасываем сработавший таймер - он запускается только для одной команды

  if (timeIsOwer)
  {
    Serial.println("Program: Timeout");
    _timer = 0;
    _pause = false;
  }

  // если это последняя команда - конец программы

  if (_line == _count)
  {
    Stepper.setCurrentPosition(Stepper.currentPosition());
    _state = PROGRAM_STOP;

    Serial.println("Program: End");
    return false;
  }

  // пребираем команды, пока одна из них не запустит двигатель, конец программы
  // будет обработан следующим вызовом run()

  while (_line < _count && watchdog++ < 100)
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
        Stepper.setMaxSpeed(command->value);
        break;

      case 't': // stop
        int force = (int)command->value;
        force ? Stepper.setCurrentPosition(Stepper.currentPosition()) : Stepper.stop();
        break;
      }
      break;

    case 'a': // acceleration
      Stepper.setAcceleration(command->value);
      break;

    case 'r':
      switch (command->name[1])
      {
      case 'o': // rotate
        Stepper.move((long)command->value);
        return true;

      case 'e':
        switch (command->name[2])
        {
        case 's': // reset
          Stepper.setCurrentPosition((long)command->value);
          break;

        case 'p': // repeat
          _cycle++;
          _line = 0;
          break;
        }
        break;
      }
      break;

    case 't': // timer
      _timer = millis() + (unsigned long)command->value;
      break;

    case 'p': // pause
      _pause = true;
      _timer = millis() + (unsigned long)command->value;
      return true;
    }
  }

  return true;
}

bool ProgramClass::run()
{
  if (_state != PROGRAM_STOP)
    return false;

  Stepper.setCurrentPosition(0);

  _state = PROGRAM_RUN;
  _cycle = 1;
  _line = 0;
  _timer = 0;
  _pause = false;

  Serial.println("Program: Run");

  return true;
}

bool ProgramClass::stop(int force)
{
  if (_state != PROGRAM_RUN)
    return false;

  force ? Stepper.setCurrentPosition(Stepper.currentPosition()) : Stepper.stop();
  _state = PROGRAM_STOP;

  Serial.println("Program: Stop");

  return true;
}

ProgramClass Program;
