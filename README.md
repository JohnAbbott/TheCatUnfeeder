# TheCatUnfeeder
Allow the brown cat to eat but close the door if the orange cat tries.

This application monitors a motion detector and if it detects motion it turns on a white LED.  
Then the application reads the data from a light color temperature sensor.  If there is a significant 
amount of warmth in the color that means it is the orange cat reflecting back light.  Kick on the server 
motor and close the door to the feedbowl.  Wait a few minutes for the orange cat to go away and then repeat
the cycle.
