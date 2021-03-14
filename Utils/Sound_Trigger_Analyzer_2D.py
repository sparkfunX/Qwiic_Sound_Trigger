# Sound Trigger Analyzer 2D

# Written by: Paul Clark
# Last update: March 14th, 2021

# Reads three UBX files logged by the Qwiic Sound Trigger Example2,
# extracts the TIM_TM2 of the sound events and calculates the location
#
# python Sound_Trigger_Analyzer_2D.py TIM_TM2_A.ubx TIM_TM2_B.ubx TIM_TM2_C.ubx 8.0 6.0 10.0 20.0
#
# Replace TIM_TM2_A.ubx with the name of the file from the first system as necessary
# Replace TIM_TM2_B.ubx with the name of the file from the second system as necessary
# Replace TIM_TM2_C.ubx with the name of the file from the third system as necessary
# Replace the 8.0 with the distance between A and B in metres
# Replace the 6.0 with the distance between A and C in metres
# Replace the 10.0 with the distance between B and C in metres
# The 20.0 is optional. It is the temperature in Celcius (Centigrade). Change this to your actual temperature for added accuracy


# SparkFun code, firmware, and software is released under the MIT License (http://opensource.org/licenses/MIT)
#
# The MIT License (MIT)
#
# Copyright (c) 2020 SparkFun Electronics
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

import sys
import os

diag = True # Print diagnostics

# Add byte to checksums sum1 and sum2
def csum(byte, sum1, sum2):
    sum1 = sum1 + byte
    sum2 = sum2 + sum1
    sum1 = sum1 & 0xFF
    sum2 = sum2 & 0xFF
    return sum1,sum2

print('Sound Trigger Analyzer 2D')
print()

if len(sys.argv) < 7:
    print('ERROR: insufficient arguments!')
    print('')
    print('python Sound_Trigger_Analyzer_2D.py TIM_TM2_A.ubx TIM_TM2_B.ubx TIM_TM2_C.ubx 8.0 6.0 10.0 20.0')
    print('')
    print('TIM_TM2_A.ubx is the name of the file from the first system (A)')
    print('TIM_TM2_B.ubx is the name of the file from the second system (B)')
    print('TIM_TM2_C.ubx is the name of the file from the third system (C)')
    print('Replace the 8.0 with the distance between A and B in metres')
    print('Replace the 6.0 with the distance between A and C in metres')
    print('Replace the 10.0 with the distance between B and C in metres')
    print('The 20.0 is optional. It is the temperature in Celcius (Centigrade)')
    raise Exception('Insufficient arguments!')

filenameA = sys.argv[1]
filenameB = sys.argv[2]
filenameC = sys.argv[3]
AB = float(sys.argv[4])
AC = float(sys.argv[5])
BC = float(sys.argv[6])
temperature = 20.0
if len(sys.argv) >= 8:
    temperature = float(sys.argv[7])

