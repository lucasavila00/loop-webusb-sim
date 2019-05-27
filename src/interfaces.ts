export interface IColorObj {
  r: number;
  g: number;
  b: number;
}

export interface Renderer {
  process: (s: string) => boolean;
  printDebugInfo: () => void;
}
