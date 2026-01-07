#include "pid_tuner.h"
#include <WiFi.h>

pid_tuner::pid_tuner()
    : server(80), events("/events")
{
}

void pid_tuner::begin()
{
  const char *ssid = "PID_TUNER";
  const char *pw = "";

  WiFi.softAP(ssid, pw);
  WiFi.setTxPower(WIFI_POWER_13dBm);

  Serial.begin(9600);
  Serial.print("IP ADDRESS ESP32 : ");
  Serial.println(WiFi.softAPIP());

  server.on("/", HTTP_GET, [this](AsyncWebServerRequest *request)
            {
    AsyncWebServerResponse* response = request->beginResponse(200, "text/html", R"rawliteral(
    <!DOCTYPE html>
    <html>
      <head>
        <meta charset="UTF-8" />
        <meta name="viewport" content="width=device-width, initial-scale=1.0" />
        <title>PID Tuner ESP32</title>

        <style>
          .box {
            border-width: calc(0.1vw + 0.1vh);
            border-color: black;
            border-style: solid;
          }
          .main {
            gap: 1vw;
            display: flex;
            flex-direction: column;
          }
          .border {
            gap: 1vw;
            display: flex;
            flex-direction: row;
          }
          #box_1 {
            width: 25vw;
          }
          #box_5 {
            height: fit-content;
            margin-bottom: 0.5vw;
            padding-bottom: 0.4vw;
          }
          #box_buttonth,
          #boxSp {
            border: none;
          }
          .button_th {
            width: 3vw;
            margin: 0vw;
          }
          #button_motor {
            width: 10vw;
            font-weight: bold;
            margin-bottom: 0.2vw;
          }
          #indicator_motor {
            background-color: rgb(214, 0, 0);
            width: 1.5vw;
            height: 1.5vw;
            border-radius: 100%;
          }
          .row {
            margin: 1vw;
            margin-top: 0.1vw;
            margin-bottom: 0.1vw;
            display: flex;
            gap: 1.5vw;
          }
          #row_1 {
            gap: 5.5vw;
            margin-left: 2.8vw;
            margin-top: 1vh;
          }
          #row_1a {
            margin-top: 0.8vh;
          }
          #row_4 {
            margin-bottom: 1vh;
          }
          .input_number,
          .output_number,
          .output_number_a {
            font-size: 1vw;
            width: 4.3vw;
          }
          .text_1 {
            margin: 0;
            font-size: 1.2vw;
          }
          #box_judul {
            font-weight: bold;
          }
          #box_judul_1 {
            font-weight: bold;
            margin-bottom: 1.9vw;
          }

        </style>
      </head>
      <body>
        <div class="main">
          <div class="border" id="border_a">
            <div class="box" id="box_1">
              <div class="row">
                <p class="text_1" id="box_judul">INPUT OUTER LOOP</p>
              </div>
              <div class="row" id="row_1">
                <p class="text_1">P</p>
                <p class="text_1">I</p>
                <p class="text_1">D</p>
              </div>
              <div class="row" id="row_2">
                <input class="input_number" id="input_yaw_p" type="number"
                  step="0.001" min="0" max="10" value="0.000">
                <input class="input_number" id="input_yaw_i" type="number"
                  step="0.001" min="0" max="10" value="0.000">
                <input class="input_number" id="input_yaw_d" type="number"
                  step="0.001" min="0" max="10" value="0.000">
                <p class="text_1">YAW</p>
              </div>
              <div class="row" id="row_3">
                <input class="input_number" id="input_roll_p" type="number"
                  step="0.001" min="0" max="10" value="0.000">
                <input class="input_number" id="input_roll_i" type="number"
                  step="0.001" min="0" max="10" value="0.000">
                <input class="input_number" id="input_roll_d" type="number"
                  step="0.001" min="0" max="10" value="0.000">
                <p class="text_1">ROLL</p>
              </div>
              <div class="row" id="row_4">
                <input class="input_number" id="input_pitch_p" type="number"
                  step="0.001" min="0" max="10" value="0.000">
                <input class="input_number" id="input_pitch_i" type="number"
                  step="0.001" min="0" max="10" value="0.000">
                <input class="input_number" id="input_pitch_d" type="number"
                  step="0.001" min="0" max="10" value="0.000">
                <p class="text_1">PITCH</p>
              </div>
            </div>

            <div class="box" id="box_2">
              <div class="row">
                <p class="text_1" id="box_judul">OUTPUT OUTER LOOP</p>
              </div>
              <div class="row" id="row_1">
                <p class="text_1">P</p>
                <p class="text_1">I</p>
                <p class="text_1">D</p>
              </div>
              <div class="row" id="row_2">
                <input class="output_number" id="output_yaw_p" type="number"
                  step="0.001" min="-100" max="100" value="0.000" readonly>
                <input class="output_number" id="output_yaw_i" type="number"
                  step="0.001" min="-100" max="100" value="0.000" readonly>
                <input class="output_number" id="output_yaw_d" type="number"
                  step="0.001" min="-100" max="100" value="0.000" readonly>
                <p class="text_1">YAW</p>
              </div>
              <div class="row" id="row_3">
                <input class="output_number" id="output_roll_p" type="number"
                  step="0.001" min="-100" max="100" value="0.000" readonly>
                <input class="output_number" id="output_roll_i" type="number"
                  step="0.001" min="-100" max="100" value="0.000" readonly>
                <input class="output_number" id="output_roll_d" type="number"
                  step="0.001" min="-100" max="100" value="0.000" readonly>
                <p class="text_1">ROLL</p>
              </div>
              <div class="row" id="row_4">
                <input class="output_number" id="output_pitch_p" type="number"
                  step="0.001" min="-100" max="100" value="0.000" readonly>
                <input class="output_number" id="output_pitch_i" type="number"
                  step="0.001" min="-100" max="100" value="0.000" readonly>
                <input class="output_number" id="output_pitch_d" type="number"
                  step="0.001" min="-100" max="100" value="0.000" readonly>
                <p class="text_1">PITCH</p>
              </div>
            </div>

            <div class="box" id="box_3">
              <div class="row">
                <p class="text_1" id="box_judul_1">ANGLE</p>
              </div>
              <div class="row" id="row_2">
                <input class="output_number" id="angle_yaw" type="number"
                  step="0.001" min="0" max="10" value="00.000" readonly>
                <p class="text_1">YAW</p>
              </div>
              <div class="row" id="row_3">
                <input class="output_number" id="angle_roll" type="number"
                  step="0.001" min="0" max="10" value="00.000" readonly>
                <p class="text_1">ROLL</p>
              </div>
              <div class="row" id="row_4">
                <input class="output_number" id="angle_pitch" type="number"
                  step="0.001" min="0" max="10" value="00.000" readonly>
                <p class="text_1">PITCH</p>
              </div>
            </div>

            <div class="box" id="box_4">
              <div class="row">
                <p class="text_1" id="box_judul">PWM OUTPUT</p>
              </div>
              <div class="row" id="row_1a">
                <input class="output_number_a" id="MTR_1" type="number" step="0.001"
                  min="0" max="255" value="255.00" readonly>
                <p class="text_1">MTR_1</p>
              </div>
              <div class="row" id="row_2">
                <input class="output_number_a" id="MTR_2" type="number" step="0.001"
                  min="0" max="255" value="255.00" readonly>
                <p class="text_1">MTR_2</p>
              </div>
              <div class="row" id="row_3">
                <input class="output_number_a" id="MTR_3" type="number" step="0.001"
                  min="0" max="255" value="255.00" readonly>
                <p class="text_1">MTR_3</p>
              </div>
              <div class="row" id="row_4">
                <input class="output_number_a" id="MTR_4" type="number" step="0.001"
                  min="0" max="255" value="255.00" readonly>
                <p class="text_1">MTR_4</p>
              </div>
            </div>
          
            <div class="box" id="boxSp">
              <div class="box" id="box_5">
                <div class="row">
                  <p class="text_1" id="box_judul">THROTTLE</p>
                </div>
                <div class="row">
                  <input id="in_throttle" type="range" min="0" max="255" value="0">
                </div>
                <div class="row">
                  <input class="output_number_a" id="out_throttle" type="number"
                    step="1" min="0" max="255" value="0" readonly>
                  <div class="box" id="box_buttonth">
                    <input class="button_th" id="button_th1" type="button"
                      value="<">
                    <input class="button_th" id="button_th2" type="button"
                      value=">">
                  </div>
                </div>
              </div>

              <div class="box">
                <div class="row">
                  <p class="text_1" id="box_judul">ON/OFF MOTOR</p>
                </div>
                <div class="row">
                  <input id="button_motor" type="button" value="OFF">
                  <div id="indicator_motor"></div>
                </div>
              </div>
            </div>
          </div>

          <div class="border" id="border_b">
            <div class="box" id="box_9">
              <div class="row">
                <p class="text_1" id="box_judul">INPUT INNER LOOP</p>
              </div>
              <div class="row" id="row_1">
                <p class="text_1">P</p>
                <p class="text_1">I</p>
                <p class="text_1">D</p>
              </div>
              <div class="row" id="row_2">
                <input class="input_number" id="input_yaw_p_inner" type="number"
                  step="0.001" min="0" max="10" value="0.000">
                <input class="input_number" id="input_yaw_i_inner" type="number"
                  step="0.001" min="0" max="10" value="0.000">
                <input class="input_number" id="input_yaw_d_inner" type="number"
                  step="0.001" min="0" max="10" value="0.000">
                <p class="text_1">YAW</p>
              </div>
              <div class="row" id="row_3">
                <input class="input_number" id="input_roll_p_inner" type="number"
                  step="0.001" min="0" max="10" value="0.000">
                <input class="input_number" id="input_roll_i_inner" type="number"
                  step="0.001" min="0" max="10" value="0.000">
                <input class="input_number" id="input_roll_d_inner" type="number"
                  step="0.001" min="0" max="10" value="0.000">
                <p class="text_1">ROLL</p>
              </div>
              <div class="row" id="row_4">
                <input class="input_number" id="input_pitch_p_inner" type="number"
                  step="0.001" min="0" max="10" value="0.000">
                <input class="input_number" id="input_pitch_i_inner" type="number"
                  step="0.001" min="0" max="10" value="0.000">
                <input class="input_number" id="input_pitch_d_inner" type="number"
                  step="0.001" min="0" max="10" value="0.000">
                <p class="text_1">PITCH</p>
              </div>
            </div>
            <div class="box" id="box_2">
              <div class="row">
                <p class="text_1" id="box_judul">OUTPUT INNER LOOP</p>
              </div>
              <div class="row" id="row_1">
                <p class="text_1">P</p>
                <p class="text_1">I</p>
                <p class="text_1">D</p>
              </div>
              <div class="row" id="row_2">
                <input class="output_number" id="output_yaw_p_inner" type="number"
                  step="0.001" min="-100" max="100" value="0.000" readonly>
                <input class="output_number" id="output_yaw_i_inner" type="number"
                  step="0.001" min="-100" max="100" value="0.000" readonly>
                <input class="output_number" id="output_yaw_d_inner" type="number"
                  step="0.001" min="-100" max="100" value="0.000" readonly>
                <p class="text_1">YAW</p>
              </div>
              <div class="row" id="row_3">
                <input class="output_number" id="output_roll_p_inner" type="number"
                  step="0.001" min="-100" max="100" value="0.000" readonly>
                <input class="output_number" id="output_roll_i_inner" type="number"
                  step="0.001" min="-100" max="100" value="0.000" readonly>
                <input class="output_number" id="output_roll_d_inner" type="number"
                  step="0.001" min="-100" max="100" value="0.000" readonly>
                <p class="text_1">ROLL</p>
              </div>
              <div class="row" id="row_4">
                <input class="output_number" id="output_pitch_p_inner" type="number"
                  step="0.001" min="-100" max="100" value="0.000" readonly>
                <input class="output_number" id="output_pitch_i_inner" type="number"
                  step="0.001" min="-100" max="100" value="0.000" readonly>
                <input class="output_number" id="output_pitch_d_inner" type="number"
                  step="0.001" min="-100" max="100" value="0.000" readonly>
                <p class="text_1">PITCH</p>
              </div>
            </div>

            <div class="box" id="box_3">
              <div class="row">
                <p class="text_1" id="box_judul_1">GYRO</p>
              </div>
              <div class="row" id="row_2">
                <input class="output_number" id="gyro_yaw" type="number"
                  step="0.001" min="0" max="10" value="00.000" readonly>
                <p class="text_1">YAW</p>
              </div>
              <div class="row" id="row_3">
                <input class="output_number" id="gyro_roll" type="number"
                  step="0.001" min="0" max="10" value="00.000" readonly>
                <p class="text_1">ROLL</p>
              </div>
              <div class="row" id="row_4">
                <input class="output_number" id="gyro_pitch" type="number"
                  step="0.001" min="0" max="10" value="00.000" readonly>
                <p class="text_1">PITCH</p>
              </div>
            </div>

            <div class="box" id="boxSp">
              <div class="box" id="box_5">
                <div class="row">
                  <p class="text_1" id="box_judul">LOAD FILE</p>
                </div>
                <div class="row">
                  <input type="file" id="fileInput" accept=".txt">
                </div>
              </div>
              <div class="box" id="box_5">
                <div class="row">
                  <p class="text_1" id="box_judul">SAVE FILE</p>
                </div>
                <div class="row">
                  <input type="button" id="button_save" value="Save">
                </div>
              </div>
            </div>

          </div>
          <div class="border" id="border_a"> 
          
            <div class="box" id="box_trim">
              <div class="row">
                <p class="text_1" id="box_judul">TRIM MOTOR</p>
              </div>
              <div class="row" id="row_1a">
                <input class="input_number" id="trim_motor1" type="number" step="1"
                  min="-100" max="100" value="0">
                <p class="text_1">MTR_1</p>
              </div>
              <div class="row" id="row_2">
                <input class="input_number" id="trim_motor2" type="number" step="1"
                  min="-100" max="100" value="0">
                <p class="text_1">MTR_2</p>
              </div>
              <div class="row" id="row_3">
                <input class="input_number" id="trim_motor3" type="number" step="1"
                  min="-100" max="100" value="0">
                <p class="text_1">MTR_3</p>
              </div>
              <div class="row" id="row_4">
                <input class="input_number" id="trim_motor4" type="number" step="1"
                  min="-100" max="100" value="0">
                <p class="text_1">MTR_4</p>
              </div>
            </div>
          <div>
        </div>
        <script>
            const inputs = document.querySelectorAll('.input_number');
        
            // backend for html
            inputs.forEach(input => {
                input.addEventListener('blur', () => {
                    if(input.value == ""){
                        input.value = "0.000";
                    } 
                    // Remove the hard limit check here or adjust if needed for trims (-100 to 100)
                    kirimData(input.id, input.value);
                    console.log(input.id + "||" + input.value)
                });
            });

            var val_motor = 0;
            document.getElementById('in_throttle').addEventListener('input', function(){
                val_motor = parseInt(this.value);
                document.getElementById('out_throttle').value = val_motor;
                kirimData("in_throttle", val_motor);
            });

            document.getElementById('button_th1').addEventListener('click', function(){
              if(val_motor > 0) {
                val_motor -= 1;
                document.getElementById('in_throttle').value = val_motor;
                document.getElementById('in_throttle').dispatchEvent(new Event('input')); 
                console.log(val_motor);
              }
            });
            document.getElementById('button_th2').addEventListener('click', function(){
              if(val_motor < 255) {
                val_motor += 1;
                document.getElementById('in_throttle').value = val_motor;
                document.getElementById('in_throttle').dispatchEvent(new Event('input')); 
                console.log(val_motor);
              }
            });
            document.addEventListener('keydown', function(event) {
              if (event.key === "ArrowLeft") {
                if(val_motor > 0 ) {
                  val_motor -= 1;
                  document.getElementById('in_throttle').value = val_motor;
                  document.getElementById('in_throttle').dispatchEvent(new Event('input')); 
                }
              } else if (event.key === "ArrowRight") {
                console.log("Panah kanan ditekan");
                if(val_motor < 255) {
                  val_motor += 1;
                  document.getElementById('in_throttle').value = val_motor;
                  document.getElementById('in_throttle').dispatchEvent(new Event('input')); 
              }
              } else if (event.key === "ArrowDown") {
                console.log("bawah");
                button_state = 0;
                document.getElementById('button_motor').dispatchEvent(new Event('click'));
              }
            });
            var button_state = 1;
            document.getElementById('button_motor').addEventListener('click', function(){
              if(button_state == 0) {
                this.value = 'OFF';
                document.getElementById('indicator_motor').style.backgroundColor="rgb(214, 0, 0)";
                button_state = 1;
              } else if (button_state == 1) {
                this.value = 'ON';
                document.getElementById('indicator_motor').style.backgroundColor="rgb(0, 165, 0)";
                button_state = 0;
              }
              console.log(1-button_state);
              kirimData('button_motor', 1 - button_state);
            });
            document.getElementById('fileInput').addEventListener('change', function(){
              const file_dibaca = event.target.files[0];
              const reader = new FileReader();
              if (file_dibaca) {
                reader.onload = function(e) {
                  console.log(e.target.result);
                  let raw_data = e.target.result.trim();
                  let strArray = raw_data.split(",");
                  let floatArray = strArray
                                .filter(x => x.trim() !== "")
                                .map(x => parseFloat(x));
                  console.log(floatArray);  
                  
                  inputs.forEach((input, index) => {
                    if (index < floatArray.length) {
                      input.value = floatArray[index];
                      kirimData(input.id, input.value);
                    }
                  });
                }
              }
              reader.readAsText(file_dibaca);
            });
            document.getElementById('button_save').addEventListener('click', function(){
              let values = [];
              inputs.forEach(input => {
                if (input.value.trim() !== "") {
                  values.push(input.value);
                }
              });
              let text = values.join(",") + ",";
              const blob = new Blob([text], { type: "text/plain" });

              const a = document.createElement("a");
              a.href = URL.createObjectURL(blob);
              a.download = "data.txt";
              a.click();

              URL.revokeObjectURL(a.href);
            });
            // backend for send data
            function kirimData(idName, value) {
              var xhr = new XMLHttpRequest();
              xhr.open("POST", "/"+idName, true);
              xhr.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
              xhr.send("nilai=" + value);
            }
            
            //backend for receive data
            const evtSource = new EventSource('/events');
            evtSource.addEventListener("out_yaw_p", function(e) {
              document.getElementById('output_yaw_p').value = e.data;
            });
            evtSource.addEventListener("out_yaw_i", function(e) {
              document.getElementById('output_yaw_i').value = e.data;
            });
            evtSource.addEventListener("out_yaw_d", function(e) {
              document.getElementById('output_yaw_d').value = e.data;
            });

            evtSource.addEventListener("out_roll_p", function(e) {
              document.getElementById('output_roll_p').value = e.data;
            });
            evtSource.addEventListener("out_roll_i", function(e) {
              document.getElementById('output_roll_i').value = e.data;
            });
            evtSource.addEventListener("out_roll_d", function(e) {
              document.getElementById('output_roll_d').value = e.data;
            });

            evtSource.addEventListener("out_pitch_p", function(e) {
              document.getElementById('output_pitch_p').value = e.data;
            });
            evtSource.addEventListener("out_pitch_i", function(e) {
              document.getElementById('output_pitch_i').value = e.data;
            });
            evtSource.addEventListener("out_pitch_d", function(e) {
              document.getElementById('output_pitch_d').value = e.data;
            });

            evtSource.addEventListener("out_yaw_p_inner", function(e) {
              document.getElementById('output_yaw_p_inner').value = e.data;
            });
            evtSource.addEventListener("out_yaw_i_inner", function(e) {
              document.getElementById('output_yaw_i_inner').value = e.data;
            });
            evtSource.addEventListener("out_yaw_d_inner", function(e) {
              document.getElementById('output_yaw_d_inner').value = e.data;
            });

            evtSource.addEventListener("out_roll_p_inner", function(e) {
              document.getElementById('output_roll_p_inner').value = e.data;
            });
            evtSource.addEventListener("out_roll_i_inner", function(e) {
              document.getElementById('output_roll_i_inner').value = e.data;
            });
            evtSource.addEventListener("out_roll_d_inner", function(e) {
              document.getElementById('output_roll_d_inner').value = e.data;
            });

            evtSource.addEventListener("out_pitch_p_inner", function(e) {
              document.getElementById('output_pitch_p_inner').value = e.data;
            });
            evtSource.addEventListener("out_pitch_i_inner", function(e) {
              document.getElementById('output_pitch_i_inner').value = e.data;
            });
            evtSource.addEventListener("out_pitch_d_inner", function(e) {
              document.getElementById('output_pitch_d_inner').value = e.data;
            });

            evtSource.addEventListener("angle_yaw", function(e) {
              document.getElementById('angle_yaw').value = e.data;
            });
            evtSource.addEventListener("angle_roll", function(e) {
              document.getElementById('angle_roll').value = e.data;
            });
            evtSource.addEventListener("angle_pitch", function(e) {
              document.getElementById('angle_pitch').value = e.data;
            });

            evtSource.addEventListener("gyro_yaw", function(e) {
              document.getElementById('gyro_yaw').value = e.data;
            });
            evtSource.addEventListener("gyro_roll", function(e) {
              document.getElementById('gyro_roll').value = e.data;
            });
            evtSource.addEventListener("gyro_pitch", function(e) {
              document.getElementById('gyro_pitch').value = e.data;
            });
            evtSource.addEventListener("sendPWM_1", function(e) {
              document.getElementById('MTR_1').value = e.data;
            });
            evtSource.addEventListener("sendPWM_2", function(e) {
              document.getElementById('MTR_2').value = e.data;
            });
            evtSource.addEventListener("sendPWM_3", function(e) {
              document.getElementById('MTR_3').value = e.data;
            });
            evtSource.addEventListener("sendPWM_4", function(e) {
              document.getElementById('MTR_4').value = e.data;
            });
        </script>
      </body>
    </html>

    )rawliteral");
    request->send(response); });

  // handler setiap data yang diterima

  // --- HANDLER BARU UNTUK TRIM ---
  server.on("/trim_motor1", HTTP_POST, [this](AsyncWebServerRequest *request)
            {
    if (request->hasParam("nilai", true)) {
      trim_m1 = request->getParam("nilai", true)->value().toFloat();
      request->send(200, "text/plain", "OK");  
    } else {
      request->send(400, "text/plain", "Missing value");
    } });

  server.on("/trim_motor2", HTTP_POST, [this](AsyncWebServerRequest *request)
            {
    if (request->hasParam("nilai", true)) {
      trim_m2 = request->getParam("nilai", true)->value().toFloat();
      request->send(200, "text/plain", "OK");  
    } else {
      request->send(400, "text/plain", "Missing value");
    } });

  server.on("/trim_motor3", HTTP_POST, [this](AsyncWebServerRequest *request)
            {
    if (request->hasParam("nilai", true)) {
      trim_m3 = request->getParam("nilai", true)->value().toFloat();
      request->send(200, "text/plain", "OK");  
    } else {
      request->send(400, "text/plain", "Missing value");
    } });

  server.on("/trim_motor4", HTTP_POST, [this](AsyncWebServerRequest *request)
            {
    if (request->hasParam("nilai", true)) {
      trim_m4 = request->getParam("nilai", true)->value().toFloat();
      request->send(200, "text/plain", "OK");  
    } else {
      request->send(400, "text/plain", "Missing value");
    } });
  // --- END HANDLER TRIM ---

  server.on("/input_yaw_p_inner", HTTP_POST, [this](AsyncWebServerRequest *request)
            {
    if (request->hasParam("nilai", true)) {
      yaw_p_inner = request->getParam("nilai", true)->value().toFloat();
      request->send(200, "text/plain", "OK");  
    } else {
      request->send(400, "text/plain", "Missing value");
    } });
  server.on("/input_yaw_i_inner", HTTP_POST, [this](AsyncWebServerRequest *request)
            {
    if (request->hasParam("nilai", true)) {
      yaw_i_inner = request->getParam("nilai", true)->value().toFloat();
      request->send(200, "text/plain", "OK");  
    } else {
      request->send(400, "text/plain", "Missing value");
    } });
  server.on("/input_yaw_d_inner", HTTP_POST, [this](AsyncWebServerRequest *request)
            {
    if (request->hasParam("nilai", true)) {
      yaw_d_inner = request->getParam("nilai", true)->value().toFloat();
      request->send(200, "text/plain", "OK");  
    } else {
      request->send(400, "text/plain", "Missing value");
    } });
  server.on("/input_pitch_p_inner", HTTP_POST, [this](AsyncWebServerRequest *request)
            {
    if (request->hasParam("nilai", true)) {
      pitch_p_inner = request->getParam("nilai", true)->value().toFloat();
      request->send(200, "text/plain", "OK");  
    } else {
      request->send(400, "text/plain", "Missing value");
    } });
  server.on("/input_pitch_i_inner", HTTP_POST, [this](AsyncWebServerRequest *request)
            {
    if (request->hasParam("nilai", true)) {
      pitch_i_inner = request->getParam("nilai", true)->value().toFloat();
      request->send(200, "text/plain", "OK");  
    } else {
      request->send(400, "text/plain", "Missing value");
    } });
  server.on("/input_pitch_d_inner", HTTP_POST, [this](AsyncWebServerRequest *request)
            {
    if (request->hasParam("nilai", true)) {
      pitch_d_inner = request->getParam("nilai", true)->value().toFloat();
      request->send(200, "text/plain", "OK");  
    } else {
      request->send(400, "text/plain", "Missing value");
    } });
  server.on("/input_roll_p_inner", HTTP_POST, [this](AsyncWebServerRequest *request)
            {
    if (request->hasParam("nilai", true)) {
      roll_p_inner = request->getParam("nilai", true)->value().toFloat();
      request->send(200, "text/plain", "OK");  
    } else {
      request->send(400, "text/plain", "Missing value");
    } });
  server.on("/input_roll_i_inner", HTTP_POST, [this](AsyncWebServerRequest *request)
            {
    if (request->hasParam("nilai", true)) {
      roll_i_inner = request->getParam("nilai", true)->value().toFloat();
      request->send(200, "text/plain", "OK");  
    } else {
      request->send(400, "text/plain", "Missing value");
    } });
  server.on("/input_roll_d_inner", HTTP_POST, [this](AsyncWebServerRequest *request)
            {
    if (request->hasParam("nilai", true)) {
      roll_d_inner = request->getParam("nilai", true)->value().toFloat();
      request->send(200, "text/plain", "OK");  
    } else {
      request->send(400, "text/plain", "Missing value");
    } });
  server.on("/in_throttle", HTTP_POST, [this](AsyncWebServerRequest *request)
            {
    if (request->hasParam("nilai", true)) {
      throttle = request->getParam("nilai", true)->value().toInt();
      request->send(200, "text/plain", "OK");  
    } else {
      request->send(400, "text/plain", "Missing value");
    } });
  server.on("/button_motor", HTTP_POST, [this](AsyncWebServerRequest *request)
            {
    if (request->hasParam("nilai", true)) {
      motorState = request->getParam("nilai", true)->value().toInt();
      request->send(200, "text/plain", "OK");  
    } else {
      request->send(400, "text/plain", "Missing value");
    } });

  server.on("/input_yaw_p", HTTP_POST, [this](AsyncWebServerRequest *request)
            {
    if (request->hasParam("nilai", true)) {
      yaw_p = request->getParam("nilai", true)->value().toFloat();
      request->send(200, "text/plain", "OK");  
    } else {
      request->send(400, "text/plain", "Missing value");
    } });
  server.on("/input_yaw_i", HTTP_POST, [this](AsyncWebServerRequest *request)
            {
    if (request->hasParam("nilai", true)) {
      yaw_i = request->getParam("nilai", true)->value().toFloat();
      request->send(200, "text/plain", "OK");  
    } else {
      request->send(400, "text/plain", "Missing value");
    } });
  server.on("/input_yaw_d", HTTP_POST, [this](AsyncWebServerRequest *request)
            {
    if (request->hasParam("nilai", true)) {
      yaw_d = request->getParam("nilai", true)->value().toFloat();
      request->send(200, "text/plain", "OK");  
    } else {
      request->send(400, "text/plain", "Missing value");
    } });
  server.on("/input_pitch_p", HTTP_POST, [this](AsyncWebServerRequest *request)
            {
    if (request->hasParam("nilai", true)) {
      pitch_p = request->getParam("nilai", true)->value().toFloat();
      request->send(200, "text/plain", "OK");  
    } else {
      request->send(400, "text/plain", "Missing value");
    } });
  server.on("/input_pitch_i", HTTP_POST, [this](AsyncWebServerRequest *request)
            {
    if (request->hasParam("nilai", true)) {
      pitch_i = request->getParam("nilai", true)->value().toFloat();
      request->send(200, "text/plain", "OK");  
    } else {
      request->send(400, "text/plain", "Missing value");
    } });
  server.on("/input_pitch_d", HTTP_POST, [this](AsyncWebServerRequest *request)
            {
    if (request->hasParam("nilai", true)) {
      pitch_d = request->getParam("nilai", true)->value().toFloat();
      request->send(200, "text/plain", "OK");  
    } else {
      request->send(400, "text/plain", "Missing value");
    } });
  server.on("/input_roll_p", HTTP_POST, [this](AsyncWebServerRequest *request)
            {
    if (request->hasParam("nilai", true)) {
      roll_p = request->getParam("nilai", true)->value().toFloat();
      request->send(200, "text/plain", "OK");  
    } else {
      request->send(400, "text/plain", "Missing value");
    } });
  server.on("/input_roll_i", HTTP_POST, [this](AsyncWebServerRequest *request)
            {
    if (request->hasParam("nilai", true)) {
      roll_i = request->getParam("nilai", true)->value().toFloat();
      request->send(200, "text/plain", "OK");  
    } else {
      request->send(400, "text/plain", "Missing value");
    } });
  server.on("/input_roll_d", HTTP_POST, [this](AsyncWebServerRequest *request)
            {
    if (request->hasParam("nilai", true)) {
      roll_d = request->getParam("nilai", true)->value().toFloat();
      request->send(200, "text/plain", "OK");  
    } else {
      request->send(400, "text/plain", "Missing value");
    } });
  server.on("/in_throttle", HTTP_POST, [this](AsyncWebServerRequest *request)
            {
    if (request->hasParam("nilai", true)) {
      throttle = request->getParam("nilai", true)->value().toInt();
      request->send(200, "text/plain", "OK");  
    } else {
      request->send(400, "text/plain", "Missing value");
    } });
  server.on("/button_motor", HTTP_POST, [this](AsyncWebServerRequest *request)
            {
    if (request->hasParam("nilai", true)) {
      motorState = request->getParam("nilai", true)->value().toInt();
      request->send(200, "text/plain", "OK");  
    } else {
      request->send(400, "text/plain", "Missing value");
    } });
  // handler pengiriman data
  server.addHandler(&events);

  server.begin();
}

