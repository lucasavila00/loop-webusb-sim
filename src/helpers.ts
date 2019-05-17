import {
  MAX,
  MAX_PER_QUARTER,
  MAXIMUM_DISTANCE_IN_QUARTER,
  RADIUS,
  HEIGHT_MAP,
} from "./constants";
import { IHeightMap, IPosition } from "./interfaces";

export function easeInOutQuad(t: number): number {
  return t < 0.5 ? 2 * t * t : -1 + (4 - 2 * t) * t;
}

export function pad0(s: string): string {
  return s.length == 1 ? "0" + s : s;
}

export function toMeshPos(i: number): number {
  if (i < MAX / 4) {
    return i;
  } else if (i < (2 * MAX) / 4) {
    return (4 * MAX) / 4 - (i % MAX_PER_QUARTER) - 1;
  } else if (i < (3 * MAX) / 4) {
    return i;
  } else {
    return (2 * MAX) / 4 - (i % MAX_PER_QUARTER) - 1;
  }
}

export function getHeight(i: number): number {
  let smallest: IHeightMap | undefined;
  HEIGHT_MAP.forEach(function(h) {
    if (h.i <= i) {
      smallest = h;
    }
  });

  HEIGHT_MAP.forEach(function(h) {
    if (h.i > i && !biggest) {
      biggest = h;
    }
  });

  let biggest: IHeightMap | undefined;
  if (!biggest) {
    biggest = HEIGHT_MAP[0];
  }

  if (smallest == undefined) {
    // Esse erro nunca vai acontecer
    throw Error("Smallest from this position not found");
  }
  if (smallest.i == i) {
    return smallest.z;
  }
  if (biggest.i == i) {
    return biggest.z;
  }
  var beg = smallest.i;
  var end = biggest.i == 0 ? MAX - 1 : biggest.i;
  var dif = end - beg;
  var height_dif = biggest.z - smallest.z;
  var z = (height_dif / dif) * (i - smallest.i);

  return (
    smallest.z +
    (height_dif === 0 ? 0 : easeInOutQuad(Math.abs(z / height_dif))) *
      height_dif
  );
}

export function getPosition(i: number): IPosition {
  // decide qual quadrante é.
  if (i >= (3 * MAX) / 4) {
    // primeira curva
    return {
      x:
        // -maximum_distance / MAX_PER_QUARTER +
        MAXIMUM_DISTANCE_IN_QUARTER / 2 +
        RADIUS *
          Math.sin(
            ((i % MAX_PER_QUARTER) * (180 / MAX_PER_QUARTER) * Math.PI) / 180,
          ),
      y: getHeight(i),
      z:
        RADIUS *
        Math.cos(
          ((i % MAX_PER_QUARTER) * (180 / MAX_PER_QUARTER) * Math.PI) / 180,
        ),
    };
  } else if (i >= (2 * MAX) / 4) {
    // primeira reta
    // a maior distancia é metáde do círculo de raio tal
    return {
      x:
        -MAXIMUM_DISTANCE_IN_QUARTER / MAX_PER_QUARTER +
        MAXIMUM_DISTANCE_IN_QUARTER / 2 -
        ((i % MAX_PER_QUARTER) * MAXIMUM_DISTANCE_IN_QUARTER) / MAX_PER_QUARTER,
      y: getHeight(i),
      z: RADIUS,
    };
  } else if (i >= MAX / 4) {
    return {
      x:
        -MAXIMUM_DISTANCE_IN_QUARTER / 2 +
        RADIUS *
          Math.sin(
            ((i % MAX_PER_QUARTER) * (180 / MAX_PER_QUARTER) * Math.PI) / 180 +
              Math.PI,
          ),
      y: getHeight(i),
      z:
        RADIUS *
        Math.cos(
          ((i % MAX_PER_QUARTER) * (180 / MAX_PER_QUARTER) * Math.PI) / 180 +
            Math.PI,
        ),
    };
    // segunda curva
  } else {
    // segunda reta
    // a maior distancia é metáde do círculo de raio tal
    return {
      x:
        MAXIMUM_DISTANCE_IN_QUARTER / MAX_PER_QUARTER +
        -(MAXIMUM_DISTANCE_IN_QUARTER / 2) +
        (i * MAXIMUM_DISTANCE_IN_QUARTER) / MAX_PER_QUARTER,
      y: getHeight(i),
      z: -RADIUS,
    };
  }
}
