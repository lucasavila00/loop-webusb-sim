export class Serial {
  public static getPorts = function() {
    return navigator.usb.getDevices().then(devices => {
      return devices.map(device => new Port(device));
    });
  };

  public static requestPort = function() {
    const filters = [
      { vendorId: 0x2341, productId: 0x8036 },
      { vendorId: 0x2341, productId: 0x8037 },
      { vendorId: 0x2341, productId: 0x804d },
      { vendorId: 0x2341, productId: 0x804e },
      { vendorId: 0x2341, productId: 0x804f },
      { vendorId: 0x2341, productId: 0x8050 },
    ];
    return navigator.usb
      .requestDevice({ filters: filters })
      .then(device => new Port(device));
  };
}

type OnReceiveCb = (data?: DataView) => void;
type OnReceiveErrorCb = (error: any) => void;

export class Port {
  private interfaceNumber_: number;
  private endpointIn_: number;
  private endpointOut_: number;
  public device_: USBDevice;
  private onReceive?: OnReceiveCb;
  private onReceiveError?: OnReceiveErrorCb;

  constructor(device: USBDevice) {
    this.device_ = device;
    this.interfaceNumber_ = 2; // original interface number of WebUSB Arduino demo
    this.endpointIn_ = 5; // original in endpoint ID of WebUSB Arduino demo
    this.endpointOut_ = 4; // original out endpoint ID of WebUSB Arduino demo
  }

  public setCallbacks = (
    onReceive: OnReceiveCb,
    onReceiveError: OnReceiveErrorCb,
  ) => {
    this.onReceive = onReceive;
    this.onReceiveError = onReceiveError;
  };

  public connect = () => {
    let readLoop = () => {
      this.device_.transferIn(this.endpointIn_, 64).then(
        result => {
          if (!this.onReceive) {
            throw Error("No onReceive callback registered");
          }
          this.onReceive(result.data);
          readLoop();
        },
        error => {
          if (!this.onReceiveError) {
            throw Error("No onReceiveError callback registered");
          }
          this.onReceiveError(error);
        },
      );
    };

    return this.device_
      .open()
      .then(() => {
        if (this.device_.configuration === null) {
          return this.device_.selectConfiguration(1);
        }
      })
      .then(() => {
        if (!this.device_.configuration) {
          throw Error("Device has no configuration object");
          // TODO: O que esse erro significa? Como resolver?
        }
        var configurationInterfaces = this.device_.configuration.interfaces;
        configurationInterfaces.forEach(element => {
          element.alternates.forEach(elementalt => {
            if (elementalt.interfaceClass == 0xff) {
              this.interfaceNumber_ = element.interfaceNumber;
              elementalt.endpoints.forEach(elementendpoint => {
                if (elementendpoint.direction == "out") {
                  this.endpointOut_ = elementendpoint.endpointNumber;
                }
                if (elementendpoint.direction == "in") {
                  this.endpointIn_ = elementendpoint.endpointNumber;
                }
              });
            }
          });
        });
      })
      .then(() => this.device_.claimInterface(this.interfaceNumber_))
      .then(() =>
        this.device_.selectAlternateInterface(this.interfaceNumber_, 0),
      )
      .then(() =>
        this.device_.controlTransferOut({
          requestType: "class",
          recipient: "interface",
          request: 0x22,
          value: 0x01,
          index: this.interfaceNumber_,
        }),
      )
      .then(() => {
        readLoop();
      });
  };

  disconnect = () => {
    return this.device_
      .controlTransferOut({
        requestType: "class",
        recipient: "interface",
        request: 0x22,
        value: 0x00,
        index: this.interfaceNumber_,
      })
      .then(() => this.device_.close());
  };

  send = (data: BufferSource) => {
    return this.device_.transferOut(this.endpointOut_, data);
  };
}
