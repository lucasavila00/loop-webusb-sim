import { Serial, Port } from "./serial";
import { Terminal } from "xterm";
import { Renderer } from "./renderer";
import { TERMINAL_OPTIONS } from "./constants";

class Main {
  private arduinoPort: Port | undefined = undefined;
  private textEncoder = new TextEncoder();
  private terminal = new Terminal(TERMINAL_OPTIONS);
  private connectButton = document.getElementById("connect");
  private terminalRendererEelement = document.getElementById("terminal");

  // three
  private renderer = new Renderer();

  constructor() {
    if (!this.terminalRendererEelement) {
      throw Error("No element with id terminal found when openning terminal");
    }
    this.terminal.open(this.terminalRendererEelement);

    this.registerTerminalKeyboardToSerial();
    this.registerClickHandlers();
    this.tryConnectingToArduino();

    this.renderer.animate();
    // this.renderer.render(); // remove when using next line for animation loop (requestAnimationFrame)
  }

  private connect = () => {
    if (this.arduinoPort == undefined) {
      throw Error("Port not registered when connecting");
    }
    this.terminal.writeln(
      "Connecting to " + this.arduinoPort.device_.productName + "...",
    );
    this.arduinoPort.connect().then(
      () => {
        console.log(this.arduinoPort);
        this.terminal.writeln("Connected.");
        if (this.connectButton == null) {
          throw Error("Connect button not found");
        }
        this.connectButton.textContent = "Disconnect";
        if (this.arduinoPort == undefined) {
          throw Error("Port not registered when connecting");
        }
        this.arduinoPort.setCallbacks(
          data => {
            const textDecoder = new TextDecoder();
            const dataDecoded = textDecoder.decode(data);
            const processed = this.processDedodedData(dataDecoded);
            if (!processed) {
              this.terminal.write(dataDecoded);
            }
          },
          error => {
            this.terminal.writeln("Receive error: " + error);
          },
        );
      },
      error => {
        this.terminal.writeln("Connection error: " + error);
      },
    );
  };

  private processDedodedData = (data: string): boolean => {
    if (data.startsWith("l")) {
      this.processLight(data);
      return true;
    } else if (data.startsWith("r")) {
      this.resetRenderer();
      return true;
    }
    return false;
  };

  private processLight = (data: string) => {
    //l-123-123-123-123
    const arr = data.split("-");
    const i = parseInt(arr[1]);
    const r = parseInt(arr[2]);
    const g = parseInt(arr[3]);
    const b = parseInt(arr[4]);
    this.renderer.light(i, r, g, b);
  };

  private resetRenderer = () => {
    this.renderer.reset();
  };

  private registerTerminalKeyboardToSerial() {
    this.terminal.onData(str => {
      if (this.arduinoPort !== undefined) {
        this.arduinoPort.send(this.textEncoder.encode(str)).catch(error => {
          this.terminal.writeln("Send error: " + error);
        });
      }
    });
  }

  private registerClickHandlers() {
    if (this.connectButton == null) {
      throw Error("Connect button not found");
    }

    this.connectButton.addEventListener("click", () => {
      if (this.arduinoPort) {
        this.arduinoPort.disconnect();
        if (this.connectButton == null) {
          throw Error("Connect button not found");
        }
        this.connectButton.textContent = "Connect";
        this.arduinoPort = undefined;
      } else {
        Serial.requestPort()
          .then(selectedPort => {
            this.arduinoPort = selectedPort;
            this.connect();
          })
          .catch(error => {
            this.terminal.writeln("Connection error: " + error);
          });
      }
    });
  }

  private tryConnectingToArduino() {
    Serial.getPorts().then(ports => {
      if (ports.length == 0) {
        this.terminal.writeln("No devices found.");
      } else {
        this.arduinoPort = ports[0];
        this.connect();
      }
    });
  }
}

document.addEventListener("DOMContentLoaded", _event => {
  new Main();
});