def processUBXfile(filename):

    # Sync 'state machine'
    looking_for_B5          = 0 # Looking for a UBX 0xB5 header byte
    looking_for_62          = 1 # Looking for a UBX 0x62 header byte
    looking_for_class       = 2 # Looking for UBX class byte
    looking_for_ID          = 3 # Looking for UBX ID byte
    looking_for_length_LSB  = 4 # Looking for UBX length bytes
    looking_for_length_MSB  = 5
    processing_payload      = 6 # Processing the payload. Keep going until length bytes have been processed
    looking_for_checksum_A  = 7 # Looking for UBX checksum bytes
    looking_for_checksum_B  = 8
    sync_lost               = 9 # Go into this state if sync is lost (bad checksum etc.)

    ubx_state = sync_lost # Initialize the state machine

    # Storage for UBX messages
    ubx_length = 0
    ubx_class = 0
    ubx_ID = 0
    ubx_checksum_A = 0
    ubx_checksum_B = 0
    ubx_expected_checksum_A = 0
    ubx_expected_checksum_B = 0

    # TIM_TM2 specifics
    risingEdgeCount = 0
    times = []
    risingEdgeCounts = []

    print('Processing',filename)
    print()
    filesize = os.path.getsize(filename) # Record the file size

    # Try to open file for reading
    try:
        fi = open(filename,"rb")
    except:
        raise Exception('Invalid file!')

    processed = -1 # The number of bytes processed
    keepGoing = True

    while keepGoing:

        # Read one byte from the file
        fileBytes = fi.read(1)
        if (len(fileBytes) == 0):
            print('ERROR: Read zero bytes from file 1. End of file?! Or zero file size?!')
            raise Exception('End of file 1?! Or zero file size?!')
        c = fileBytes[0]

        processed = processed + 1 # Keep a record of how many bytes have been read and processed

        # Process data bytes according to ubx_state
        # UBX messages:
        # Sync Char 1: 0xB5
        # Sync Char 2: 0x62
        # Class byte
        # ID byte
        # Length: two bytes, little endian
        # Payload: length bytes
        # Checksum: two bytes

        # TIM_TM2 is class 0x0d ID 0x03

        if (ubx_state == looking_for_B5) or (ubx_state == sync_lost):
            if (c == 0xB5): # Have we found Sync Char 1 (0xB5) if we were expecting one?
                if (ubx_state == sync_lost):
                    print("UBX Sync Char 1 (0xB5) found at byte "+str(processed)+". Checking for Sync Char 2")
                ubx_state = looking_for_62 # Now look for Sync Char 2 (0x62)
            else:
                #print("Was expecting Sync Char 0xB5 but did not receive one!")
                ubx_state = sync_lost
        elif (ubx_state == looking_for_62):
            if (c == 0x62): # Have we found Sync Char 2 (0x62) when we were expecting one?
                ubx_expected_checksum_A = 0 # Reset the expected checksum
                ubx_expected_checksum_B = 0
                ubx_state = looking_for_class # Now look for Class byte
            else:
                print("Panic!! Was expecting Sync Char 2 (0x62) but did not receive one!")
                ubx_state = sync_lost
        elif (ubx_state == looking_for_class):
            ubx_class = c
            ubx_expected_checksum_A = ubx_expected_checksum_A + c # Update the expected checksum
            ubx_expected_checksum_B = ubx_expected_checksum_B + ubx_expected_checksum_A
            ubx_state = looking_for_ID # Now look for ID byte
        elif (ubx_state == looking_for_ID):
            ubx_ID = c
            ubx_expected_checksum_A = ubx_expected_checksum_A + c # Update the expected checksum
            ubx_expected_checksum_B = ubx_expected_checksum_B + ubx_expected_checksum_A
            message_type = '0x%02X 0x%02X'%(ubx_class,ubx_ID) # Record the message type
            ubx_state = looking_for_length_LSB # Now look for length LSB
        elif (ubx_state == looking_for_length_LSB):
            ubx_length = c # Store the length LSB
            ubx_expected_checksum_A = ubx_expected_checksum_A + c # Update the expected checksum
            ubx_expected_checksum_B = ubx_expected_checksum_B + ubx_expected_checksum_A
            ubx_state = looking_for_length_MSB # Now look for length MSB
        elif (ubx_state == looking_for_length_MSB):
            ubx_length = ubx_length + (c * 256) # Add the length MSB
            ubx_expected_checksum_A = ubx_expected_checksum_A + c # Update the expected checksum
            ubx_expected_checksum_B = ubx_expected_checksum_B + ubx_expected_checksum_A
            ubx_state = processing_payload # Now look for payload bytes (length: ubx_length)
        elif (ubx_state == processing_payload):
            if (ubx_class == 0x0d) and (ubx_ID == 0x03): # Is this a TIM_TM2 message?
                if (ubx_length == 28 - 1): # Is this byte 1?
                    if ((c & 0x80) == 0x80): # Is the newRisingEdge bit set?
                        newRisingEdge = True
                    else:
                        newRisingEdge = False
                if (ubx_length == 28 - 2): # Is this byte 2?
                    risingEdgeCount = c # Extract the count LSB
                if (ubx_length == 28 - 3): # Is this byte 3?
                    risingEdgeCount += c * 0x100 # Extract and add the count MSB
                if (ubx_length == 28 - 8): # Is this byte 8?
                    towMsR = c # Extract the Time Of Week of the Rising edge in milliseconds
                if (ubx_length == 28 - 9): # Is this byte 9?
                    towMsR += c * 0x100 # Extract the Time Of Week of the Rising edge in milliseconds
                if (ubx_length == 28 - 10): # Is this byte 10?
                    towMsR += c * 0x10000 # Extract the Time Of Week of the Rising edge in milliseconds
                if (ubx_length == 28 - 11): # Is this byte 11?
                    towMsR += c * 0x1000000 # Extract the Time Of Week of the Rising edge in milliseconds
                    towMsR *= 1000000 # Convert to nanoseconds
                if (ubx_length == 28 - 12): # Is this byte 12?
                    towMsR += c # Extract the Time Of Week of the Rising edge in nanoseconds
                if (ubx_length == 28 - 13): # Is this byte 13?
                    towMsR += c * 0x100 # Extract the Time Of Week of the Rising edge in nanoseconds
                if (ubx_length == 28 - 14): # Is this byte 14?
                    towMsR += c * 0x10000 # Extract the Time Of Week of the Rising edge in nanoseconds
                if (ubx_length == 28 - 15): # Is this byte 15?
                    towMsR += c * 0x1000000 # Extract the Time Of Week of the Rising edge in nanoseconds
            ubx_length = ubx_length - 1 # Decrement length by one
            ubx_expected_checksum_A = ubx_expected_checksum_A + c # Update the expected checksum
            ubx_expected_checksum_B = ubx_expected_checksum_B + ubx_expected_checksum_A
            if (ubx_length == 0):
                ubx_expected_checksum_A = ubx_expected_checksum_A & 0xff # Limit checksums to 8-bits
                ubx_expected_checksum_B = ubx_expected_checksum_B & 0xff
                ubx_state = looking_for_checksum_A # If we have received length payload bytes, look for checksum bytes
        elif (ubx_state == looking_for_checksum_A):
            ubx_checksum_A = c
            ubx_state = looking_for_checksum_B
        elif (ubx_state == looking_for_checksum_B):
            ubx_checksum_B = c
            ubx_state = looking_for_B5 # All bytes received so go back to looking for a new Sync Char 1 unless there is a checksum error
            if ((ubx_expected_checksum_A != ubx_checksum_A) or (ubx_expected_checksum_B != ubx_checksum_B)):
                print("Panic!! UBX checksum error!")
                if diag:
                    print('ubx_expected_checksum_A',ubx_expected_checksum_A,'ubx_expected_checksum_B',ubx_expected_checksum_B)
                ubx_state = sync_lost
            else:
                if (newRisingEdge == True):
                    times.append(towMsR)
                    risingEdgeCounts.append(risingEdgeCount)

        # Check if the end of the file has been reached
        if (processed >= filesize - 1): keepGoing = False

    print()
    fi.close() # Close the file
    return (times, risingEdgeCounts)

