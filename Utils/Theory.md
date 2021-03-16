

We know the length of the sides: AB, AC and BC.

We know the coordinates of A and B. A is our origin and is located at (0,0). B is due ‘East’ of A and defines our X axis. So we say that B is located at (AB,0). But we don’t yet know the _location_ of C. We need to calculate it. Let’s call the location of C (C<sub>X</sub>,C<sub>Y</sub>).

If we draw a line from C so it meets line A-B at right angles:



Pythagoras’ Theorem tells us that:

Equation 1: AC<sup>2</sup> = C<sub>X</sub><sup>2</sup> + C<sub>Y</sub><sup>2</sup>
Equation 2: BC<sup>2</sup> = ( AB - C<sub>X</sub> )<sup>2</sup> + C<sub>Y</sub><sup>2</sup>

If we rearrange those equations, bringing C<sub>Y</sub><sup>2</sup> to the left and multiplying out the brackets, we get:

C<sub>Y</sub><sup>2</sup> = AC<sup>2</sup> - C<sub>X</sub><sup>2</sup>
C<sub>Y</sub><sup>2</sup> = BC<sup>2</sup> - AB<sup>2</sup> + 2.AB.C<sub>X</sub> - C<sub>X</sub><sup>2</sup>

Because those two equations are equal, we can arrange them as:

AC<sup>2</sup> - C<sub>X</sub><sup>2</sup> = BC<sup>2</sup> - AB<sup>2</sup> + 2.AB.C<sub>X</sub> - C<sub>X</sub><sup>2</sup>

The two -C<sub>X</sub><sup>2</sup> cancel out. Rearranging, we get:

C<sub>X</sub> = ( AC<sup>2</sup> - BC<sup>2</sup> + AB<sup>2</sup> ) / 2.AB

We can find C<sub>Y</sub> by inserting the value for  C<sub>X</sub> into one of our earlier equations:

C<sub>Y</sub><sup>2</sup> = AC<sup>2</sup> - C<sub>X</sub><sup>2</sup>

So:

C<sub>Y</sub> = √ ( AC<sup>2</sup> - C<sub>X</sub><sup>2</sup> )

We now have the coordinates for C: ( C<sub>X</sub> , C<sub>Y</sub> )
Sound Event Timings
Now, let’s suppose our sound trigger system is running and it detects a sound. Let’s call the location of the sound S and give it coordinates ( S<sub>X</sub> , S<sub>Y</sub> ):



Our three systems are logging the time the sound arrives at each sensor (very accurately!).

The time taken for the sound to reach A is: the distance from S to A divided by the speed of sound. Likewise for B and C.

If all three systems hear the sound at the exact same time, then we know that the sound took an equal time to travel from S to A, B and C. That’s a special case where the distances SA, SB and SC are equal (_equidistant_). But usually sensors B and C will hear the sound at different times to A due to the distances SB and SC being different to SA. We need to include those differences in distance in our calculation. To save some typing, let’s call the distance SA: D

SB = D + D<sub>diffB</sub>
SC = D + D<sub>diffC</sub>

D<sub>diffB</sub> is the difference between SB and D. Likewise, D<sub>diffC</sub> is the difference between SC and D. Note that D<sub>diffB</sub> or D<sub>diffC</sub> could be negative if the source of the sound is closer to B or C than A.

Sensors B and C will hear the sound earlier or later than A depending on D<sub>diffB</sub> and D<sub>diffC</sub>.

What we do not know is how long the sound took to travel from S to A, because we do not know the distance D. That’s what we need to calculate,

What we do know is the _difference_ in the time of arrival of the sound at our three sensors. If we bring the three sets of sensor data together, we can calculate the _difference_ in the time of arrival at each sensor. Let’s say that:

The time recorded by trigger A is timeA
The time recorded by trigger B is timeB
The time recorded by trigger C is timeC

Let’s calculate the differences in those times:

T<sub>diffB</sub> = timeB - timeA
T<sub>diffC</sub> = timeC - timeA

Again, T<sub>diffB</sub> and T<sub>diffC</sub> could be negative if the source of the sound is closer to B or C than A.
Sound Event Coordinates
We can convert those differences in time into differences in distance by multiplying by the speed of sound:

