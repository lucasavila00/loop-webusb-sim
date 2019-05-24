export interface IColorObj {
  r: number;
  g: number;
  b: number;
}

export interface IHeightMap {
  i: number;
  z: number;
}

export interface IPosition {
  x: number;
  y: number;
  z: number;
}

export interface Renderer {
  process: (s: string) => boolean;
  printDebugInfo: () => void;
}
