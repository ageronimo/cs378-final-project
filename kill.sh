gnome-terminal --tab -- bash -c "killall -r roscore"
gnome-terminal --tab -- bash -c "sleep 1s; killall -9 rosmaster"
