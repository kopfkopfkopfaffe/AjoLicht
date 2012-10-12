import time
import serial
from numpy import *
import Image
ser = serial.Serial(port='/dev/ttyS0',baudrate=38400,parity=serial.PARITY_NONE,stopbits=serial.STOPBITS_ONE,bytesize=serial.EIGHTBITS)
color_map = zeros((256, 3), dtype=int16 )
char_map = (["","",""])

# ------------------------------------------------------------------------------------

def main():

    # setup the screen palette...
  set_palette()
  i=0 
  stime = .2
	
  while 1:
	i += 20
	if(i>255):i=0

        print("knight alive")

	time.sleep(stime)
	allblack()
	line0(i)
     	time.sleep(stime)
	allblack()
	line4(i)
     	time.sleep(stime)
	allblack()
	line2(i)
     	time.sleep(stime)
	allblack()
	line1(i)
     	time.sleep(stime)
	allblack()
	line3(i)

	


# ------------------------------------------------------------------------------------

# ------------------------------------------------------------------------------------
def line0(count):
  data0 = "A0%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s" % (char_map[0][count],char_map[1][count],char_map[2][count],char_map[0][count],char_map[1][count],char_map[2][count],char_map[0][count],char_map[1][count],char_map[2][count],char_map[0][count],char_map[1][count],char_map[2][count],char_map[0][count],char_map[1][count],char_map[2][count])
  ser.open()
  ser.write(data0)
  ser.close()

def line1(count):
  data0 = "A1%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s" % (char_map[0][count],char_map[1][count],char_map[2][count],char_map[0][count],char_map[1][count],char_map[2][count],char_map[0][count],char_map[1][count],char_map[2][count],char_map[0][count],char_map[1][count],char_map[2][count],char_map[0][count],char_map[1][count],char_map[2][count])
  ser.open()
  ser.write(data0)
  ser.close()

def line2(count):
  data0 = "A2%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s" % (char_map[0][count],char_map[1][count],char_map[2][count],char_map[0][count],char_map[1][count],char_map[2][count],char_map[0][count],char_map[1][count],char_map[2][count],char_map[0][count],char_map[1][count],char_map[2][count],char_map[0][count],char_map[1][count],char_map[2][count])
  ser.open()
  ser.write(data0)
  ser.close()

def line3(count):
  data0 = "A3%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s" % (char_map[0][count],char_map[1][count],char_map[2][count],char_map[0][count],char_map[1][count],char_map[2][count],char_map[0][count],char_map[1][count],char_map[2][count],char_map[0][count],char_map[1][count],char_map[2][count],char_map[0][count],char_map[1][count],char_map[2][count])
  ser.open()
  ser.write(data0)
  ser.close()

def line4(count):
  data0 = "A4%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s" % (char_map[0][count],char_map[1][count],char_map[2][count],char_map[0][count],char_map[1][count],char_map[2][count],char_map[0][count],char_map[1][count],char_map[2][count],char_map[0][count],char_map[1][count],char_map[2][count],char_map[0][count],char_map[1][count],char_map[2][count])
  ser.open()
  ser.write(data0)
  ser.close()

def allblack():
  black = chr(1)+chr(1)+chr(1)+chr(1)+chr(1)+chr(1)+chr(1)+chr(1)+chr(1)+chr(1)+chr(1)+chr(1)+chr(1)+chr(1)+chr(1)
  ser.open()
  ser.write("A0"+black)
  ser.write("A1"+black)
  ser.write("A2"+black)
  ser.write("A3"+black)
  ser.write("A4"+black)



# ------------------------------------------------------------------------------------
def make_cosine():
  "Knock up a little pre-calculated cosine lookup table..."
  i = 0
  for i in range (0,256):
      # Play with the values here for interesting results... I just made them up! :-)
      cos_tab[i]=60*(cos(i*PI/32))
  print (cos_tab)
# ------------------------------------------------------------------------------------

