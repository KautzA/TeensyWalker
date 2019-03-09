##0 - Right/Left
##1 - Back/Forward
##2 - Down/Up
##3 - Rotation around z
##4 - Rotation around x
##5 - Rotation around y


import pygame
import serial
import time
import sys
import math

ser = serial.Serial(
    port='COM12',
    baudrate=38400,
    stopbits=serial.STOPBITS_ONE,
    parity=serial.PARITY_NONE,
    bytesize=serial.EIGHTBITS
    
    #write_timeout=2
)


def text_render(screen,text,pos,color = (0,0,0),size=50):
    try:
        text = str(text)
        font = pygame.font.SysFont('Comic Sans MS',size)
        text = font.render(text, True, color)
        screen.blit(text,pos)
    except Exception, e:
        print 'Font Error'
        raise e


def translate(value, leftMin, leftMax, rightMin, rightMax):
    # Figure out how 'wide' each range is
    leftSpan = leftMax - leftMin
    rightSpan = rightMax - rightMin

    # Convert the left range into a 0-1 range (float)
    valueScaled = float(value - leftMin) / float(leftSpan)

    # Convert the 0-1 range into a value in the right range.
    return rightMin + (valueScaled * rightSpan)




print "Pygame Loaded"
from pygame.locals import *
ser.flushOutput()


pygame.init()
DisplayWidth = 800
DisplayHeight = 400
DisplaySurface = pygame.display.set_mode((DisplayWidth,DisplayHeight))

joystick_count = pygame.joystick.get_count()
print ("There is ", joystick_count, "joystick/s")
if joystick_count == 0:
    print ("Error, I did not find any joysticks")
else:
    SpaceMouse = pygame.joystick.Joystick(1)
    SpaceMouse.init()
    print (SpaceMouse.get_name())
    print (SpaceMouse.get_numaxes())
    print (SpaceMouse.get_numbuttons())

    NumAxes = SpaceMouse.get_numaxes()
    NumButtons = SpaceMouse.get_numbuttons()
print("Started")




