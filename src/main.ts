import { Serial, Port } from "./serial";
import { Terminal } from "xterm";
import { RendererLoop } from "./renderer_loop";
import { TERMINAL_OPTIONS } from "./constants";
import { Renderer } from "./interfaces";
import { getRendererType } from "./helpers";
import { RendererTable } from "./renderer_table";

class Main {
  private arduinoPort: Port | undefined = undefined;
  private textEncoder = new TextEncoder();
  private terminal = new Terminal(TERMINAL_OPTIONS);
  private connectButton = document.getElementById("connect");
  private terminalRendererEelement = document.getElementById("terminal");

  // three
  private renderer: Renderer =
    getRendererType() === "LOOP" ? new RendererLoop() : new RendererTable();

  constructor() {
    if (!this.terminalRendererEelement) {
      throw Error("No element with id terminal found when openning terminal");
    }
    this.terminal.open(this.terminalRendererEelement);

    this.registerTerminalKeyboardToSerial();
    this.registerClickHandlers();
    this.tryConnectingToArduino();
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
            const processed = this.processDecodedData(dataDecoded);
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

  private processDecodedData = (data: string): boolean => {
    // console.log({ data });
    if (data.startsWith("_")) {
      return this.renderer.process(data);
    }
    return false;
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
  public printHeightList = () => {
    this.renderer.printDebugInfo();
  };
}

document.addEventListener("DOMContentLoaded", _event => {
  var controller = new Main();
  controller.printHeightList();
});
