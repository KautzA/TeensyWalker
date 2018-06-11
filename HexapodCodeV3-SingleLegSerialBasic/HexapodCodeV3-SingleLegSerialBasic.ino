/*
 Code by Alchemsit_Arthur for a teensy 3.6 based quad.


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
   Takes Commander packet protocol
   8 bytes
     1 - 0xFF
     2 - Look Vert, 3 to 235, centerd at 128
     3 - Look Hori, 3 to 235, centerd at 128
     4 - Walk Vert, 3 to 235, centerd at 128
     5 - Walk Hori, 3 to 235, centerd at 128
     6 - Buttons, each button is 1 bit, from lsb to msb; R1,R2,R3,L1,L2,L3,RT,LT
     7 - Extended bit,
     8 - checksum, 255-((b2+b3+b4+b5+b6+b7)%256)
   Extended Byte
     bits 4-7 are command, bits 0-3 is data (cccc dddd)
     Commands              Data                                   other
       0 -Reset           - NA                                    - reset any set extended byte values
       1 -Nothing         - NA                                    - no function, does not reset any values
       2 -Ext1 Follows    - Ext1
       3 -Ext2 Follows    - Ext2
       4 -Leg Place mode  - Selected leg, numbers are as above    - LV->x,LH->y,WV->Z,WH->WheelAzimuth,Buttons->WheelElevation, Ext1 to wheel rotation
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
          1 - Swerve Mode (Swerve Steering)
          2 - Rule based gait (Rule based)
          
 
 This code runs on commander packets and moves based on that.
 
 example packets
 (0xff, 0x80, 0x80, 0x80, 0x80, 0x0, 0x0, 0xff),(255, 128, 128, 128, 128, 0, 0, 255), This is standard, all joysticks centered and no buttons or extended byte.
 (0xff, 0xeb, 0x80, 0x80, 0x80, 0x0, 0x0, 0x94),(255, 235, 128, 128, 128, 0 ,0, 148), This is left stick up, all other values centered or zero.
 
 
 */