D<sub>diffB</sub> = T<sub>diffB</sub> * speed_of_sound
D<sub>diffC</sub> = T<sub>diffC</sub> * speed_of_sound

Now we can use Pythogoras’ Theorem as before to study our triangles:

D<sup>2</sup> = S<sub>X</sub><sup>2</sup> + S<sub>Y</sub><sup>2</sup>
(D + D<sub>diffB</sub>)<sup>2</sup> = (AB - S<sub>X</sub>)<sup>2</sup> + S<sub>Y</sub><sup>2</sup>

We need to add an extra triangle to include SC:



The width of the new triangle is:

C<sub>X</sub> - S<sub>X</sub>

The height of the new triangle is:

C<sub>Y</sub> - S<sub>Y</sub>

So, using Pythagoras’ Theorem again:

(D + D<sub>diffC</sub>)<sup>2</sup> = (C<sub>X</sub> - S<sub>X</sub>)<sup>2</sup> + (C<sub>Y</sub> - S<sub>Y</sub>)<sup>2</sup>

We now have our three equations:

Equation 3: D<sup>2</sup> = S<sub>X</sub><sup>2</sup> + S<sub>Y</sub><sup>2</sup>
Equation 4: (D + D<sub>diffB</sub>)<sup>2</sup> = (AB - S<sub>X</sub>)<sup>2</sup> + S<sub>Y</sub><sup>2</sup>
Equation 5: (D + D<sub>diffC</sub>)<sup>2</sup> = (C<sub>X</sub> - S<sub>X</sub>)<sup>2</sup> + (C<sub>Y</sub> - S<sub>Y</sub>)<sup>2</sup>

We need to solve for D. To begin, let’s substitute the value for S<sub>Y</sub><sup>2</sup> from the second equation into the first equation, so we can find the relationship between D and S<sub>X</sub>:

D<sup>2</sup> = S<sub>X</sub><sup>2</sup> + (D + D<sub>diffB</sub>)<sup>2</sup> - (AB - S<sub>X</sub>)<sup>2</sup>

Multiplying out the brackets, that becomes:

D<sup>2</sup> = S<sub>X</sub><sup>2</sup> + D<sup>2</sup> + 2.D<sub>diffB</sub>.D  + D<sub>diffB</sub><sup>2</sup> - AB<sup>2</sup> + 2.AB.S<sub>X</sub> - S<sub>X</sub><sup>2</sup>

As before, the D<sup>2</sup> cancel and so do the S<sub>X</sub><sup>2</sup> leaving:

0 = 2.D<sub>diffB</sub>.D + D<sub>diffB</sub><sup>2</sup> - AB<sup>2</sup> + 2.AB.S<sub>X</sub>

Rearranging:

2.AB.S<sub>X</sub> = AB<sup>2</sup> - 2.D<sub>diffB</sub>.D - D<sub>diffB</sub><sup>2</sup>

Divide through by 2.AB:

S<sub>X</sub> = ( AB<sup>2</sup> - 2.D<sub>diffB</sub>.D - D<sub>diffB</sub><sup>2</sup> ) / 2.AB

Let’s make life easier for ourselves and say that:

Equation 6: S<sub>X</sub> = S<sub>Xa</sub>.D + S<sub>Xb</sub>

Where:

S<sub>Xa</sub> = ( -2.D<sub>diffB</sub> ) / 2.AB
S<sub>Xb</sub> = ( AB<sup>2</sup> - D<sub>diffB</sub><sup>2</sup> ) / 2.AB

We can calculate both values because we know AB and can calculate D<sub>diffB</sub>:

D<sub>diffB</sub> = T<sub>diffB</sub> * speed_of_sound = ( timeB - timeA ) * speed_of_sound

Looking at first equation 3 again:

D<sup>2</sup> = S<sub>X</sub><sup>2</sup> + S<sub>Y</sub><sup>2</sup>

If we replace S<sub>X</sub> with ( S<sub>Xa</sub>.D + S<sub>Xb</sub> ), we get:

D<sup>2</sup> = ( S<sub>Xa</sub>.D + S<sub>Xb</sub> )<sup>2</sup> + S<sub>Y</sub><sup>2</sup>

Rearranging:

