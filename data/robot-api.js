function powerOn() {
  return fetch("/stepper/enable", { method: "POST" })
    .then((response) => console.log("Power on:", response.status))
    .catch((e) => console.error("Power on error:", e));
}

function powerOff() {
  return stopStepper(1)
    .then(() => fetch("/stepper/disable", { method: "POST" }))
    .then((response) => console.log("Power off:", response.status))
    .catch((e) => console.error("Power off error:", e));
}

function setSpeed(value) {
  return fetch(`/stepper?speed=${Math.abs(value)}`, { method: "POST" })
    .then((response) => console.log("Set speed:", response.status))
    .then(() => (prevData.stepper.isRunning ? rotate(value) : undefined))
    .catch((e) => console.error("Set speed error.", e));
}

function setAcceleration(value) {
  return fetch(`/stepper?acceleration=${value}`, { method: "POST" })
    .then((response) => console.log("Set acceleration:", response.status))
    .catch((e) => console.error("Set acceleration error.", e));
}

function rotate(speed) {
  const direction = Math.sign(speed);
  const steps = direction * 100500000;
  return fetch(`/stepper/rotate?steps=${steps}`, { method: "POST" })
    .then((response) => console.log("Start:", response.status))
    .catch((e) => console.error("Start error.", e));
}

function startStepper(speed, acceleration) {
  return powerOn()
    .then(() => setSpeed(speed))
    .then(() => setAcceleration(acceleration))
    .then(() => rotate(speed));
}

function stopStepper(force) {
  return fetch(`/stepper/stop?force=${force}`, { method: "POST" })
    .then((response) => console.log("Stop:", response.status))
    .catch((e) => console.error("Stop:", e));
}

function resetStepper() {
  return fetch("/stepper/reset", { method: "POST" })
    .then((response) => console.log("Reset:", response.status))
    .then(() => setTimeout(() => clearCharts(), 100))
    .catch((e) => console.error("Reset error.", e));
}

function downloadProgram() {
  return fetch("/program/text", { method: "GET" })
    .then((response) => {
      console.log("Download program:", response.status);
      return response.text();
    })
    .catch((e) => console.error("Download program error.", e));
}

function uploadProgram(text) {
  const params = new URLSearchParams();
  params.append("text", text);
  return fetch(`/program/text?${params.toString()}`, { method: "POST" })
    .then((response) => console.log("Upload program:", response.status))
    .catch((e) => console.error("Upload program error.", e));
}

function runProgram(text) {
  return uploadProgram(text)
    .then(() => powerOn())
    .then(() => fetch("/program/run", { method: "POST" }))
    .then((response) => console.log("Run program:", response.status))
    .catch((e) => console.error("Run program error.", e));
}

function stopProgram() {
  return fetch("/program/stop", { method: "POST" })
    .then((response) => console.log("Stop program:", response.status))
    .catch((e) => console.error("Stop program error.", e));
}

function readTextFile(file) {
  return new Promise((resolve, reject) => {
    if (file.size > 10 * 1024) {
      reject(new Error("File too large"));
    } else {
      const reader = new FileReader();
      reader.onload = () => resolve(reader.result);
      reader.onerror = () => reject(reader.error);
      reader.readAsText(file);
    }
  });
}

function saveTextFile(text) {
  var blob = new Blob([text], { type: "text/plain;charset=utf-8" });
  saveAs(blob, "program.txt");
}
