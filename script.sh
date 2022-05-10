echo running car system
xterm -e roscore & 
xterm -e roslaunch ./src/race/src/basiclanuch.launch &
xterm -e rosrun race dist_finder &
xterm -e rosrun race hw1pub &
xterm -e rosrun race control

