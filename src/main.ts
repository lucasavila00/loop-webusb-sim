import { Serial, Port } from "./serial";
import { Terminal } from "xterm";
import { RendererLoop } from "./loop/renderer";
import { TERMINAL_OPTIONS } from "./constants";
import { Renderer } from "./interfaces";
import { RendererTable } from "./table/renderer";

function getRenderer({ writeln }: { writeln: (s: string) => void }): Renderer {
  if ((window as any).renderer_type === "LOOP") {
    return new RendererLoop(writeln);
  } else if ((window as any).renderer_type === "TABLE") {
    return new RendererTable(writeln);
  } else {
    throw Error(
      "It wasn't possible to determine the type of renderer based on window.renderer_type",
    );
  }
}

class Main {
  private arduinoPort: Port | undefined = undefined;
  private textEncoder = new TextEncoder();
  private terminal = new Terminal(TERMINAL_OPTIONS);
  private connectButton = document.getElementById("connect");
  private terminalRendererEelement = document.getElementById("terminal");
  private renderer: Renderer; //inicializa depois que já tiver inicializado o terminal

  constructor() {
    if (!this.terminalRendererEelement) {
      throw Error("No element with id 'terminal' found when openning terminal");
    }
    this.terminal.open(this.terminalRendererEelement);

    this.registerTerminalKeyboardToSerial();
    this.registerClickHandlers();
    this.tryConnectingToArduino();

    this.renderer = getRenderer({
      writeln: s => {
        this.terminal.writeln(s);
      },
    });
  }

  private connect = () => {
    if (this.arduinoPort == undefined) {
      throw Error("Port not registered when connecting");
    }
    this.terminal.writeln(
      "Conectando a " + this.arduinoPort.device_.productName + "...",
    );
    this.arduinoPort.connect().then(
      () => {
        console.log(this.arduinoPort);
        this.terminal.writeln("Conectado.");
        if (this.connectButton == null) {
          throw Error("Connect button not found");
        }
        this.connectButton.textContent = "Desconectar";
        if (this.arduinoPort == undefined) {
          throw Error("Port not registered when connecting");
        }
        this.arduinoPort.setCallbacks(
          data => {
            const textDecoder = new TextDecoder();
            const dataDecoded = textDecoder.decode(data);
            const processed = this.processDecodedData(dataDecoded);
            if (!processed) {
              this.terminal.write("Recebido: " + dataDecoded);
            }
          },
          error => {
            this.terminal.writeln("Erro ao receber: " + error);
          },
        );
      },
      error => {
        this.terminal.writeln("Erro de conexão: " + error);
      },
    );
  };

  private processDecodedData = (data: string): boolean => {
    if (data.startsWith("_")) {
      return this.renderer.process(data);
    }
    return false;
  };

  private registerTerminalKeyboardToSerial() {
    this.terminal.onData(str => {
      if (this.arduinoPort !== undefined) {
        this.arduinoPort.send(this.textEncoder.encode(str)).catch(error => {
          this.terminal.writeln("Erro de envio: " + error);
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
        this.connectButton.textContent = "Conectar";
        this.arduinoPort = undefined;
      } else {
        Serial.requestPort()
          .then(selectedPort => {
            this.arduinoPort = selectedPort;
            this.connect();
          })
          .catch(error => {
            this.terminal.writeln("Erro de conexão: " + error);
          });
      }
    });
  }

  private tryConnectingToArduino() {
    Serial.getPorts().then(ports => {
      if (ports.length == 0) {
        this.terminal.writeln("Nenhum dispositivo encontrado.");
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
