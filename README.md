# Робот

Прошивка Arduino для управления цилиндрическим роботом с маятниковым управлением.

Возможности:

- WiFi точка доступа `Robot`
- DNS сервер
- WEB сервер
- API для управления шаговым двигателем и акселерометром с помощью http запросов

# Сборка прошивки

# API управления роботом

## Шаговый двигатель

### Скорость вращения, шагов в секунду

```
POST /stepper/speed?value=123
GET /stepper/speed
```

Допускается менять скорость вращения _на ходу_, прямо в процессе вращения. Однако, двигатель физически не может резко увеличить/сбросить скорость вращения, поэтому ему приходится менять ее плавно. То, как быстро двигатель наберет/сбросит скорость вращения, определяется параметром `ускорение`, шагов в секунду за секунду.

```
POST /stepper/acceleration?value=123
GET /stepper/acceleration
```

### Повернуться на указанное количество шагов

Положительное значение - по часовой стрелке, отрицательное - против часовой стрелки. Двигатлель начинает вращение сразу после получения команды. После поворота на указанное количество шагов двигатель останавливается.

```
POST /stepper/move?value=123
```

### Остановить вращение (стоп)

```
POST /stepper/stop
```

### Вращается ли двигатель

```
GET /stepper/isRunning
```

### Текущая скорость вращения

При подаче команды `повернуться` двигатель сначала плавно меняет `текущую скорость` от 0 до заданной, а затем продолжает вращаться с постоянной скоростью. Если задано малое количество шагов для поворота или небольшое  `ускорение`, то к моменту окончания вращения двигатель может не успеть набрать заданную скорость.

```
GET /stepper/currentSpeed
```

### Осталось шагов повернуться

Как только значение станет равным 0 двигатель остановится.

```
GET /stepper/distanceToGo
```

### Текущая и конечная позиции

На валу двигателя имеется две виртуальные метки - `текущая позиция` и `конечная позиция`.

При включении питания двигателя обе метки равны `0`. При подаче команды `повернуться` двигатель смещает `конечную` позицию относительно `текущей` на указанное число шагов и начинает вращение до тех пор, пока `текущая` позиция не достигнет `конечной`.

При подаче команды `стоп` обе позиции сбрасываются в 0.

```
GET /stepper/currentPosition
GET /stepper/targetPosition
```

### Количество шагов двигателя на один оборот

```
GET /stepper/stepsPerRevolution
```

### Сброс

Останавливает вращение, сбрасывает текущую и конечную позиции в 0. Используется для установки текущей позиции двигателя как нулевой позиции.

```
POST /stepper/reset
```