void pid_tuner::sendOutYaw_p(float value)
{
  events.send(String(value, 3), "out_yaw_p");
}

void pid_tuner::sendOutYaw_i(float value)
{
  events.send(String(value, 3), "out_yaw_i");
}

void pid_tuner::sendOutYaw_d(float value)
{
  events.send(String(value, 3), "out_yaw_d");
}

void pid_tuner::sendOutRoll_p(float value)
{
  events.send(String(value, 3), "out_roll_p");
}

void pid_tuner::sendOutRoll_i(float value)
{
  events.send(String(value, 3), "out_roll_i");
}

void pid_tuner::sendOutRoll_d(float value)
{
  events.send(String(value, 3), "out_roll_d");
}

void pid_tuner::sendOutPitch_p(float value)
{
  events.send(String(value, 3), "out_pitch_p");
}

void pid_tuner::sendOutPitch_i(float value)
{
  events.send(String(value, 3), "out_pitch_i");
}

void pid_tuner::sendOutPitch_d(float value)
{
  events.send(String(value, 3), "out_pitch_d");
}

void pid_tuner::sendAngle_yaw(float value)
{
  events.send(String(value, 3), "angle_yaw");
}

void pid_tuner::sendAngle_roll(float value)
{
  events.send(String(value, 3), "angle_roll");
}

