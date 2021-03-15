# 2D Tests

**python ..\Sound_Trigger_Analyzer_2D.py 1_TIM_TM2_A.ubx 1_TIM_TM2_B.ubx 1_TIM_TM2_C.ubx 10.0 11.0 8.0 20.0**

Distance from sensor A to sensor B is 10.0m
Distance from sensor A to sensor C is 11.0m
Distance from sensor B to sensor C is 8.0m

Test 1: Source located at (4.000,3.000)
Test 2: Source located at (7.000,4.000)
Test 3: Source located at (9.000,1.000) - i.e. East of C

**python ..\Sound_Trigger_Analyzer_2D.py 4_TIM_TM2_A.ubx 4_TIM_TM2_B.ubx 4_TIM_TM2_C.ubx 10.0 10.0 10.0 20.0**

Distance from sensor A to sensor B is 10.0m
Distance from sensor A to sensor C is 10.0m
Distance from sensor B to sensor C is 10.0m

Test 4: Source located at (5.000,2.887) - i.e. center of the triangle
Test 5: Source located at (0.000,5.7735) - i.e. directly North of A on the circumference of the circle
Test 6: Source located at (-0.7735,2.8868) - i.e. West of A on the circumference of the circle - FAILS
Test 7: Source located at (5.000,-2.8868) - i.e. due South of C on the circumference of the circle - FAILS
