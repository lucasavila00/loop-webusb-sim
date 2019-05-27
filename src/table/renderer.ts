import { Renderer, IColorObj } from "../interfaces";
import { pad0 } from "./helpers";
import { MAX } from "./constants";

const print_info = (writeln: (s: string) => void) => {
  writeln("Simulador da mesa 12x12 do Neopixels.");
  writeln("O código para subir no arduino e rodar neste simulador");
  writeln("pode ser encontrado em:");
  writeln(
    "https://github.com/degroote22/loop-webusb-sim/blob/d0237eb1d09590c3a038aa723da472f2b663e46d/sketches_table/test1/test1.ino",
  );
  writeln("");
  writeln(
    "Não se esqueça de instalar ou baixar a biblioteca WEBUSB para arduino, que está disponível em: ",
  );
  writeln(
    "https://github.com/webusb/arduino/tree/d7af7a03fbd263f717feca0b2131ef9cd99b77ca/library/WebUSB",
  );
  writeln("");
  writeln(
    `WebUSB só funciona em: Arduino Leonardo, Arduino/Genuino Micro, Arduino/Genuino Zero, Arduino MKR1000, Arduino MKRZero, Arduino MKRFox1200, Adafruit Feather 32u4`,
  );
  writeln("");
  setTimeout(() => {
    for (let i = 0; i < 20; i++) {
      writeln("");
    }
  }, 10000);
};
export class RendererTable implements Renderer {
  constructor(writeln: (s: string) => void) {
    print_info(writeln);
    for (let i = 0; i < MAX * MAX; i++) {
      this.lit[i] = { r: 0, g: 0, b: 0 };
    }

    let grid = document.createElement("div");
    grid.id = "grid";

    for (var i = 0; i < MAX; i++) {
      var row = document.createElement("div");
      row.className = "row";
      for (var x = 0; x < MAX; x++) {
        var cell = document.createElement("div");
        cell.className = "gridsquare";
        var gsn = i * MAX + x;
        cell.id = "gs-" + gsn;
        cell.style.backgroundColor = "#000000";
        row.appendChild(cell);
      }
      grid.appendChild(row);
    }

    document.body.appendChild(grid);
  }
  private lit: { [key: number]: IColorObj } = {};

  public printDebugInfo = () => {
    // nada pra imprimir nesse caso
  };

  public process = (_data: string): boolean => {
    const data = _data.substr(1, _data.length - 1);
    if (data.startsWith("l")) {
      this.processLight(data);
      return true;
    } else if (data.startsWith("r")) {
      this.reset();
      return true;
    } else if (data.startsWith("show")) {
      this.render();
      return true;
    }
    return false;
  };

  private render() {
    for (var i = 0; i < MAX; i++) {
      for (var j = 0; j < MAX; j++) {
        const { r, g, b } = this.lit[decodeGrid(i, j)];

        var decoded = decodeGrid(i, j);
        var element = document.getElementById("gs-" + decoded);

        var bg =
          "#" +
          pad0(r.toString(16)) +
          pad0(g.toString(16)) +
          pad0(b.toString(16));

        if (!element) {
          throw Error("Nao achou a célula numero " + decoded);
        }
        element.style.backgroundColor = bg;
      }
    }
  }
  private reset() {
    for (var i = 0; i < MAX; i++) {
      for (var j = 0; j < MAX; j++) {
        var gsn = decodeGrid(i, j);
        var element = document.getElementById("gs-" + gsn);
        if (!element) {
          throw Error("Nao achou a célula numero " + gsn);
        }
        element.style.backgroundColor = "#000000";
      }
    }
  }

  private light = (i: number, r: number, g: number, b: number) => {
    this.lit[i] = {
      r,
      g,
      b,
    };
  };

  private processLight = (data: string) => {
    //l-123-123123123
    const arr = data.split("-");
    const i = parseInt(arr[1]);

    const j = decodeFromNeopixel(i);
    const c = parseInt(arr[2]);

    const maskb = 255;
    const maskg = maskb << 8;
    const maskr = maskb << 16;

    const b = maskb & c;
    const g = (maskg & c) >> 8;
    const r = (maskr & c) >> 16;
    this.light(j, r, g, b);
  };
}

function decodeFromNeopixel(i: number) {
  //int decode(int i, int j)
  // {
  //     int column = i * 12;
  //     int row = i % 2 == 0 ? j : 11 - j;
  //     return column + row;
  // }
  const row = Math.floor(i / 12);
  const column = i % 12;
  return row * 12 + (row % 2 === 0 ? column : 11 - column);
}

function decodeGrid(i: number, j: number) {
  return i * MAX + j;
}
