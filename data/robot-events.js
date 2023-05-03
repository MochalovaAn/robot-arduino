var source = new EventSource("/events");

var robot = { stepper: {}, program: {} };
var timeOffset = 0;
var speedCapture = false;
var accelerationCapture = false;

source.addEventListener(
  "open",
  (e) => {
    console.log("Robot connected");
    connection_alert.classList.add("d-none");
  },
  false
);

source.addEventListener(
  "error",
  (e) => {
    if (e.target.readyState != EventSource.OPEN) {
      console.log("Robot disconnected");
      connection_alert.classList.remove("d-none");
      millis.innerHTML = "Disconnected";
    }
  },
  false
);

source.addEventListener(
  "status",
  (e) => {
    const data = JSON.parse(e.data);

    if (timeOffset === 0) {
      timeOffset = Date.now() - data.millis;
    }

    // speed and acceleration values by program

    if (data.program.isRunning) {
      if (!speedCapture) {
        speed.value = data.stepper.speed * Math.sign(data.stepper.distanceToGo);
        speed_hint.innerHTML = speed.value;
      }
      if (!accelerationCapture) {
        acceleration.value = data.stepper.acceleration;
        acceleration_hint.innerHTML = acceleration.value;
      }
    }

    // control buttons

    if (robot.stepper.isRunning !== data.stepper.isRunning) {
      start_stepper.disabled = data.stepper.isRunning;
      stop_stepper.disabled = !data.stepper.isRunning;
    }

    // program buttons and bage

    if (robot.program.isRunning !== data.program.isRunning) {
      run_program.disabled = data.program.isRunning;
      stop_program.disabled = !data.program.isRunning;

      if (data.program.isRunning) {
        stop_bage.classList.add("d-none");
        run_bage.classList.remove("d-none");
      } else {
        run_bage.classList.add("d-none");
        stop_bage.classList.remove("d-none");
      }
    }

    // charts

    if (data.stepper.isRunning || robot.stepper.isRunning) {
      const time = data.millis + timeOffset;

      speed_chart.series[0].addPoint(
        [time, data.stepper.speed * Math.sign(data.stepper.distanceToGo)],
        true,
        speed_chart.series[0].points.length >= 300,
        false
      );
      speed_chart.series[1].addPoint(
        [time, data.stepper.currentSpeed],
        true,
        speed_chart.series[1].points.length >= 300,
        false
      );

      acceleration_chart.series[0].addPoint(
        [time, data.stepper.acceleration],
        true,
        acceleration_chart.series[0].points.length >= 300,
        false
      );
      acceleration_chart.series[1].addPoint(
        [time, data.gyro.ay],
        true,
        acceleration_chart.series[1].points.length >= 300,
        false
      );

      stepperAngle = data.stepper.currentPosition % 2048;
      if (stepperAngle > 1024) stepperAngle = stepperAngle - 2048;
      if (stepperAngle < -1024) stepperAngle = stepperAngle + 2048;

      gyro_chart.series[0].addPoint(
        [time, (360 * stepperAngle) / 2048],
        true,
        gyro_chart.series[0].points.length >= 300,
        false
      );
      gyro_chart.series[1].addPoint(
        [time, data.gyro.yaw],
        true,
        gyro_chart.series[1].points.length >= 300,
        false
      );
    }

    millis.innerHTML = `${data.millis} - ${data.clients}/10`;

    robot = data;
  },
  false
);

speed.addEventListener("pointerdown", (e) => (speedCapture = true));
speed.addEventListener("change", (e) => (speedCapture = false));

acceleration.addEventListener(
  "pointerdown",
  (e) => (accelerationCapture = true)
);
acceleration.addEventListener("change", (e) => (accelerationCapture = false));
