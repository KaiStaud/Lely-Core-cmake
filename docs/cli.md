Available callbacks:
- write_object [Index] [SubIndex] [value]
- move_to [position] : Sets a new target position. Automatically selects profile position mode.
- set_rpm [frequency]: Sets new target frequency. Selects profile velocity mode.

Write the following sequency to switch the device to operation enabled without CAN traffic:
(1) write_object 0x6040 0 0x6
(2) write_object 0x6040 0 0x7
(3) write_object 0x6040 0 0xF

Set new target position
(4) move_to 15000