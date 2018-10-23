/*
 Code by Alchemsit_Arthur for a teensy 3.6 based quad. Can also be used on a hexapod.


 Quadraped Gait generator leg test
 uses globals for leg transitions. Also uses floats.
 X Axis is to the Front
 Y Axis is to the right
 Z Axis is to the top
 
 +X
 ^    Front
 |
 |   0  ^  1
 |   \\| |//
 |   //| |\\
 |   3  v  2
 o---------->+Y
 
 Leg
 
 body -> Coxa -> tibia -> femur -> femurRot -> tharsus
 
 Controls
   Takes custom packet protocol
   12 bytes
     1 - 0xFF
     2 - 0xAA
     3 - X, 3 to 253, centered at 128
     4 - Y, 3 to 253, centered at 128
     5 - Z, 3 to 253, centered at 128
     6 - Rx, 3 to 253, centered at 128
     7 - Ry, 3 to 253, centered at 128
     8 - Rz, 3 to 253, centered at 128
     9 - Buttons, each button is 1 bit, from lsb to msb; R1,R2,R3,L1,L2,L3,RT,LT
     10- Extended byte1,
     11- Extended byte2,
     12 - checksum, 255-((b3+b4+b5+b6+b7+b8+b9+b10+b11)%256)
   Extended Byte
     bits 4-7 are command, bits 0-3 is data (cccc dddd)
     Commands              Data                                   other
       0 -Reset           - NA                                    - reset any set extended byte values
       1 -Nothing         - NA                                    - no function, does not reset any values
       2 -Ext1 Follows    - Ext1
       3 -Ext2 Follows    - Ext2
       4 -Leg Place mode  - Selected leg, numbers are as above    - X->x,Y->y,Z->Z,Rz->WheelAzimuth,Rx->WheelElevation, Ry to wheel rotation
       5 -GaitMode        - Selected Gait                         - 0:MathGait,1:SwerveSteer,2:RuleGait
       6 -
       7 -
       8 -
       9 -
       10-
       11-
       12-
       13-
       14-
       15-
      Gait Select
        Data
          0 - Default Gait (periodic gait)
          1 - Crawl Gait (periodic gait)
          2 - Swerve Mode (Swerve Steering)
          3 - Rule based gait (Rule based)
          
 

 */










