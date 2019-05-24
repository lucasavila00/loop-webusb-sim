import { Renderer, IColorObj } from "./interfaces";
import { pad0 } from "./helpers";
const MAX = 12;
export class RendererTable implements Renderer {
  constructor() {
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

  public printDebugInfo = () => {};

  public lock = 0;
  public process = (_data: string): boolean => {
    if (this.lock < 100) {
      this.lock++;
      console.log({ _data });
    }
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
