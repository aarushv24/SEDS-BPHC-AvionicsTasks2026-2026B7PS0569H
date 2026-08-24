# SEDS-BPHC-AvionicsTasks2026-2026B7PS0569H

## AARUSH VASHISTH    2026B7PS0569H

## TASK 1

For the Finding The Sea Floor Task, I first plotted a static graph with no noise reduction or corrupted value removal using plain matplotlib and putting the #VALUE! as NaN.

For the next step, I implemented pandas to read the csv file (in the first step, I used the vanilla with open('filename.csv')) and clear out the #VALUE! using the pd.to_numeric(..., errors='coerce') function. Then I added subplots since they are necessary for plotting a real time graph via FuncAnimation. To clear out the corrupted values, I looked at the data and set the threshold of the difference between (y-1) and (y+1) to 60m (assuming 'y' is the current point) since almost every value was falling under that category barring a few outliers, ie the corrupted values. Then interpolated them using y.interpolate().

To smoothen out the graph, I used the rolling mean method where it looks at 5 points (min. 1 point to prevent NaN for the first 5 values), calculates their mean and hence smoothens out the noise by implementing those values. To plot it in real time, I made use of FuncAnimation with interval set at 1 second for every plot.

## TASK 2

To keep watch over Odysseus, the task was to make a system using Tinkercad which will display the state of the sea (or ship in case of WRECKED) on the LCD screen.

I first made the systems for STORM, CHARYBDIS, and the anchor drop system separately. For the push button system (for the anchor mechanism), I had to first map out how to handle the current and last state since each press means a flip, unlike an LED pushbutton system. So it triggers only when the previous button state was LOW and the current (as soon as you press, that is) is HIGH. Which then flips the variable anchorDropped to 1 to be used for later if-else blocks.

For STORM, after the push button project, handling the photoresistor was relatively simpler. I first measured the maximum value using Serial.println() and divided it by half for the threshold value and then a simple if-else block to check the state.

CHARYBDIS was similar too, only this time I first had to measure the time for the ping to come back, divide it by 2 and multiply it by the speed constant (which I found by printing out the value of the ping time at a known distance and finding speed via that) to find out the distance and then a simple if-else block to check the 100cm threshold and handle the buzzer.

Then to wrap it all up, I combined these three projects, added an LCD (and the LiquidCrystal library in the code) and tested it once which the enum seaState. Once I verified everything was working fine one at a time, I then proceeded to add the precedence in STORM and CHARYBDIS case, ie, only one of them can run at the same time. To implement that, I added an if check - the code only checks these two states if and only if the current state is OPEN SEA or the state that's being checked. If it satisfies neither, it implies that the other state is active. For example, if STORM is already running, the code checks the currentState (which is STORM) and hits the if block for CHARYBIDS which runs only when currentState is OPEN SEA or CHARYBDIS - in this case, its neither. So even if the distance is less that 100cm, LCD will show STORM since it was active first.

Finally, I added the WRECKED check using millis() and a time variable that keeps on updating every loop the ship isnt in STORM or CHARYBIDS. As soon as either event happens, it stops updating, ie, becomes a snapshot of the moment when the ship entered STORM or CHARYBDIS. Then with a simple if block to check whether the difference between millis() and the snapshot is 5sec or not. Once its WREKCED, until the simulation is restarted, nothing will change the currentState. Similarly, once anchor is dropped (ie, anchorDrop = 1), until the button is pressed again, LCD will display only ANCHOR DROPPED and no other event matters (unless WRECKED already occurred, in which case anchor drop doesnt matter).
