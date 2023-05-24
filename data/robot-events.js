var source = new EventSource("/events");

var prevData = { stepper: {}, program: {} };
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
    let firstEvent = false;

    if (timeOffset === 0) {
      timeOffset = Date.now() - data.millis;
      firstEvent = true;
    }

    // speed and acceleration values by program

    const direction = Math.sign(data.stepper.distanceToGo || prevData.stepper.distanceToGo);
    const stepperSpeed = data.stepper.speed * direction;

    if (prevData.program.isRunning || firstEvent) {
      if (data.stepper.isRunning && !power.checked) power.checked = true;

      if (!speedCapture && stepperSpeed !== speed.value) {
        speed.value = stepperSpeed;
        speed_hint.innerHTML = stepperSpeed;
      }
      if (!accelerationCapture && data.stepper.acceleration != acceleration.value) {
        acceleration.value = data.stepper.acceleration;
        acceleration_hint.innerHTML = data.stepper.acceleration;
      }
    }

    // control buttons

    if (prevData.stepper.isRunning !== data.stepper.isRunning) {
      start_stepper.disabled = data.stepper.isRunning;
      stop_stepper.disabled = !data.stepper.isRunning;
    }

    // program buttons and bage

    if (prevData.program.isRunning !== data.program.isRunning) {
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

    if (data.stepper.isRunning || prevData.stepper.isRunning) {
      const time = data.millis + timeOffset;

      appendChart(speed_chart, 0, time, stepperSpeed);
      appendChart(speed_chart, 1, time, data.stepper.currentSpeed);

      // appendChart(acceleration_chart, 0, time, (100 * Math.abs(data.gyro.ax)) / 2);
      // appendChart(acceleration_chart, 1, time, (100 * Math.abs(data.gyro.ay)) / 2);
      // appendChart(acceleration_chart, 2, time, (100 * Math.abs(data.gyro.az)) / 2);
      // appendChart(acceleration_chart, 3, time, (100 * data.stepper.acceleration) / data.stepper.stepsPerRevolution);

      const stepperPosition = data.stepper.currentPosition % data.stepper.stepsPerRevolution;
      const stepperAngle = Math.abs(360 * stepperPosition) / data.stepper.stepsPerRevolution;

      // appendChart(gyro_chart, 0, time, data.gyro.roll);
      // appendChart(gyro_chart, 1, time, data.gyro.pitch);
      // appendChart(gyro_chart, 2, time, data.gyro.yaw);
      appendChart(gyro_chart, 3, time, stepperAngle);
    }

    millis.innerHTML = `${data.millis} - ${data.clients}/10`;

    prevData = data;
  },
  false
);

speed.addEventListener("pointerdown", (e) => (speedCapture = true));
speed.addEventListener("change", (e) => (speedCapture = false));

acceleration.addEventListener("pointerdown", (e) => (accelerationCapture = true));
acceleration.addEventListener("change", (e) => (accelerationCapture = false));
