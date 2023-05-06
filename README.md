# Робот

Прошивка <a href="https://www.arduino.cc" target="_blank">Arduino</a> для управления цилиндрическим роботом с маятниковым управлением. Выпускная квалификационная работа магистра.

Мочалова Анастасия  
3821М1ПМкн ИТММ ННГУ  
<a href="mailto: anastasia.mochalova@google.com">anastasia.mochalova@google.com</a>

## Содержание

- [WiFi точка доступа](#wifi-точка-доступа)
- [WEB сервер](#web-сервер)
- [Программирование движения](#программирование-движения)
- [HTTP API](#http-api)
- [EventSource](#eventsource)

## WiFi точка доступа

Название: `Robot`  
Пароль:

## WEB сервер

Для управления и программирования робота может использоваться мобильный телефон или ноутбук.

После подключения к WiFi точке доступа робота автоматически откроется Web-страница управления. Если страница управления не открылась автоматически, запустите браузер и в адресной строке введите <a href="http://172.16.0.1" target="_blank">http://172.16.0.1</a>. Если страница загрузилась не полностью или вверху страницы выводится надпись `Связь с роботом потеряна`, то попробуйте перезагрузить страницу.

## Программирование движения

Чтобы робот начал движение, надо со Web-станицы управления или с помощью [HTTP API](#http-api) загрузить в него `программу` и запустить ее на исполнение.

### Программа

`Программа` - это набор `команд` для управления шаговым двигателем. Робот последовательно выполняет команды, шаговый двигатель вращается, грузик на валу двигателя меняет положение и приводит в движение робота.

`Программа` загружается в робот в виде текста, на каждой строчке которого расположена одна `команда` и ее `параметр` (при наличии). Параметр отделяется от команды пробелом или табуляцией.

Исполнение программы начинается с первой команды. После выполнения последней команды программа останавливается.

Пример программы:

```text
acceleration 500
speed 1000
rotate 4096
pause 5000
speed 500
timer 250
rotate -99999
stop 1
repeat
```

### Команды

Поддерживаемые команды программы и их параметры:

- `speed` - Установить скорость вращения. Параметр - значение скорости, шагов в секунду. Разрешается меять скорость _на ходу_ во время вращения двигателя.

- `acceleration` - Установить ускорение. Параметр - значение ускорения, шагов в секунду за секунду.

- `rotate` - Повернуться на указанное количество шагов. Параметр - количество шагов для поворота. Положительное значение - по часовой стрелке, отрицательное - против часовой стрелки. После поворота на указанное количество шагов двигатель останавливается. Вращение выполняется с заданной скоростью `speed`. Разгон и остановка делаются плавно с заданным ускорением `acceleration`.

- `timer` - Запустить таймер исполнения команды `rotate`. Параметр - время таймера, миллисекунд. По истечении заданного времени программа перейдет к следующей команде, даже если двигатель все еще вращается. Следующей командой может быть любая другая команда.

- `stop` - Остановить вращение. Если задан параметр 1, то двигатель остановится резко, иначе остановится плавно с заданным ускорением `acceleration`. Используется для остановки вращения по таймеру.

- `pause` - Сделать паузу. Параметр - время паузы, миллисекунд. Во время программа не выполняет никаких команд.

- `repeat` - Повторить программу с начала. Параметр отсутствует. Используется для зацикливания исполнения программы.

## HTTP API

Адрес сервера `HTTP-API`:

- `http://172.16.0.1:80`

Для получения параметров робота используется HTTP запрос `GET`.

Для управления шаговым двигателем и изменения программы используется HTTP запрос `POST`. Устанавливаемые значения передаются как параметры строки запроса. Если какой-либо параметр не указан, то его значение в роботе не меняется, и ошибки не возникает.

Шаговый двигатель:

- [GET /stepper](#get-stepper) - Получить параметры и состояние шагового двигателя.
- [POST /stepper](#post-stepper) - Установить параметры вращения шагового двигателя.
- [POST /stepper/rotate](#post-stepperrotate) - Повернуться на указанное количество шагов.
- [POST /stepper/stop](#post-stepperstop) - Остановить вращение (стоп).
- [POST /stepper/reset](#post-stepperreset) - Сброс.
- [POST /stepper/disable](#post-stepperdisable) - Отключить питание.
- [POST /stepper/enable](#post-stepperenable) - Включить питание.

Гироскоп:

- [GET /gyro](#get-gyro) - Получить параметры гироскопа.

Программа:

- [GET /program](#get-program) - Получить состояние программы.
- [GET /program/text](#get-programtext) - Получить текущую программу.
- [POST /program/text](#post-programtext) - Загрузить новую программу.
- [POST /propgram/run](#post-programrun) - Выполнить программу.
- [POST /propgram/stop](#post-programstop) - Остановить программу.

### Шаговый двигатель

#### GET /stepper

Получить параметры и состояние шагового двигателя.

Возвращает параметры в теле ответа в формате `application/json`:

- `speed` - Заданная скорость вращения, шагов в секунду.

- `acceleration` - Заданное ускорение, шагов в секунду за секунду. Двигатель физически не может резко увеличить/сбросить скорость вращения, поэтому ему приходится менять ее плавно. `acceleration` определяет, как быстро двигатель наберет/сбросит заданную скорость вращения `speed`.

- `currentSpeed` - Текущая скорость вращения, шагов в секунду. При подаче команды `rotate` двигатель сначала плавно меняет текущую скорость от 0 до заданной, а затем продолжает вращаться с постоянной скоростью. Если задано малое количество шагов для поворота или небольшое ускорение, то к моменту окончания вращения двигатель может не успеть набрать заданную скорость.

- `currentPosition` - Текущая позиция. На валу двигателя имеется две виртуальные метки: `текущая позиция` и `конечная позиция`. При включении питания двигателя обе метки равны 0. При подаче команды `rotate` двигатель смещает `конечную` позицию относительно `текущей` на указанное число шагов и начинает вращение до тех пор, пока `текущая` позиция не достигнет `конечной`.

- `targetPosition` - Конечная позиция.

- `distanceToGo` - Осталось шагов повернуться. Разница между `currentPosition` и `targetPosition`. Как только значение станет равным 0 двигатель остановится.

- `isRunning` - Вращается ли двигатель.

- `stepsPerRevolution` - Количество шагов двигателя на один оборот. Значение по умолчанию `4096`.

Пример запроса (Python):

```python
import requests

response = requests.get("http://172.16.0.1/stepper")
stepper_data = response.json()

print(response.status_code, response.reason)
print(stepper_data)
```

Пример ответа (JSON):

```json
{
  "acceleration": 1,
  "speed": 1,
  "currentSpeed": 0,
  "currentPosition": 0,
  "targetPosition": 0,
  "distanceToGo": 0,
  "isRunning": false,
  "stepsPerRevolution": 4096
}
```

#### POST /stepper

Установить параметры вращения шагового двигателя.

Параметры запроса:

- `acceleration` - Ускорение, шагов в секунду за секунду.
- `speed` - Заданная скорость вращения, шагов в секунду.
- `rotate` - Повернуться на указанное количество шагов. Положительное значение - по часовой стрелке, отрицательное - против часовой стрелки. Если параметр указан, то двигатель сразу начинает вращение.

Допускается менять параметры _на ходу_ во время вращения двигателя.

Пример запроса (Python):

```python
import requests

response = requests.post("http://172.16.0.1/stepper?speed=1000&acceleration=500")
print(response.status_code, response.reason)
```

#### POST /stepper/rotate

Повернуться на указанное количество шагов.

Параметры запроса:

- `steps` - количество шагов для поворота. Положительное значение - по часовой стрелке, отрицательное - против часовой стрелки. Если параметр не задан, то запрос игнорируется.

Двигатель начинает вращение сразу после получения команды. После поворота на указанное количество шагов двигатель останавливается. Вращение выполняется с заданной скоростью `speed`. Разгон и остановка делаются плавно с заданным ускорением `acceleration`.

Пример запроса (Python):

```python
import requests

response = requests.post("http://172.16.0.1/stepper/rotate?steps=4096")
print(response.status_code, response.reason)
```

#### POST /stepper/stop

Остановить вращение (стоп).

Параметры запроса:

- `force` - Режим остановки плавно/резко.

Двигатель останавливается сразу после получения запроса. Если параметр `force` не задан или равен 0, то останавливает плавно с заданным ускорением. Иначе останавливает резко. `Заданная скорость` и `ускорение` и после остановки не меняются.

Пример запроса 1 (Python):

```python
import requests

response = requests.post("http://172.16.0.1/stepper/stop")
print(response.status_code, response.reason)
```

Пример запроса 2 (Python):

```python
import requests

response = requests.post("http://172.16.0.1/stepper/stop?force=1")
print(response.status_code, response.reason)
```

#### POST /stepper/reset

Сброс.

Параметры запроса:

- `position` - Новая текущая позиция двигателя, шагов.

Резко останавливает вращение. Сбрасывает `текущую позицию` в указанную или в 0, если параметр `position` не указан. `Заданная скорость` и `ускорение` не меняются. Используется для установки текущей позиции двигателя как нулевой позиции.

Пример запроса 1 (Python):

```python
import requests

response = requests.post("http://172.16.0.1/stepper/reset")
print(response.status_code, response.reason)
```

Пример запроса 2 (Python):

```python
import requests

response = requests.post("http://172.16.0.1/stepper/reset?position=1024")
print(response.status_code, response.reason)
```

#### POST /stepper/disable

Отключить питание.

Отключает выходы упралвения двигателем, установливая для них НИЗКИЙ уровень.

Пример запроса (Python):

```python
import requests

response = requests.post("http://172.16.0.1/stepper/disable")
print(response.status_code, response.reason)
```

#### POST /stepper/enable

Включить питание.

Включает выходы управления двигателем, устанавливая для них ВЫСОКИЙ уровень.

Пример запроса (Python):

```python
import requests

response = requests.post("http://172.16.0.1/stepper/enable")
print(response.status_code, response.reason)
```

### Гироскоп

#### GET /gyro

Получить параметры гироскопа.

Возвращает параметры в теле ответа в формате `application/json`:

- `roll` - Крен (крыло вверх/вниз), вокруг оси X.
- `pitch` - Тангаж (нос вверх/вниз), вокруг оси Y.
- `yaw` - Рыскание (нос влево/вправо), вокруг оси Z.
- `t` - Температура датчика, градус C.
- `deviceID` - Номер устройства гироскопа или 0, если гироскоп не подключен.

Пример запроса (Python):

```python
import requests

response = requests.get("http://172.16.0.1/gyro")
gyro_data = response.json()

print(response.status_code, response.reason)
print(gyro_data)
```

Пример ответа (JSON):

```json
{
  "deviceID": 0,
  "roll": 0,
  "yaw": 180,
  "pitch": 0,
  "t": 36.52999878
}
```

### Программа

#### GET /program

Получить состояние программы.

Возвращает состояние программы в теле ответа в формате `application/json`:

- `state` - Состояние программы: 0 - программа не згружена, 1 - программа остановлена/завершена, 2 - программа выполняется.
- `count` - Количество команд программы.
- `line` - Текущая исполняемая строка программы.
- `cycle` - Количество выполненных циклов программы, если в программе есть команда `repeat`.
- `isRunning` - Исполняется ли программа.

Пример запроса (Python):

```python
import requests

response = requests.get("http://172.16.0.1/program")
program_data = response.json()

print(response.status_code, response.reason)
print(program_data)
```

Пример ответа (JSON):

```json
{
  "state": 2,
  "count": 6,
  "line": 1,
  "cycle": 1,
  "isRunning": true
}
```

#### GET /program/text

Получить текущую программу.

Возвращает текст текущей программы в теле ответа в формате `text/plain` в кодировке `utf-8`.

Пример запроса (Python):

```python
import requests

response = requests.get("http://172.16.0.1/program/text")
program_text = response.text

print(response.status_code, response.reason)
print(program_text)
```

Пример ответа (plain text):

```
acceleration 500
speed 1000
rotate 4096
speed 500
rotate -4096
repeat
```

#### POST /program/text

Загрузка новой программы.

Параметры запроса:

- `text` - текст новой программы.

Текст программы также может быть указан в теле запроса в формате `text/plain` в кодировке `utf-8`.

Пример запроса 1 (Python):

```python
import requests

program_text = "acceleration 500\r\nspeed 1000\r\nrotate 4096\r\n"

response = requests.post("http://172.16.0.1/program/text?text=" + program_text)
print(response.status_code, response.reason)
```

Пример запроса 2 (Python):

```python
import requests

headers = { 'Content-Type': 'text/plain' }
program_text = "acceleration 500\r\nspeed 1000\r\nrotate 4096\r\n"

response = requests.post("http://172.16.0.1/program/text", headers=headers, data=program_text)
print(response.status_code, response.reason)
```

#### POST /program/run

Выполнить программу.

Если программа не загружена или уже исполняется, то команда игнорируется.

Пример запроса (Python):

```python
import requests

response = requests.post("http://172.16.0.1/program/run")
print(response.status_code, response.reason)
```

#### POST /program/stop

Остановить программу.

Пример запроса (Python):

```python
import requests

response = requests.post("http://172.16.0.1/program/stop")
print(response.status_code, response.reason)
```

## EventSource

`EventSource` интерфейс используется для получения серверных событий (server-sent events, SSE) об изменении параметров робота.

URL событий:

- `/events` - события робота.

События робота:

- [status](#status) - Изменение текущего состояние робота.

### События

#### status

Событие изменения текущего состояния робота. Вызывается каждые 100 мс.

Данные события - объект JSON:

- `millis` - Количество миллисекунд с момента включения робота.
- `clients` - Количество подключенных к роботу WiFi клиентов.
- `stepper` - Текущие параметры и состояние шагового двигателя.
- `gyro` - Текущие параметры гироскопа.
- `program` - Текущее состояние программы.

Пример использования (JavaScript):

```javascript
var source = new EventSource("/events");

source.addEventListener(
  "status",
  (event) => console.log("Yaw", event.data.gyro.yaw),
  false
);
```

Пример данных события (JSON):

```json
{
  "millis": 192969,
  "clients": 1,
  "stepper": {
    "acceleration": 1,
    "speed": 1,
    "currentSpeed": 0,
    "currentPosition": 0,
    "targetPosition": 0,
    "distanceToGo": 0,
    "isRunning": false,
    "stepsPerRevolution": 4096
  },
  "gyro": {
    "deviceID": 0,
    "roll": 0,
    "pitch": 0,
    "yaw": 180,
    "t": 36.52999878
  },
  "program": {
    "count": 6,
    "line": 1,
    "cycle": 1,
    "isRunning": false
  }
}
```