S<sub>Y</sub><sup>2</sup> = D<sup>2</sup> - ( S<sub>Xa</sub>.D + S<sub>Xb</sub> )<sup>2</sup>

Multiplying out the bracket, it becomes:

S<sub>Y</sub><sup>2</sup> = D<sup>2</sup> - S<sub>Xa</sub><sup>2</sup>.D<sup>2</sup> - 2.S<sub>Xa</sub>.S<sub>Xb</sub>.D - S<sub>Xb</sub><sup>2</sup>

Simplifying:

S<sub>Y</sub><sup>2</sup> = ( 1 - S<sub>Xa</sub><sup>2</sup> ).D<sup>2</sup> - 2.S<sub>Xa</sub>.S<sub>Xb</sub>.D - S<sub>Xb</sub><sup>2</sup>

So:

Equation 7: S<sub>Y</sub> = √ ( ( 1 - S<sub>Xa</sub><sup>2</sup> ).D<sup>2</sup> - 2.S<sub>Xa</sub>.S<sub>Xb</sub>.D - S<sub>Xb</sub><sup>2</sup> )

That’s ever so slightly horrible, but let’s go with it…

Now we have values for S<sub>X</sub> and S<sub>Y</sub> in terms of D, which we can plug into equation 5:

(D + D<sub>diffC</sub>)<sup>2</sup> = (C<sub>X</sub> - S<sub>X</sub>)<sup>2</sup> + (C<sub>Y</sub> - S<sub>Y</sub>)<sup>2</sup>

Substituting for S<sub>X</sub> and S<sub>Y</sub> we get:

(D + D<sub>diffC</sub>)<sup>2</sup> = (C<sub>X</sub> - ( S<sub>Xa</sub>.D + S<sub>Xb</sub> ) )<sup>2</sup> + (C<sub>Y</sub> - √ ( ( 1 - S<sub>Xa</sub><sup>2</sup> ).D<sup>2</sup> - 2.S<sub>Xa</sub>.S<sub>Xb</sub>.D - S<sub>Xb</sub><sup>2</sup> ) )<sup>2</sup>

Multiplying out the brackets gives (brace yourself!):

D<sup>2</sup> + 2.D<sub>diffC</sub>.D + D<sub>diffC</sub><sup>2</sup> = C<sub>X</sub><sup>2</sup> - 2.C<sub>X</sub>.S<sub>Xa</sub>.D - 2.C<sub>X</sub>.S<sub>Xb</sub> + S<sub>Xa</sub><sup>2</sup>.D<sup>2</sup> + 2.S<sub>Xa</sub>.S<sub>Xb</sub>.D + S<sub>Xb</sub><sup>2</sup> + C<sub>Y</sub><sup>2</sup> - 2.C<sub>Y</sub>.√ ( ( 1 - S<sub>Xa</sub><sup>2</sup> ).D<sup>2</sup> - 2.S<sub>Xa</sub>.S<sub>Xb</sub>.D - S<sub>Xb</sub><sup>2</sup> ) + ( 1 - S<sub>Xa</sub><sup>2</sup> ).D<sup>2</sup> - 2.S<sub>Xa</sub>.S<sub>Xb</sub>.D - S<sub>Xb</sub><sup>2</sup>

Simplifying and moving everything but the root to the left gives:

( 1 - S<sub>Xa</sub><sup>2</sup> - ( 1 - S<sub>Xa</sub><sup>2</sup> )).D<sup>2</sup> + ( 2.D<sub>diffC</sub> + 2.C<sub>X</sub>.S<sub>Xa</sub> - 2.S<sub>Xa</sub>.S<sub>Xb</sub> + 2.S<sub>Xa</sub>.S<sub>Xb</sub>).D + D<sub>diffC</sub><sup>2</sup> - C<sub>X</sub><sup>2</sup> + 2.C<sub>X</sub>.S<sub>Xb</sub> - S<sub>Xb</sub><sup>2</sup> - C<sub>Y</sub><sup>2</sup> + S<sub>Xb</sub><sup>2</sup> = -2.C<sub>Y</sub>.√ ( ( 1 - S<sub>Xa</sub><sup>2</sup> ).D<sup>2</sup> - 2.S<sub>Xa</sub>.S<sub>Xb</sub>.D - S<sub>Xb</sub><sup>2</sup> )