# ------------------------------------------------------------------------------------
def set_palette():
       
  i=0

  for i in range(0,64):
    color_map[i][0] = 255
    color_map[i][1] = i * 4
    color_map[i][2] = 255 - (i * 4)
    
    color_map[i+64][0] = 255 - (i * 4)     
    color_map[i+64][1] = 255 
    color_map[i+64][2] = (i * 4)
        
    color_map[i+128][0] = 0     
    color_map[i+128][1] = 255 - (i * 4)
    color_map[i+128][2] = 255
   
    color_map[i+192][0] = i * 4        
    color_map[i+192][1] = 0  
    color_map[i+192][2] = 255    

  #scale to 0..128
  for i in range(0,256):
    char_map[0]+=chr(int(color_map[i][0]/255.0*126))
    char_map[1]+=chr(int(color_map[i][1]/255.0*126))
    char_map[2]+=chr(int(color_map[i][2]/255.0*126))
# ------------------------------------------------------------------------------------

# ------------------------------------------------------------------------------------
def senddata():
  print ("sending data")
  count = 0
  count = 4
  data1 = "A4%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s" % (char_map[0][plasma_buffer[count][0]],char_map[1][plasma_buffer[count][0]],char_map[2][plasma_buffer[count][0]],char_map[0][plasma_buffer[count][1]],char_map[1][plasma_buffer[count][1]],char_map[2][plasma_buffer[count][1]],char_map[0][plasma_buffer[count][2]],char_map[1][plasma_buffer[count][2]],char_map[2][plasma_buffer[count][2]],char_map[0][plasma_buffer[count][3]],char_map[1][plasma_buffer[count][3]],char_map[2][plasma_buffer[count][3]],char_map[0][plasma_buffer[count][4]],char_map[1][plasma_buffer[count][4]],char_map[2][plasma_buffer[count][4]])
  count = 2
  data2 = "A2%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s" % (char_map[0][plasma_buffer[count][0]],char_map[1][plasma_buffer[count][0]],char_map[2][plasma_buffer[count][0]],char_map[0][plasma_buffer[count][1]],char_map[1][plasma_buffer[count][1]],char_map[2][plasma_buffer[count][1]],char_map[0][plasma_buffer[count][2]],char_map[1][plasma_buffer[count][2]],char_map[2][plasma_buffer[count][2]],char_map[0][plasma_buffer[count][3]],char_map[1][plasma_buffer[count][3]],char_map[2][plasma_buffer[count][3]],char_map[0][plasma_buffer[count][4]],char_map[1][plasma_buffer[count][4]],char_map[2][plasma_buffer[count][4]])
  count = 1
  data3 = "A1%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s" % (char_map[0][plasma_buffer[count][0]],char_map[1][plasma_buffer[count][0]],char_map[2][plasma_buffer[count][0]],char_map[0][plasma_buffer[count][1]],char_map[1][plasma_buffer[count][1]],char_map[2][plasma_buffer[count][1]],char_map[0][plasma_buffer[count][2]],char_map[1][plasma_buffer[count][2]],char_map[2][plasma_buffer[count][2]],char_map[0][plasma_buffer[count][3]],char_map[1][plasma_buffer[count][3]],char_map[2][plasma_buffer[count][3]],char_map[0][plasma_buffer[count][4]],char_map[1][plasma_buffer[count][4]],char_map[2][plasma_buffer[count][4]])
  count = 3
  data4 = "A3%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s" % (char_map[0][plasma_buffer[count][0]],char_map[1][plasma_buffer[count][0]],char_map[2][plasma_buffer[count][0]],char_map[0][plasma_buffer[count][1]],char_map[1][plasma_buffer[count][1]],char_map[2][plasma_buffer[count][1]],char_map[0][plasma_buffer[count][2]],char_map[1][plasma_buffer[count][2]],char_map[2][plasma_buffer[count][2]],char_map[0][plasma_buffer[count][3]],char_map[1][plasma_buffer[count][3]],char_map[2][plasma_buffer[count][3]],char_map[0][plasma_buffer[count][4]],char_map[1][plasma_buffer[count][4]],char_map[2][plasma_buffer[count][4]])

  ser.open()
  ser.write(data0)
  ser.write(data1)
  ser.write(data2)
  ser.write(data3)
  ser.write(data4)
 # print(data0)
 # print(data1)
 # print(data2)
 # print(data3)
 # print(data4)
  ser.close()
# ------------------------------------------------------------------------------------

if __name__ == '__main__': main()
