import { pad0 } from "./helpers";
import * as THREE from "three";
import { MeshPhongMaterial } from "three";

const _cache: { [key: string]: MeshPhongMaterial } = {};

export function getMaterial(r: number, g: number, b: number) {
  var color =
    pad0(r.toString(16)) + pad0(g.toString(16)) + pad0(b.toString(16));

  if (_cache[color] == undefined) {
    _cache[color] = new THREE.MeshPhongMaterial({
      color: parseInt(color, 16),
      flatShading: true,
    });
  }
  return _cache[color];
}
