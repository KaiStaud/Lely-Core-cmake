**FRAM MEMORY MAP**
|Address|Index:Subindex|Name|
|----|----|----|
| 0x000000-0x000003 | RESERVED (MAGIC WORD)                        |
| 0x000004-0x000007 | 0x1000:0x00 | Device type                    |
| 0x000008-0x000008 | 0x1001:0x00 | Error register                 |
| 0x000009-0x00000B | ALIGNMENT PADDING                    |
| 0x00000C-0x00000F | 0x1012:0x00 | COB-ID time stamp object       |
| 0x000010-0x000010 | 0x1016:0x00 | Consumer heartbeat time        |
| 0x000011-0x000013 | ALIGNMENT PADDING                    |
| 0x000014-0x000017 | 0x1016:0x01 | Consumer heartbeat time1       |
| 0x000018-0x000019 | 0x1017:0x00 | Producer heartbeat time        |
| 0x00001A-0x00001B | ALIGNMENT PADDING                    |
| 0x00001C-0x00001C | 0x1018:0x00 | Identity object                |
| 0x00001D-0x00001F | ALIGNMENT PADDING                    |
| 0x000020-0x000023 | 0x1018:0x01 | Vendor-ID                      |
| 0x000024-0x000027 | 0x1018:0x02 | Product code                   |
| 0x000028-0x00002B | 0x1018:0x03 | Revision number                |
| 0x00002C-0x00002F | 0x1018:0x04 | Serial number                  |
| 0x000030-0x000033 | 0x1f80:0x00 | NMT startup                    |
| 0x000034-0x000034 | 0x1400:0x00 | RPDO communication parameter   |
| 0x000035-0x000037 | ALIGNMENT PADDING                    |
| 0x000038-0x00003B | 0x1400:0x01 | COB-ID used by RPDO            |
| 0x00003C-0x00003C | 0x1400:0x02 | transmission type              |
| 0x00003D-0x00003F | ALIGNMENT PADDING                    |
| 0x000040-0x000041 | 0x1400:0x03 | inhibit time                   |
| 0x000042-0x000043 | ALIGNMENT PADDING                    |
| 0x000044-0x000044 | 0x1400:0x04 | compatibility entry            |
| 0x000045-0x000047 | ALIGNMENT PADDING                    |
| 0x000048-0x000049 | 0x1400:0x05 | event-timer                    |
| 0x00004A-0x00004B | ALIGNMENT PADDING                    |
| 0x00004C-0x00004C | 0x1600:0x00 | RPDO mapping parameter         |
| 0x00004D-0x00004F | ALIGNMENT PADDING                    |
| 0x000050-0x000053 | 0x1600:0x01 | RPDO mapping parameter1        |
| 0x000054-0x000054 | 0x1800:0x00 | TPDO communication parameter   |
| 0x000055-0x000057 | ALIGNMENT PADDING                    |
| 0x000058-0x00005B | 0x1800:0x01 | COB-ID used by TPDO            |
| 0x00005C-0x00005C | 0x1800:0x02 | transmission type              |
| 0x00005D-0x00005F | ALIGNMENT PADDING                    |
| 0x000060-0x000061 | 0x1800:0x03 | inhibit time                   |
| 0x000062-0x000063 | ALIGNMENT PADDING                    |
| 0x000064-0x000064 | 0x1800:0x04 | reserved                       |
| 0x000065-0x000067 | ALIGNMENT PADDING                    |
| 0x000068-0x000069 | 0x1800:0x05 | event timer                    |
| 0x00006A-0x00006B | ALIGNMENT PADDING                    |
| 0x00006C-0x00006C | 0x1800:0x06 | SYNC start value               |
| 0x00006D-0x00006F | ALIGNMENT PADDING                    |
| 0x000070-0x000070 | 0x1a00:0x00 | TPDO mapping parameter         |
| 0x000071-0x000073 | ALIGNMENT PADDING                    |
| 0x000074-0x000077 | 0x1a00:0x01 | TPDO mapping parameter1        |
| 0x000078-0x00007B | 0x2000:0x00 | Object with custom SDO downloa |
| 0x00007C-0x00007F | 0x2001:0x00 | Object with custom SDO upload  |
| 0x000080-0x000083 | 0x6040:0x00 | Controlword                    |
| 0x000084-0x000087 | 0x6041:0x00 | Statusword                     |
| 0x000088-0x00008B | 0x60FD:0x00 | Digital Inputs                 |
| 0x00008C-0x00008F | 0x6098:0x00 | Homing Method                  |
| 0x000090-0x000093 | 0x6099:0x00 | Homing Speed                   |
| 0x000094-0x000097 | 0x609A:0x00 | Homing Acceleration            |
| 0x000098-0x00009B | 0x6060:0x00 | Modes of Operation             |
| 0x00009C-0x00009F | 0x607A:0x00 | Target Position                |

```
FRAM USAGE STATISTICS
---------------------------------------------
Total FRAM         : 524288 Bytes (512 KB)
Reserved (magic)   : 4 Bytes
Data used          : 110 Bytes
Alignment padding  : 46 Bytes
---------------------------------------------
Total used         : 160 Bytes (0.031 %)
Free               : 524128 Bytes (99.969 %)
---------------------------------------------
[----------------------------------------]

Persistent entries: 39
```