import { MAX } from "./constants";
import * as THREE from "three";
import "three/examples/js/WebGL";
import { getPosition, toMeshPos, getHeight } from "./helpers";
import { getMaterial } from "./material_cache";
import { IColorObj, Renderer } from "../interfaces";
import { OrbitControls } from "three-orbitcontrols-ts";

const print_info = (writeln: (s: string) => void) => {
  writeln("Simulador da fita em loop de 300 leds do Neopixels.");
  writeln("O código para subir no arduino e rodar neste simulador");
  writeln("pode ser encontrado em:");
  writeln(
    "https://github.com/degroote22/loop-webusb-sim/blob/d0237eb1d09590c3a038aa723da472f2b663e46d/sketches_loop/test1/test1.ino",
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

export class RendererLoop implements Renderer {
  private camera: THREE.PerspectiveCamera;
  private controls: OrbitControls;
  private scene: THREE.Scene;
  private renderer: THREE.WebGLRenderer;
  private meshes: THREE.Mesh[] = [];
  private lit: { [key: number]: IColorObj } = {};
  constructor(writeln: (s: string) => void) {
    print_info(writeln);

    for (let i = 0; i < MAX; i++) {
      this.lit[i] = { r: 0, g: 0, b: 0 };
    }

    this.scene = new THREE.Scene();
    this.scene.background = new THREE.Color(0x444444);
    // scene.fog = new THREE.FogExp2(0xcccccc, 0.002);
    this.renderer = new THREE.WebGLRenderer({ antialias: true });
    this.renderer.setPixelRatio(window.devicePixelRatio);
    this.renderer.setSize(window.innerWidth, window.innerHeight);
    document.body.appendChild(this.renderer.domElement);
    this.camera = new THREE.PerspectiveCamera(
      60,
      window.innerWidth / window.innerHeight,
      1,
      1000,
    );
    this.camera.position.set(400, 200, 0);
    // controls
    this.controls = new OrbitControls(this.camera, this.renderer.domElement);
    this.controls.addEventListener("change", this.render); // call this only in static scenes (i.e., if there is no animation loop)
    this.controls.enableDamping = true; // an animation loop is required when either damping or auto-rotation are enabled
    this.controls.dampingFactor = 0.25;
    // this.controls.screenSpacePanning = false;
    this.controls.minDistance = 100;
    this.controls.maxDistance = 500;
    this.controls.maxPolarAngle = Math.PI / 2;

    // world
    this.createMeshes();

    // lights
    this.createLights();

    window.addEventListener("resize", this.onWindowResize, false);
    this.lightenMeshes();

    this.render(); // remove when using next line for animation loop (requestAnimationFrame)
    // this.animate();
  }

  public printDebugInfo = () => {
    var list = [];
    for (var i = 0; i < MAX; i++) {
      list.push(getHeight(i));
    }
    console.log(list);
    console.log(JSON.stringify(list));
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
      this.lightenMeshes();
      this.render();
      return true;
    }
    return false;
  };

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
    const c = parseInt(arr[2]);

    const maskb = 255;
    const maskg = maskb << 8;
    const maskr = maskb << 16;

    const b = maskb & c;
    const g = (maskg & c) >> 8;
    const r = (maskr & c) >> 16;
    this.light(i, r, g, b);
  };

  private reset = () => {
    for (let i = 0; i < MAX; i++) {
      this.lit[i] = { r: 0, g: 0, b: 0 };
    }
  };

  private animate = () => {
    // requestAnimationFrame(this.animate);

    this.controls.update(); // only required if controls.enableDamping = true, or if controls.autoRotate = true
    this.render();
  };

  private lightenMeshes = () => {
    Object.keys(this.lit).forEach(key => {
      const lit = this.lit[parseInt(key)];
      this.meshes[toMeshPos(parseInt(key) % MAX)].material = getMaterial(
        lit.r,
        lit.g,
        lit.b,
      );
    });
  };

  private render = () => {
    this.renderer.render(this.scene, this.camera);
  };

  private onWindowResize = () => {
    this.camera.aspect = window.innerWidth / window.innerHeight;
    this.camera.updateProjectionMatrix();
    this.renderer.setSize(window.innerWidth, window.innerHeight);
  };

  private createLights() {
    const light = new THREE.DirectionalLight(0xffffff);
    light.position.set(1, 1, 1);
    this.scene.add(light);
    const light2 = new THREE.DirectionalLight(0x002288);
    light2.position.set(-1, -1, -1);
    this.scene.add(light2);
    const light3 = new THREE.AmbientLight(0x222222);
    this.scene.add(light3);
  }

  private createMeshes() {
    const geometry = new THREE.SphereGeometry(2, 8, 8);
    const material = new THREE.MeshPhongMaterial({
      color: 0x000000,
      flatShading: true,
    });
    for (var i = 0; i < MAX; i++) {
      var mesh = new THREE.Mesh(geometry, material);
      var pos = getPosition(i);
      mesh.position.x = pos.x;
      mesh.position.y = pos.y;
      mesh.position.z = pos.z;
      mesh.updateMatrix();
      mesh.matrixAutoUpdate = false;
      this.scene.add(mesh);
      this.meshes.push(mesh);
    }
  }
}