void pid_tuner::sendAngle_pitch(float value)
{
  events.send(String(value, 3), "angle_pitch");
}

void pid_tuner::sendPWM_1(float value)
{
  events.send(String(value, 3), "sendPWM_1");
}

void pid_tuner::sendPWM_2(float value)
{
  events.send(String(value, 3), "sendPWM_2");
}

void pid_tuner::sendPWM_3(float value)
{
  events.send(String(value, 3), "sendPWM_3");
}

void pid_tuner::sendPWM_4(float value)
{
  events.send(String(value, 3), "sendPWM_4");
}
void pid_tuner::sendOutYawInner_p(float value)
{
  events.send(String(value, 3), "out_yaw_p_inner");
}

void pid_tuner::sendOutYawInner_i(float value)
{
  events.send(String(value, 3), "out_yaw_i_inner");
}

void pid_tuner::sendOutYawInner_d(float value)
{
  events.send(String(value, 3), "out_yaw_d_inner");
}

void pid_tuner::sendOutRollInner_p(float value)
{
  events.send(String(value, 3), "out_roll_p_inner");
}

void pid_tuner::sendOutRollInner_i(float value)
{
  events.send(String(value, 3), "out_roll_i_inner");
}

void pid_tuner::sendOutRollInner_d(float value)
{
  events.send(String(value, 3), "out_roll_d_inner");
}

