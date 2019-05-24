import { MAX } from "./constants";
import * as THREE from "three";
// import "three/examples/js/controls/OrbitControls";
import "three/examples/js/WebGL";
import { getPosition, toMeshPos, getHeight } from "./helpers";
import { getMaterial } from "./material_cache";
import { IColorObj, Renderer } from "./interfaces";
import { OrbitControls } from "three-orbitcontrols-ts";

// TODO Ver se n precisa dessa checagem
// if (WEBGL.isWebGLAvailable() === false) {
//   document.body.appendChild(WEBGL.getWebGLErrorMessage());
// }
let lock = 0;
export class RendererLoop implements Renderer {
  private camera: THREE.PerspectiveCamera;
  private controls: OrbitControls;
  private scene: THREE.Scene;
  private renderer: THREE.WebGLRenderer;
  private meshes: THREE.Mesh[] = [];
  private lit: { [key: number]: IColorObj } = {};

  constructor() {
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