timesA, countsA = processUBXfile(filenameA)
timesB, countsB = processUBXfile(filenameB)
timesC, countsC = processUBXfile(filenameC)

print('Found',len(timesA),'rising edge times in file A')
print('Found',len(timesB),'rising edge times in file B')
print('Found',len(timesC),'rising edge times in file C')
print()

speed_of_sound = 331.3 + (0.606 * temperature) # Calculate the speed of sound

speed_of_sound = 1.0 # Handy for testing with fake TIM_TM2 data

maxTimeAB = AB / speed_of_sound # Calculate what the maximum time difference can be between A and B
maxTimeAC = AC / speed_of_sound # Calculate what the maximum time difference can be between A and C
maxTimeBC = BC / speed_of_sound # Calculate what the maximum time difference can be between B and C

# 1: AC**2 = CX**2 + CY**2
# 2: BC**2 = (AB - CX)**2 + CY**2
# Rearrange 2: CY**2 = BC**2 - (AB - CX)**2
# Substitute into 1: AC**2 = CX**2 + BC**2 - (AB - CX)**2
# Multiply the square: AC**2 = CX**2 + BC**2 - AB**2 + 2.CX - CX**2
# CX = (AC**2 - BC**2 

CX = (AC**2 - BC**2 + AB**2) / (2 * AB) # Calculate the X coordinate of C
CY = (AC**2 - CX**2)**0.5 # Calculate the Y coordinate of C
if diag: print('CX =',CX,' CY =',CY)

# Check that the X coordinate of C lies between A and B
if (CX < 0) or (CX > AB):
    print('Please rename your points. The X coordinate of C must lie between A and B')
    raise Exception('Invalid point order!')

for timeA in timesA: # Step through each time in file A
    for timeB in timesB: # Compare to each time in file B
        for timeC in timesC: # Compare to each time in file C
            timeA *= 1e-9 # convert nanoseconds to seconds
            timeB *= 1e-9 # convert nanoseconds to seconds
            timeC *= 1e-9 # convert nanoseconds to seconds
            if diag: print ('timeA:',timeA,' timeB:',timeB,' timeC:',timeC)
            if (abs(timeA - timeB) <= maxTimeAB) \
               and (abs(timeA - timeC) <= maxTimeAC) \
               and (abs(timeB - timeC) <= maxTimeBC): # Is the difference less than the maximum

                TdiffB = timeB - timeA # Can be + or -
                TdiffC = timeC - timeA # Can be + or -
                if diag: print('TdiffB =',TdiffB,' TdiffC =',TdiffC)

                DdiffB = TdiffB * speed_of_sound # Distance difference between B and A
                DdiffC = TdiffC * speed_of_sound # Distance difference between C and A
                if diag: print('DdiffB =',DdiffB,' DdiffC =',DdiffC)

                SXb = (AB**2 - DdiffB**2) / (2 * AB) # Calculate the b part of x = a.D + b
                SXa = (0 - (2 * DdiffB)) / (2 * AB) # Calculate the a part of x = a.D + b
                if diag:
                    sign = '+'
                    if (SXb < 0):
                        sign = ''
                    print('x =',SXa,'D',sign,SXb)
