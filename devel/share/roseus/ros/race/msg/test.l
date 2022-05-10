;; Auto-generated. Do not edit!


(when (boundp 'race::test)
  (if (not (find-package "RACE"))
    (make-package "RACE"))
  (shadow 'test (find-package "RACE")))
(unless (find-package "RACE::TEST")
  (make-package "RACE::TEST"))

(in-package "ROS")
;;//! \htmlinclude test.msg.html


(defclass race::test
  :super ros::object
  :slots (_test ))

(defmethod race::test
  (:init
   (&key
    ((:test __test) "")
    )
   (send-super :init)
   (setq _test (string __test))
   self)
  (:test
   (&optional __test)
   (if __test (setq _test __test)) _test)
  (:serialization-length
   ()
   (+
    ;; string _test
    4 (length _test)
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; string _test
       (write-long (length _test) s) (princ _test s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; string _test
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _test (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
   ;;
   self)
  )

(setf (get race::test :md5sum-) "8d4693bd9512b94755470aae7830e048")
(setf (get race::test :datatype-) "race/test")
(setf (get race::test :definition-)
      "string test

")



(provide :race/test "8d4693bd9512b94755470aae7830e048")