void pid_tuner::sendOutPitchInner_p(float value)
{
  events.send(String(value, 3), "out_pitch_p_inner");
}

void pid_tuner::sendOutPitchInner_i(float value)
{
  events.send(String(value, 3), "out_pitch_i_inner");
}

void pid_tuner::sendOutPitchInner_d(float value)
{
  events.send(String(value, 3), "out_pitch_d_inner");
}

void pid_tuner::sendGyro_yaw(float value)
{
  events.send(String(value, 3), "angle_yaw_inner");
}

void pid_tuner::sendGyro_roll(float value)
{
  events.send(String(value, 3), "angle_roll_inner");
}

void pid_tuner::sendGyro_pitch(float value)
{
  events.send(String(value, 3), "angle_pitch_inner");
}

float pid_tuner::getYawP()
{
  return yaw_p;
}

float pid_tuner::getYawI()
{
  return yaw_i;
}

float pid_tuner::getYawD()
{
  return yaw_d;
}

float pid_tuner::getPitchP()
{
  return pitch_p;
}

float pid_tuner::getPitchI()
{
  return pitch_i;
}

float pid_tuner::getPitchD()
{
  return pitch_d;
}

float pid_tuner::getRollP()
{
  return roll_p;
}

float pid_tuner::getRollI()
{
  return roll_i;
}

float pid_tuner::getRollD()
{
  return roll_d;
}
float pid_tuner::getYawInnerP()
{
  return yaw_p_inner;
}

float pid_tuner::getYawInnerI()
{
  return yaw_i_inner;
}

float pid_tuner::getYawInnerD()
{
  return yaw_d_inner;
}

float pid_tuner::getPitchInnerP()
{
  return pitch_p_inner;
}

float pid_tuner::getPitchInnerI()
{
  return pitch_i_inner;
}

float pid_tuner::getPitchInnerD()
{
  return pitch_d_inner;
}

float pid_tuner::getRollInnerP()
{
  return roll_p_inner;
}

float pid_tuner::getRollInnerI()
{
  return roll_i_inner;
}

float pid_tuner::getRollInnerD()
{
  return roll_d_inner;
}
int pid_tuner::getThrottle()
{
  return throttle;
}

int pid_tuner::getMotorState()
{
  return motorState;
}

// --- GETTER TRIM ---
float pid_tuner::getTrimM1()
{
  return trim_m1;
}

float pid_tuner::getTrimM2()
{
  return trim_m2;
}

float pid_tuner::getTrimM3()
{
  return trim_m3;
}

float pid_tuner::getTrimM4()
{
  return trim_m4;
}