time.sleep(.25)
print("LoopStart");
while True:
    ser.flushInput #discard anything in the serial buffer
    #while (ser.inWaiting > 0):
    #    print ser.read()
    
    for event in pygame.event.get():
        if event.type == QUIT:
            print("quit")
            ser.close()
            pygame.quit()
            sys.exit()
    #get inputs
    AxisX = SpaceMouse.get_axis(0)
    AxisY = -SpaceMouse.get_axis(1)
    AxisZ = -SpaceMouse.get_axis(2)
    AxisRx = -SpaceMouse.get_axis(5)
    AxisRy = SpaceMouse.get_axis(4)
    AxisRz = -SpaceMouse.get_axis(3)

    AxisList = [AxisX, AxisY, AxisZ, AxisRx, AxisRy, AxisRz]

    #get keyboard keys
    keydown=pygame.key.get_pressed()
    Button1 = keydown[pygame.K_1]
    Button2 = keydown[pygame.K_2]
    Button3 = keydown[pygame.K_3]
    Button4 = keydown[pygame.K_4]
    Button5 = keydown[pygame.K_5]
    Button6 = keydown[pygame.K_6]
    Button7 = keydown[pygame.K_7]#SpaceMouse.get_button(1)
    Button8 = keydown[pygame.K_8]

    #scale inputs
    AxisX = int(translate(AxisX, -1, 1, 3, 253)+.5)
    AxisY = int(translate(AxisY, -1, 1, 3, 253)+.5)
    AxisZ = int(translate(AxisZ, -1, 1, 3, 253)+.5)
    AxisRx = int(translate(AxisRx, -1, 1, 3, 253)+.5)
    AxisRy = int(translate(AxisRy, -1, 1, 3, 253)+.5)
    AxisRz = int(translate(AxisRz, -1, 1, 3, 253)+.5)

    OutByte1 = int(255) #255
    OutByte2 = int(170) #255
    OutByte3 = AxisX    #X axis
    OutByte4 = AxisY    #Y axis
    OutByte5 = AxisZ    #Z axis
    OutByte6 = AxisRx   #Rx
    OutByte7 = AxisRy   #Ry
    OutByte8 = AxisRz   #Rz
    OutByte9 = ((Button1*1)+(Button2*2)+(Button3*4)+(Button4*8)+(Button5*16)+(Button6*32)+(Button7*64)+(Button8*128))       #Buttons
    OutByte10 = (1)*16+(0)        #UserCommand(high nibble)+ UserData1 (low nibble)
    OutByte11 = 1       #UserData2
    OutByte12 = int(255-(int((OutByte3+OutByte4+OutByte5+OutByte6+OutByte7+OutByte8+OutByte9+OutByte10+OutByte11)%256)))        #checksum


    #set extended byte commands
    #reset
    if keydown[pygame.K_r]:
        OutByte10 = (0)*16+(0)
    #gaits
    if keydown[pygame.K_9]:
        OutByte10 = (5)*16+(0) #GaitSelect/2x2

    if keydown[pygame.K_0]:
        OutByte10 = (5)*16+(1) #GaitSelect/crawl
    
    
    ser.write(chr(OutByte1))
    #print"1Send "
    #print(OutByte1)
    ser.write(chr(OutByte2))
    #print"2Send"
    #print(OutByte2)
    ser.write(chr(OutByte3))
    #print"3Send"
    #print(OutByte3)
    ser.write(chr(OutByte4))
    #print"4Send"
    #print(OutByte4)
    ser.write(chr(OutByte5))
    #print"5Send"
    #print(OutByte5)
    ser.write(chr(OutByte6))
    #print"6Send"
    #print(OutByte6)
    ser.write(chr(OutByte7))
    #print"7Send"
    #print(OutByte7)
    ser.write(chr(OutByte8))
    #print"8Send"
    #print(OutByte8)
    ser.write(chr(OutByte9))
    #print"9Send"
    #print(OutByte9)
    ser.write(chr(OutByte10))
    #print"10Send"
    #print(OutByte10)
    ser.write(chr(OutByte11))
    #print"11Send"
    #print(OutByte11)
    ser.write(chr(OutByte12))
    #print"12Send"
    #print(OutByte12)
    ser.flush()
    time.sleep(.1)

    
    DisplaySurface.fill((0,0,0))
    pygame.draw.circle(DisplaySurface,(255,255,255),(DisplayWidth-15,15),10,0)
    
    for Axis in range(0,NumAxes):
        print Axis,", ", SpaceMouse.get_axis(Axis)
        AxisDistance = (DisplayWidth)/NumAxes
        pygame.draw.circle(DisplaySurface,(255,255,255),(int(25+((Axis+.5)*AxisDistance)),DisplayHeight/4 - int(AxisList[Axis]*(((DisplayHeight/2)-50)/2))),5,0)
    print""
    
    #add text for Commander Packets
    CommandPackets = str(str(OutByte1).zfill(3)+" "+str(OutByte2).zfill(3)+" "+str(OutByte3).zfill(3)+" "+str(OutByte4).zfill(3)+" "+str(OutByte5).zfill(3)+" "+str(OutByte6).zfill(3)+" "+str(OutByte7).zfill(3)+" "+str(OutByte8).zfill(3)+" "+str(OutByte9).zfill(3)+" "+str(OutByte10).zfill(3)+" "+str(OutByte11).zfill(3)+" "+str(OutByte12).zfill(3))
    text_render(DisplaySurface,CommandPackets,(15,250),(0,255,0),25)
    HexPackets = str("0x%x" % OutByte1 +"   "+ "0x%x" % OutByte2+"   "+ "0x%x" % OutByte3+"   "+ "0x%x" % OutByte4+"   "+ "0x%x" % OutByte5+"   "+ "0x%x" % OutByte6+"   "+ "0x%x" % OutByte7+"   "+ "0x%x" % OutByte8+"   "+ "0x%x" % OutByte9+"   "+ "0x%x" % OutByte10+"   "+ "0x%x" % OutByte11+"   "+ "0x%x" % OutByte12)
    text_render(DisplaySurface,HexPackets,(15,275),(0,255,0),15)

    pygame.draw.circle(DisplaySurface,(0,255,255*Button1),(300,600),5,0)
    
    pygame.display.update()
    time.sleep(.1)

            
