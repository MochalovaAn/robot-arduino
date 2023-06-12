import requests

response = requests.get("http://172.16.0.1/stepper")
stepper_data = response.json()
print("GET /stepper", response.status_code, response.reason)
print(stepper_data)

response = requests.post("http://172.16.0.1/stepper?speed=1000&acceleration=500")
print("POST /stepper?speed=1000&acceleration=500", response.status_code, response.reason)

response = requests.post("http://172.16.0.1/stepper/rotate?steps=2048")
print("POST /stepper/rotate?steps=2048", response.status_code, response.reason)

response = requests.post("http://172.16.0.1/stepper/stop")
print("POST /stepper/stop", response.status_code, response.reason)

response = requests.post("http://172.16.0.1/stepper/stop?force=1")
print("POST /stepper/stop?force=1", response.status_code, response.reason)

response = requests.post("http://172.16.0.1/stepper/reset")
print("POST /stepper/reset", response.status_code, response.reason)

response = requests.post("http://172.16.0.1/stepper/reset?position=1024")
print("POST /stepper/reset?position=1024", response.status_code, response.reason)

response = requests.post("http://172.16.0.1/stepper/disable")
print("POST /stepper/reset", response.status_code, response.reason)

response = requests.post("http://172.16.0.1/stepper/enable")
print("POST /stepper/reset", response.status_code, response.reason)

response = requests.get("http://172.16.0.1/gyro")
gyro_data = response.json()
print("GET /gyro", response.status_code, response.reason)
print(gyro_data)

response = requests.get("http://172.16.0.1/program")
program_data = response.json()
print("GET /program", response.status_code, response.reason)
print(program_data)

response = requests.get("http://172.16.0.1/program/text")
program_text = response.text
print("GET /program/text", response.status_code, response.reason)
print(program_text)

program_text = "acceleration 500\r\nspeed 1000\r\nmove 2048\r\n"
response = requests.post("http://172.16.0.1/program/text?text=" + program_text)
print("POST program/text?text=", response.status_code, response.reason)

response = requests.post("http://172.16.0.1/program/run")
print("POST /program/run", response.status_code, response.reason)

response = requests.post("http://172.16.0.1/program/stop")
print("POST /program/stop", response.status_code, response.reason)
