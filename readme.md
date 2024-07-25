# ESP-Miner

| Supported Targets | ESP32-S3 (BitDsk v5+) |
| ----------------- | --------------------- |

## Requires Python3.4 or later and pip

Install bitaxetool from pip. pip is included with Python 3.4 but if you need to install it check <https://pip.pypa.io/en/stable/installation/>

```
pip install --upgrade bitaxetool
```

## Hardware Required

This firmware is designed to run on a BitDsk v5+

If you do have a Bitdsk with no USB connectivity make sure to establish a serial connection with either a JTAG ESP-Prog device or a USB-to-UART bridge

## Preconfiguration


1. Download the esp-miner-factory-v2.0.3.bin file from the release tab.
   Click [here](https://github.com/bitdsk/ESP-Miner-for-Bitdsk/releases) for the release tab

2. Copy `config.cvs.example` to `config.cvs` and modify `hostname`, `wifissid`, `wifipass`, `stratumurl`, `stratumport`, `stratumuser`, `stratumpass`

3. If need StaticIp,modify `staticipopen` to 1, `staticip`, `staticnetmask`, `staticopengw`, `dnsmain`, `dnsbackup`, `dnsfallback`

The following are recommendations but it is necessary that you do have all values in your `config.cvs`file to flash properly.


- recomended values for the Bitdsk 5.0.x

  ```
  key,type,encoding,value
  main,namespace,,
  asicfrequency,data,u16,300
  ```

## Flash

The bitaxetool includes all necessary library for flashing the binary file to the Bitaxe Hardware.

The bitaxetool requires a config.cvs preloaded file and the appropiate firmware.bin file in it's executed directory.

3. Flash with the bitaxetool

```
bitaxetool --config ./config.cvs --firmware ./dskesp-factory-v0.0.1.bin
```

## API
Bitdsk provides an API to expose actions and information.

For more details take a look at `main/http_server/http_server.c`.

Things that can be done are:
  
  - Get System Info
  - Get Swarm Info
  - Update Swarm
  - Swarm Options
  - System Restart Action
  - Update System Settings Action
  - System Options
  - Update OTA Firmware
  - Update OTA WWW
  - WebSocket

Some API examples in curl:
  ```bash
  # Get system information
  curl http://YOUR-BITAXE-IP/api/system/info
  ```
  ```bash
  # Get swarm information
  curl http://YOUR-BITAXE-IP/api/swarm/info
  ```
  ```bash
  # System restart action
  curl -X POST http://YOUR-BITAXE-IP/api/system/restart
  ```