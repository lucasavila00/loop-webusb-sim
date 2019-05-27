import { IHeightMap } from "./interfaces";

export const MAX = 300;
export const MAX_PER_QUARTER = MAX / 4;
export const RADIUS = 100;
export const CIRCUNFERENCE = 2 * Math.PI * RADIUS;
export const MAXIMUM_DISTANCE_IN_QUARTER = CIRCUNFERENCE / 2;

export const HEIGHT_MAP: IHeightMap[] = [
  { i: 0, z: 0 },
  { i: (1 * MAX) / 8, z: -25 },
  { i: (2 * MAX) / 8, z: 0 },
  { i: (3 * MAX) / 8, z: 0 },
  { i: (4 * MAX) / 8, z: 0 },
  { i: (5 * MAX) / 8, z: 25 },
  { i: (6 * MAX) / 8, z: 0 },
  { i: (7 * MAX) / 8, z: 0 },
];
