Available callbacks:
- write_object [Index] [SubIndex] [value]
- move_to [position] : Sets a new target position. Automatically selects profile position mode.
- set_rpm [frequency]: Sets new target frequency. Selects profile velocity mode.

Write the following sequency to switch the device to operation enabled without CAN traffic:
(1) write_object 0x6040 0 0x6
(2) write_object 0x6040 0 0x7
(3) write_object 0x6040 0 0xF

Set new target position.

t_total   = 9 s
t_a = t_d =  3 s
t_const = 3 s
x_a + x_d = 12000 steps
x_const   = 18000 steps

Homing: Write 6 to 0x6040.
The drive will move in the specified direction until the endstop was triggered.
TODO: Use the ramp-up from trapezoidal profile to move. Don't start with full speed.

feed = 150 mm
150 Umdrehungen
200 Steps * 150 für kompletten Weg = 30 000 Steps

30 000 Schritte in 9 Sekunden
Floating-Point Implementierung:
Nach 3000 ms soll v_max = 6 -> a_max = 6/3000 =  0.002 Steps / ms²

Rest mit konstanter Geschwindikeit: 3000 ms mit  v_max=6
--> In 3000 ms werden 18000 Steps verfahren!