import { pad0 } from "./helpers";
import * as THREE from "three";
import { MeshPhongMaterial } from "three";

const _cache: { [key: string]: MeshPhongMaterial } = {};

let lock = 0;
export function getMaterial(r: number, g: number, b: number) {
  var color = (r << 16) | (g << 8) | b;

  if (_cache[color] == undefined) {
    _cache[color] = new THREE.MeshPhongMaterial({
      color: color,
      flatShading: true,
    });
  }
  return _cache[color];
}