Fortunately the D<sup>2</sup> cancels out completely, which is good news. If it didn’t, we’d have a quartic equation to solve instead of a quadratic…

Simplifying:

( 2.D<sub>diffC</sub> + 2.C<sub>X</sub>.S<sub>Xa</sub>).D + D<sub>diffC</sub><sup>2</sup> - C<sub>X</sub><sup>2</sup> + 2.C<sub>X</sub>.S<sub>Xb</sub> - C<sub>Y</sub><sup>2</sup> = -2.C<sub>Y</sub>.√ ( ( 1 - S<sub>Xa</sub><sup>2</sup> ).D<sup>2</sup> - 2.S<sub>Xa</sub>.S<sub>Xb</sub>.D - S<sub>Xb</sub><sup>2</sup> )

Dividing through by -2.C<sub>Y</sub> gives:

( ( 2.D<sub>diffC</sub> + 2.C<sub>X</sub>.S<sub>Xa</sub> ).D + D<sub>diffC</sub><sup>2</sup> - C<sub>X</sub><sup>2</sup> + 2.C<sub>X</sub>.S<sub>Xb</sub> - C<sub>Y</sub><sup>2</sup> ) / -2.C<sub>Y</sub> = √ ( ( 1 - S<sub>Xa</sub><sup>2</sup> ).D<sup>2</sup> - 2.S<sub>Xa</sub>.S<sub>Xb</sub>.D - S<sub>Xb</sub><sup>2</sup> )

We now need to square both sides to get rid of the root. Painful… But we need to do it… But first, to make life easier for ourselves, let’s simplify further and say:

da.D + db = √ ( ( 1 - S<sub>Xa</sub><sup>2</sup> ).D<sup>2</sup> - 2.S<sub>Xa</sub>.S<sub>Xb</sub>.D - S<sub>Xb</sub><sup>2</sup> )

Where:

da = ( 2.D<sub>diffC</sub> + 2.C<sub>X</sub>.S<sub>Xa</sub> ) / -2.C<sub>Y</sub>
db = ( D<sub>diffC</sub><sup>2</sup> - C<sub>X</sub><sup>2</sup> + 2.C<sub>X</sub>.S<sub>Xb</sub> - C<sub>Y</sub><sup>2</sup> ) / -2.C<sub>Y</sub>

Square both sides:

da<sup>2</sup>.D<sup>2</sup> + 2.da.db.D + db<sup>2</sup> = ( 1 - S<sub>Xa</sub><sup>2</sup> ).D<sup>2</sup> - 2.S<sub>Xa</sub>.S<sub>Xb</sub>.D - S<sub>Xb</sub><sup>2</sup>

Moving the terms on the right over to the left:

( da<sup>2</sup> - ( 1 - S<sub>Xa</sub><sup>2</sup> ) ).D<sup>2</sup> + ( 2.da.db + 2.S<sub>Xa</sub>.S<sub>Xb</sub> ).D + db<sup>2</sup> + S<sub>Xb</sub><sup>2</sup> = 0

That’s a quadratic equation which we can solve in the usual way. Let’s make:

qa = da<sup>2</sup> - 1 + S<sub>Xa</sub><sup>2</sup>
qb = 2.da.db + 2.S<sub>Xa</sub>.S<sub>Xb</sub>
qc = db<sup>2</sup> + S<sub>Xb</sub><sup>2</sup>

Then:

D = ( -qb +/- √( qb<sup>2</sup> - 4.qa.qc ) ) / 2.qa

That gives us two possible values for D. One will be negative or otherwise invalid, leaving us with a value for D.

We can then use plug the value for D into equation 6 to find the X coordinate of S because we already know the values of S<sub>Xa</sub> and S<sub>Xb</sub>:

S<sub>X</sub> = S<sub>Xa</sub>.D + S<sub>Xb</sub>

We can then calculate the Y coordinate of S using equation 7:

S<sub>Y</sub> = √ ( ( 1 - S<sub>Xa</sub><sup>2</sup> ).D<sup>2</sup> - 2.S<sub>Xa</sub>.S<sub>Xb</sub>.D - S<sub>Xb</sub><sup>2</sup> )

Enjoy your coordinates!
