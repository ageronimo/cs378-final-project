#gnome-terminal -e roscore
gnome-terminal --tab -- bash -c "sleep 1s; roscore"
gnome-terminal --tab -- bash -c "sleep 2s; roslaunch ./src/race/src/basiclanuch.launch"
