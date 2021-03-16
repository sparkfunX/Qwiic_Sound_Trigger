# Sound Location with the Qwiic Sound Trigger and the u-blox ZED-F9P Part 2
## Introduction
In the last tutorial, we showed you how to calculate the location of a sound using the new [SparkX Qwiic Sound Trigger](https://www.sparkfun.com/products/17979) and the [u-blox ZED-F9P GNSS receiver](https://www.sparkfun.com/products/16481).

The Qwiic Sound Trigger is a new product which you can use on its own, or as part of your Qwiic system. It is based on the VM1010 from Vesper Technologies and the TI PCA9536 GPIO expander. The VM1010 is a clever little device which can be placed into a very low power "Wake On Sound" mode. When it detects a sound, it wakes up and pulls its TRIG (D<sub>OUT</sub>) pin high. The VM1010 can then be placed into "Normal" mode by pulling the MODE pin low; it then functions as a regular microphone. The analog microphone signal is available on the AUDIO (V<sub>OUT</sub>) pin. All of this happens really quickly, within 50 microseconds (much faster than a capacitive MEMS microphone), so you don't miss the start of the audio signal. This makes it ideal for use as a sound trigger!

The u-blox ZED-F9P GNSS receiver is an old friend. It is a top-of-the-line module for high accuracy GNSS location solutions including RTK that is capable of 10mm, three-dimensional accuracy. With this board, you will be able to know where your (or any object's) X, Y, and Z location is within roughly the width of your fingernail! Something that doesn’t get discussed as much as it should is that the ZED-F9P can also capture the timing of a signal on its INT pin with _**nanosecond**_ resolution! It does that through a UBX message called TIM_TM2.

In this tutorial, we take this to the next dimension. Quite literally! Last time, we used two sound trigger systems to calculate where a sound came from along the line from one trigger to the other. That’s a one-dimensional (1D) system. If we add a third sound trigger, we can _triangulate_ the location of a sound in two dimensions (2D) allowing us to plot the position in X and Y or East and North!

Let's talk first about _triangulation_….
## Sound Source Triangulation
Last time, we learned that we can calculate the location of a sound by measuring the _difference_ in the sound’s time-of-arrival at two sound triggers. In our 1D example, we:

* Converted the time difference into distance (by multiplying by the speed of sound)
* Subtracted that distance from the distance between our two sound triggers
* And then divided by 2 to calculate the distance from the closest trigger (the one that detects the sound first)

In 2D, there is a little more math involved. Let’s call our three sound trigger systems A, B and C. A is our reference or _origin_. If we position trigger B due East from A, we can call the line that joins A to B our X axis. Remember when you used to draw graphs in math class? You had the origin in the bottom left corner of your graph paper and you drew the X axis horizontally out to the right. We are doing the same thing here. Trigger A is our origin at X = 0 and Y = 0. We write that as (0,0). If Trigger B is 8 metres (8m) away from A, then its location is X = 8 and Y = 0. We write that as (8,0).



So far, so good. Now, where should we position trigger C? In reality, it doesn’t really matter. We could position C so that ABC forms a perfect equilateral triangle (a triangle where all three sides are the same length). That would give us the best coverage of the area. But the coordinates of C would be (4,6.93). Not pleasant.



To keep the math simpler for this example, let’s position C 6m due North from A. The coordinates of C are X = 0 and Y = 6. We write that as (0,6).



We know the distance from A to B is 8m, and that the distance from A to C is 6m. But what about the distance from B to C? We need to know that too. If we get out a tape measure and measure it, we’ll find it is exactly 10m. If you remember Pythagoras’ Theorem from your math class, _the square on the hypotenuse is equal to the sum of the squares on the other two sides_, we can calculate the distance from B to C by:

* Squaring (multiplying by itself) the distance from A to B: 8 x 8 = 64
* Squaring (multiplying by itself) the distance from A to C: 6 x 6 = 36
* Summing (adding) them: 64 + 36 = 100
* Calculating the square root: √100 = 10



Again, to keep the math easier, let’s pretend that the speed of sound is 1 metre per second (1m/s), not 343.42m/s.

Now, let’s suppose our sound trigger system is running and it detects a sound:

* The time recorded by trigger A is 10:00:03.605551
* The time recorded by trigger B is 10:00:05.385165
* The time recorded by trigger C is 10:00:05.000000

Let’s calculate the differences in those times:

* A records the sound first, so we will use that as our reference
* B records the sound 10:00:05.385165 - 10:00:03.605551 = 1.779614 seconds later
* C records the sound 10:00:05.000000 - 10:00:03.605551 = 1.394449 seconds later

With the speed of sound being 1m/s, we now know that the sound travelled an extra 1.779614m when travelling to B compared to A. And we know that the sound travelled an extra 1.394449m when travelling to C compared to A.

What we do not know is how far the sound travelled to get to A. That’s the first thing we need to calculate.

Let’s call the location of the sound: S. Let’s call the coordinates of S: ( x , y ). Let’s call the distance from S to A: D.

If we sketch that - not to scale - it looks like this:



We know that:

* The distance from S to A is D metres
* The distance from S to B is D + 1.779614 metres
* The distance from S to C is D + 1.394449 metres

In order to find the location of S, we need to use triangles. That’s why this technique is called “triangulation”. In trigonometry and geometry, triangulation is the process of determining the location of a point by forming triangles to it from known points.

If we divide our diagram up into more triangles:



We can use Pythagoras’ Theorem to calculate the distances we need:

* D<sup>2</sup> = x<sup>2</sup> + y<sup>2</sup>
* (D + 1.779614)<sup>2</sup> = (8 - x)<sup>2</sup> + y<sup>2</sup>
* (D + 1.394449)<sup>2</sup> = x<sup>2</sup> + (6 - y)<sup>2</sup>

We can write that as:

* D<sup>2</sup> = x<sup>2</sup> + y<sup>2</sup>
* y<sup>2</sup> = (D + 1.779614)<sup>2</sup> - (8 - x)<sup>2</sup>
* x<sup>2</sup> = (D + 1.394449)<sup>2</sup> - (6 - y)<sup>2</sup>

We need to solve for D. To begin, let’s substitute the value for y<sup>2</sup> from the second equation into the first equation:

* D<sup>2</sup> =  x<sup>2</sup> + (D + 1.779614)<sup>2</sup> - (8 - x)<sup>2</sup>

Multiplying out the brackets, that becomes:

* D<sup>2</sup> =  x<sup>2</sup> + D<sup>2</sup> + 1.779614.D + 1.779614.D + 1.779614<sup>2</sup> - ( 8<sup>2</sup> - 8x - 8x + x<sup>2</sup> )

Simplifying, it becomes:

* D<sup>2</sup> =  x<sup>2</sup> + D<sup>2</sup> + 3.559228.D + 3.167026 - (64 - 16.x + x<sup>2</sup>)

If we remove the brackets:

* D<sup>2</sup> =  x<sup>2</sup> + D<sup>2</sup> + 3.559228.D + 3.167026 - 64 + 16.x - x<sup>2</sup>

The D<sup>2</sup> cancel out and so do the x<sup>2</sup> leaving:

* 0 = 3.559228.D + 3.167026 - 64 + 16.x

One final rearrange leaves:

* 16.x = -3.559228.D + 60.832974

If we divide through by 16 we are left with:

* x = -0.222452.D + 3.802061

Now let’s go back to our three triangles:

* D<sup>2</sup> = x<sup>2</sup> + y<sup>2</sup>
* y<sup>2</sup> = (D + 1.779614)<sup>2</sup> - (8 - x)<sup>2</sup>
* x<sup>2</sup> = (D + 1.394449)<sup>2</sup> - (6 - y)<sup>2</sup>

This time, let’s substitute the value for x<sup>2</sup> from the third equation into the first equation:

* D<sup>2</sup> = (D + 1.394449)<sup>2</sup> - (6 - y)<sup>2</sup> + y<sup>2</sup>

Multiplying out the brackets, that becomes:

* D<sup>2</sup> =  D<sup>2</sup> + 1.394449D + 1.394449D + 1.944488 - (36 - 6y - 6y + y<sup>2</sup>) + y<sup>2</sup>

If we remove the brackets:

* D<sup>2</sup> =  D<sup>2</sup> + 1.394449D + 1.394449D + 1.944488 - 36 + 6y + 6y - y<sup>2</sup> + y<sup>2</sup>

Again, the D<sup>2</sup> cancel out and so do the y<sup>2</sup> leaving:

* 0 = 2.788898.D + 1.944488 - 36 + 12.y

One final rearrange leaves:

* 12.y = -2.788898.D + 34.055512

If we divide through by 12 we are left with:

* y = -0.232408.D + 2.837959

Now we can put our values for x and y back into our first equation:

* D<sup>2</sup> = x<sup>2</sup> + y<sup>2</sup>

* D<sup>2</sup> = (-0.222452.D + 3.802061)<sup>2</sup> + (-0.232408.D + 2.837959)<sup>2</sup>

Multiplying out the brackets, we get:

* D<sup>2</sup> = 0.049485.D<sup>2</sup> - 1.691552.D + 14.455668 + 0.054013.D<sup>2</sup> - 1.319129.D + 8.054011

Simplifying:

* 0.896502.D<sup>2</sup> + 3.010681.D - 22.509679 = 0

Now, I’m sure you will remember _quadratic equations_ from your math class too? We can solve for D using the equation:

* ( -b +/- √(b<sup>2</sup> - 4.a.c ) / 2.a

* a = 0.896502
* b = 3.010681
* c = -22.509679

Inserting our values, D is:

* ( -3.010681 +/- √(9.064200 + 80.719889) ) / 1.793004

Which equals:

* 3.605550 or -6.963804

We can ignore the negative value since it is not within our triangle. Now we know that D is 3.605550m !

Looking back at our equations for x and y:

* x = -0.222452.D + 3.802061
* y = -0.232408.D + 2.837959

If we insert the value for D, we can finally calculate the x and y coordinates of S:

* ( -0.802062 + 3.802061 , -0.837959 + 2.837959 )

Which is:

* ( 3.000 , 2.000 )



Fancy that!

This technique can be used on any configuration of trigger locations. They don’t have to be positioned in a neat right angle triangle. If you would like proof of that and would like to see the math to solve it, have a look at the section called Here There Be Dragons!
## Now, The Good News
Thanks for sticking with us. The good news is that we’ve written more Python code to do that math for you!

The [GitHub Hardware Repo](https://github.com/sparkfunX/Qwiic_Sound_Trigger) contains [two examples](https://github.com/sparkfunX/Qwiic_Sound_Trigger/tree/master/Examples) for the Sound Trigger. You can run these in the Arduino IDE. The second example is the crowd pleaser! It runs on the [MicroMod Data Logging Carrier Board](https://www.sparkfun.com/products/16829) together with the [Artemis Processor Board](https://www.sparkfun.com/products/16401) but it should work just fine with any of our processor boards. It communicates with our [ZED-F9P Breakout](https://www.sparkfun.com/products/16481) and the [Qwiic Sound Trigger](https://www.sparkfun.com/products/17979) to capture sound events and log them to SD card as u-blox UBX TIM_TM2 messages.

Once you have your three sound event UBX TIM_TM2 files, our [Sound_Trigger_Analyzer_2D.py](https://github.com/sparkfunX/Qwiic_Sound_Trigger/tree/master/Utils) will process the files for you and calculate the location of the sound events!

* Copy the TIM_TM2.ubx file from the SD card from the first system. Rename the file so you know which system it came from. You might want to call it TIM_TM2_A.ubx.
* Do the same for the TIM_TM2.ubx files from the second and third systems. Again, rename them so you know which system they came from.
* Place all three files in the same folder / directory as the Sound_Trigger_Analyzer_2D.py Python file

Run the Python code by calling:

```**python Sound_Trigger_Analyzer_2D.py TIM_TM2_A.ubx TIM_TM2_B.ubx TIM_TM2_C.ubx 8.0 6.0 10.0 20.0**```

* Replace TIM_TM2_A.ubx with the name of the file from the first system (A)
* Replace TIM_TM2_B.ubx with the name of the file from the second system (B)
* Replace TIM_TM2_C.ubx with the name of the file from the third system (C)
* Replace the 8.0, 6.0 and 10.0 with the distances between your sound triggers in metres
  * You must enter them in the correct order: **A-B** then **A-C** then **B-C**
* The 20.0 is optional. It is the temperature in Celcius (Centigrade). Change this to your actual temperature for added accuracy

The Python code will calculate and display the (x,y) coordinates of any valid sound events it finds. The coordinate system uses the location of A as the origin, and the line running from A to B as the X axis. The Y axis is (of course) 90 degrees counterclockwise from X.

The calculation code assumes that the X coordinate of C is _between_ A and B. If C lies to the left of A, or the right of B, then you need to rename your points. If C is to the left of A, then B becomes A, C becomes B, and A becomes C:



Enjoy your coordinates!
## Here There Be Dragons!
Sound_Trigger_Analyzer_2D.py can work with any configuration of the trigger positions (so long as the X coordinate of C is _between_ A and B). They do not need to be arranged in a nice right-angled triangle with A-C pointing North. Don’t believe me? Want proof? Good for you! OK. Brace yourself. Here we go… Here is the actual math used in Sound_Trigger_Analyzer_2D.py.
### Sensor Coordinates
Let’s suppose our three sound triggers are arranged such that they form a triangle ABC:




We know the length of the sides: AB, AC and BC.

We know the coordinates of A and B. A is our origin and is located at (0,0). B is due ‘East’ of A and defines our X axis. So we say that B is located at (AB,0). But we don’t yet know the _location_ of C. We need to calculate it. Let’s call the location of C (C<sub>X</sub>,C<sub>Y</sub>).

If we draw a line from C so it meets line A-B at right angles:



Pythagoras’ Theorem tells us that:

* **Equation 1:** AC<sup>2</sup> = C<sub>X</sub><sup>2</sup> + C<sub>Y</sub><sup>2</sup>
* **Equation 2:** BC<sup>2</sup> = ( AB - C<sub>X</sub> )<sup>2</sup> + C<sub>Y</sub><sup>2</sup>

If we rearrange those equations, bringing C<sub>Y</sub><sup>2</sup> to the left and multiplying out the brackets, we get:

* C<sub>Y</sub><sup>2</sup> = AC<sup>2</sup> - C<sub>X</sub><sup>2</sup>
* C<sub>Y</sub><sup>2</sup> = BC<sup>2</sup> - AB<sup>2</sup> + 2.AB.C<sub>X</sub> - C<sub>X</sub><sup>2</sup>

Because those two equations are equal, we can arrange them as:

* AC<sup>2</sup> - C<sub>X</sub><sup>2</sup> = BC<sup>2</sup> - AB<sup>2</sup> + 2.AB.C<sub>X</sub> - C<sub>X</sub><sup>2</sup>

The two -C<sub>X</sub><sup>2</sup> cancel out. Rearranging, we get:

* C<sub>X</sub> = ( AC<sup>2</sup> - BC<sup>2</sup> + AB<sup>2</sup> ) / 2.AB

We can find C<sub>Y</sub> by inserting the value for  C<sub>X</sub> into one of our earlier equations:

* C<sub>Y</sub><sup>2</sup> = AC<sup>2</sup> - C<sub>X</sub><sup>2</sup>

So:

* C<sub>Y</sub> = √ ( AC<sup>2</sup> - C<sub>X</sub><sup>2</sup> )

We now have the coordinates for C: ( C<sub>X</sub> , C<sub>Y</sub> )
### Sound Event Timings
Now, let’s suppose our sound trigger system is running and it detects a sound. Let’s call the location of the sound S and give it coordinates ( S<sub>X</sub> , S<sub>Y</sub> ):



Our three systems are logging the time the sound arrives at each sensor (very accurately!).

The time taken for the sound to reach A is: the distance from S to A divided by the speed of sound. Likewise for B and C.

If all three systems hear the sound at the exact same time, then we know that the sound took an equal time to travel from S to A, B and C. That’s a special case where the distances SA, SB and SC are equal (_equidistant_). But usually sensors B and C will hear the sound at different times to A due to the distances SB and SC being different to SA. We need to include those differences in distance in our calculation. To save some typing, let’s call the distance SA: D

* SB = D + D<sub>diffB</sub>
* SC = D + D<sub>diffC</sub>

D<sub>diffB</sub> is the difference between SB and D. Likewise, D<sub>diffC</sub> is the difference between SC and D. Note that D<sub>diffB</sub> or D<sub>diffC</sub> could be negative if the source of the sound is closer to B or C than A.

Sensors B and C will hear the sound earlier or later than A depending on D<sub>diffB</sub> and D<sub>diffC</sub>.

What we do not know is how long the sound took to travel from S to A, because we do not know the distance D. That’s what we need to calculate,

What we do know is the _difference_ in the time of arrival of the sound at our three sensors. If we bring the three sets of sensor data together, we can calculate the _difference_ in the time of arrival at each sensor. Let’s say that:

* The time recorded by trigger A is timeA
* The time recorded by trigger B is timeB
* The time recorded by trigger C is timeC

Let’s calculate the differences in those times:

* T<sub>diffB</sub> = timeB - timeA
* T<sub>diffC</sub> = timeC - timeA

Again, T<sub>diffB</sub> and T<sub>diffC</sub> could be negative if the source of the sound is closer to B or C than A.
### Sound Event Coordinates
We can convert those differences in time into differences in distance by multiplying by the speed of sound:

* D<sub>diffB</sub> = T<sub>diffB</sub> * speed_of_sound
* D<sub>diffC</sub> = T<sub>diffC</sub> * speed_of_sound

Now we can use Pythogoras’ Theorem as before to study our triangles:

* D<sup>2</sup> = S<sub>X</sub><sup>2</sup> + S<sub>Y</sub><sup>2</sup>
* (D + D<sub>diffB</sub>)<sup>2</sup> = (AB - S<sub>X</sub>)<sup>2</sup> + S<sub>Y</sub><sup>2</sup>

We need to add an extra triangle to include SC:



The width of the new triangle is:

* C<sub>X</sub> - S<sub>X</sub>

The height of the new triangle is:

* C<sub>Y</sub> - S<sub>Y</sub>

So, using Pythagoras’ Theorem again:

* (D + D<sub>diffC</sub>)<sup>2</sup> = (C<sub>X</sub> - S<sub>X</sub>)<sup>2</sup> + (C<sub>Y</sub> - S<sub>Y</sub>)<sup>2</sup>

We now have our three equations:

* **Equation 3:** D<sup>2</sup> = S<sub>X</sub><sup>2</sup> + S<sub>Y</sub><sup>2</sup>
* **Equation 4:** (D + D<sub>diffB</sub>)<sup>2</sup> = (AB - S<sub>X</sub>)<sup>2</sup> + S<sub>Y</sub><sup>2</sup>
* **Equation 5:** (D + D<sub>diffC</sub>)<sup>2</sup> = (C<sub>X</sub> - S<sub>X</sub>)<sup>2</sup> + (C<sub>Y</sub> - S<sub>Y</sub>)<sup>2</sup>

We need to solve for D. To begin, let’s substitute the value for S<sub>Y</sub><sup>2</sup> from the second equation into the first equation, so we can find the relationship between D and S<sub>X</sub>:

* D<sup>2</sup> = S<sub>X</sub><sup>2</sup> + (D + D<sub>diffB</sub>)<sup>2</sup> - (AB - S<sub>X</sub>)<sup>2</sup>

Multiplying out the brackets, that becomes:

* D<sup>2</sup> = S<sub>X</sub><sup>2</sup> + D<sup>2</sup> + 2.D<sub>diffB</sub>.D  + D<sub>diffB</sub><sup>2</sup> - AB<sup>2</sup> + 2.AB.S<sub>X</sub> - S<sub>X</sub><sup>2</sup>

As before, the D<sup>2</sup> cancel and so do the S<sub>X</sub><sup>2</sup> leaving:

* 0 = 2.D<sub>diffB</sub>.D + D<sub>diffB</sub><sup>2</sup> - AB<sup>2</sup> + 2.AB.S<sub>X</sub>

Rearranging:

* 2.AB.S<sub>X</sub> = AB<sup>2</sup> - 2.D<sub>diffB</sub>.D - D<sub>diffB</sub><sup>2</sup>

Divide through by 2.AB:

* S<sub>X</sub> = ( AB<sup>2</sup> - 2.D<sub>diffB</sub>.D - D<sub>diffB</sub><sup>2</sup> ) / 2.AB

Let’s make life easier for ourselves and say that:

* **Equation 6:** S<sub>X</sub> = S<sub>Xa</sub>.D + S<sub>Xb</sub>

Where:

* S<sub>Xa</sub> = ( -2.D<sub>diffB</sub> ) / 2.AB
* S<sub>Xb</sub> = ( AB<sup>2</sup> - D<sub>diffB</sub><sup>2</sup> ) / 2.AB

We can calculate both values because we know AB and can calculate D<sub>diffB</sub>:

* D<sub>diffB</sub> = T<sub>diffB</sub> * speed_of_sound = ( timeB - timeA ) * speed_of_sound

Looking at equation 3 again:

* D<sup>2</sup> = S<sub>X</sub><sup>2</sup> + S<sub>Y</sub><sup>2</sup>

If we replace S<sub>X</sub> with ( S<sub>Xa</sub>.D + S<sub>Xb</sub> ), we get:

* D<sup>2</sup> = ( S<sub>Xa</sub>.D + S<sub>Xb</sub> )<sup>2</sup> + S<sub>Y</sub><sup>2</sup>

Rearranging:

* S<sub>Y</sub><sup>2</sup> = D<sup>2</sup> - ( S<sub>Xa</sub>.D + S<sub>Xb</sub> )<sup>2</sup>

Multiplying out the bracket, it becomes:

* S<sub>Y</sub><sup>2</sup> = D<sup>2</sup> - S<sub>Xa</sub><sup>2</sup>.D<sup>2</sup> - 2.S<sub>Xa</sub>.S<sub>Xb</sub>.D - S<sub>Xb</sub><sup>2</sup>

Simplifying:

* S<sub>Y</sub><sup>2</sup> = ( 1 - S<sub>Xa</sub><sup>2</sup> ).D<sup>2</sup> - 2.S<sub>Xa</sub>.S<sub>Xb</sub>.D - S<sub>Xb</sub><sup>2</sup>

So:

* **Equation 7:** S<sub>Y</sub> = √ ( ( 1 - S<sub>Xa</sub><sup>2</sup> ).D<sup>2</sup> - 2.S<sub>Xa</sub>.S<sub>Xb</sub>.D - S<sub>Xb</sub><sup>2</sup> )

That’s ever so slightly horrible, but let’s go with it…

Now we have values for S<sub>X</sub> and S<sub>Y</sub> in terms of D, which we can plug into equation 5:

* (D + D<sub>diffC</sub>)<sup>2</sup> = (C<sub>X</sub> - S<sub>X</sub>)<sup>2</sup> + (C<sub>Y</sub> - S<sub>Y</sub>)<sup>2</sup>

Substituting for S<sub>X</sub> and S<sub>Y</sub> we get:

* (D + D<sub>diffC</sub>)<sup>2</sup> = (C<sub>X</sub> - ( S<sub>Xa</sub>.D + S<sub>Xb</sub> ) )<sup>2</sup> + (C<sub>Y</sub> - √ ( ( 1 - S<sub>Xa</sub><sup>2</sup> ).D<sup>2</sup> - 2.S<sub>Xa</sub>.S<sub>Xb</sub>.D - S<sub>Xb</sub><sup>2</sup> ) )<sup>2</sup>

Multiplying out the brackets gives (brace yourself!):

* D<sup>2</sup> + 2.D<sub>diffC</sub>.D + D<sub>diffC</sub><sup>2</sup> = C<sub>X</sub><sup>2</sup> - 2.C<sub>X</sub>.S<sub>Xa</sub>.D - 2.C<sub>X</sub>.S<sub>Xb</sub> + S<sub>Xa</sub><sup>2</sup>.D<sup>2</sup> + 2.S<sub>Xa</sub>.S<sub>Xb</sub>.D + S<sub>Xb</sub><sup>2</sup> + C<sub>Y</sub><sup>2</sup> - 2.C<sub>Y</sub>.√ ( ( 1 - S<sub>Xa</sub><sup>2</sup> ).D<sup>2</sup> - 2.S<sub>Xa</sub>.S<sub>Xb</sub>.D - S<sub>Xb</sub><sup>2</sup> ) + ( 1 - S<sub>Xa</sub><sup>2</sup> ).D<sup>2</sup> - 2.S<sub>Xa</sub>.S<sub>Xb</sub>.D - S<sub>Xb</sub><sup>2</sup>

Simplifying and moving everything but the root to the left gives:

* ( 1 - S<sub>Xa</sub><sup>2</sup> - ( 1 - S<sub>Xa</sub><sup>2</sup> )).D<sup>2</sup> + ( 2.D<sub>diffC</sub> + 2.C<sub>X</sub>.S<sub>Xa</sub> - 2.S<sub>Xa</sub>.S<sub>Xb</sub> + 2.S<sub>Xa</sub>.S<sub>Xb</sub>).D + D<sub>diffC</sub><sup>2</sup> - C<sub>X</sub><sup>2</sup> + 2.C<sub>X</sub>.S<sub>Xb</sub> - S<sub>Xb</sub><sup>2</sup> - C<sub>Y</sub><sup>2</sup> + S<sub>Xb</sub><sup>2</sup> = -2.C<sub>Y</sub>.√ ( ( 1 - S<sub>Xa</sub><sup>2</sup> ).D<sup>2</sup> - 2.S<sub>Xa</sub>.S<sub>Xb</sub>.D - S<sub>Xb</sub><sup>2</sup> )

Fortunately the D<sup>2</sup> term cancels out completely, which is good news. If it didn’t, we’d have a quartic equation to solve instead of a quadratic…

Simplifying:

* ( 2.D<sub>diffC</sub> + 2.C<sub>X</sub>.S<sub>Xa</sub>).D + D<sub>diffC</sub><sup>2</sup> - C<sub>X</sub><sup>2</sup> + 2.C<sub>X</sub>.S<sub>Xb</sub> - C<sub>Y</sub><sup>2</sup> = -2.C<sub>Y</sub>.√ ( ( 1 - S<sub>Xa</sub><sup>2</sup> ).D<sup>2</sup> - 2.S<sub>Xa</sub>.S<sub>Xb</sub>.D - S<sub>Xb</sub><sup>2</sup> )

Dividing through by -2.C<sub>Y</sub> gives:

* ( ( 2.D<sub>diffC</sub> + 2.C<sub>X</sub>.S<sub>Xa</sub> ).D + D<sub>diffC</sub><sup>2</sup> - C<sub>X</sub><sup>2</sup> + 2.C<sub>X</sub>.S<sub>Xb</sub> - C<sub>Y</sub><sup>2</sup> ) / -2.C<sub>Y</sub> = √ ( ( 1 - S<sub>Xa</sub><sup>2</sup> ).D<sup>2</sup> - 2.S<sub>Xa</sub>.S<sub>Xb</sub>.D - S<sub>Xb</sub><sup>2</sup> )

We now need to square both sides to get rid of the root. Painful… But we need to do it… But first, to make life easier for ourselves, let’s simplify further and say:

* da.D + db = √ ( ( 1 - S<sub>Xa</sub><sup>2</sup> ).D<sup>2</sup> - 2.S<sub>Xa</sub>.S<sub>Xb</sub>.D - S<sub>Xb</sub><sup>2</sup> )

Where:

* da = ( 2.D<sub>diffC</sub> + 2.C<sub>X</sub>.S<sub>Xa</sub> ) / -2.C<sub>Y</sub>
* db = ( D<sub>diffC</sub><sup>2</sup> - C<sub>X</sub><sup>2</sup> + 2.C<sub>X</sub>.S<sub>Xb</sub> - C<sub>Y</sub><sup>2</sup> ) / -2.C<sub>Y</sub>

Remember that we can already calculate values for da and db because we know or can calculate values for: D<sub>diffC</sub>; C<sub>X</sub>; S<sub>Xb</sub>; and C<sub>Y</sub>.

Square both sides:

* da<sup>2</sup>.D<sup>2</sup> + 2.da.db.D + db<sup>2</sup> = ( 1 - S<sub>Xa</sub><sup>2</sup> ).D<sup>2</sup> - 2.S<sub>Xa</sub>.S<sub>Xb</sub>.D - S<sub>Xb</sub><sup>2</sup>

Moving the terms on the right over to the left:

* ( da<sup>2</sup> - ( 1 - S<sub>Xa</sub><sup>2</sup> ) ).D<sup>2</sup> + ( 2.da.db + 2.S<sub>Xa</sub>.S<sub>Xb</sub> ).D + db<sup>2</sup> + S<sub>Xb</sub><sup>2</sup> = 0

That’s a quadratic equation which we can solve in the usual way. Let’s make:

* qa = da<sup>2</sup> - 1 + S<sub>Xa</sub><sup>2</sup>
* qb = 2.da.db + 2.S<sub>Xa</sub>.S<sub>Xb</sub>
* qc = db<sup>2</sup> + S<sub>Xb</sub><sup>2</sup>

Remember that we can already calculate values for qa, qb and qc because we already know: da; S<sub>Xa</sub>; db; and S<sub>Xb</sub>.

Solving the quadratic::

* D = ( -qb +/- √( qb<sup>2</sup> - 4.qa.qc ) ) / 2.qa

That gives us two possible values for D. One will be negative or otherwise invalid, leaving us with a value for D.

We can then plug the value for D into equation 6 to find the X coordinate of S because we already know the values of S<sub>Xa</sub> and S<sub>Xb</sub>:

* S<sub>X</sub> = S<sub>Xa</sub>.D + S<sub>Xb</sub>

We can then calculate the Y coordinate of S using equation 3:

* D<sup>2</sup> = S<sub>X</sub><sup>2</sup> + S<sub>Y</sub><sup>2</sup>
* S<sub>Y</sub> = √( D<sup>2</sup> - S<sub>X</sub><sup>2</sup> )

Enjoy your coordinates!
