# gnome-terminal -e roslaunch ./src/race/src/basiclanuch.launch &
gnome-terminal --tab -- bash -c "sleep 2s; rosrun race dist_finder; exec bash -i"
gnome-terminal --tab -- bash -c "sleep 2s; rosrun race control; exec bash -i"
gnome-terminal --tab -- bash -c "sleep 2s; rosrun race kill.py; exec bash -i"
gnome-terminal --tab -- bash -c "sleep 3s; rosrun zed_wrapper zed_wrapper_node; exec bash -i"
gnome-terminal --tab -- bash -c "sleep 3s; rosrun color_tracking color_tracking_node; exec bash -i"

