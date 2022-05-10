all:
	roscore &
	rosrun rosserial_python serial_node.py /dev/ttyACM0 &
	rosrun urg_node urg_node _ip_address:=192.168.0.10 &
	rosrun race talker.py &
	rosrun race dist_finder.py &

serial:
	rosrun rosserial_python serial_node.py /dev/ttyACM0

lidar:
	rosrun urg_node urg_node _ip_address:=192.168.0.10

control:
	#rosrun race control.py
	rosrun race control

dist:
	#rosrun race dist_finder.py
	rosrun race dist_finder

kill:
	rosrun race kill.py

talker:
	#rosrun race talker.py
	rosrun race talker

clean:
	killall -9 python
	killall -9 roscore
	killall -9 urg_node
	killall -9 rosmaster
