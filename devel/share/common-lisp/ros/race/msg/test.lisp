; Auto-generated. Do not edit!


(cl:in-package race-msg)


;//! \htmlinclude test.msg.html

(cl:defclass <test> (roslisp-msg-protocol:ros-message)
  ((test
    :reader test
    :initarg :test
    :type cl:string
    :initform ""))
)

(cl:defclass test (<test>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <test>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'test)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name race-msg:<test> is deprecated: use race-msg:test instead.")))

(cl:ensure-generic-function 'test-val :lambda-list '(m))
(cl:defmethod test-val ((m <test>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader race-msg:test-val is deprecated.  Use race-msg:test instead.")
  (test m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <test>) ostream)
  "Serializes a message object of type '<test>"
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 'test))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 'test))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <test>) istream)
  "Deserializes a message object of type '<test>"
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'test) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:slot-value msg 'test) __ros_str_idx) (cl:code-char (cl:read-byte istream)))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<test>)))
  "Returns string type for a message object of type '<test>"
  "race/test")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'test)))
  "Returns string type for a message object of type 'test"
  "race/test")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<test>)))
  "Returns md5sum for a message object of type '<test>"
  "8d4693bd9512b94755470aae7830e048")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'test)))
  "Returns md5sum for a message object of type 'test"
  "8d4693bd9512b94755470aae7830e048")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<test>)))
  "Returns full string definition for message of type '<test>"
  (cl:format cl:nil "string test~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'test)))
  "Returns full string definition for message of type 'test"
  (cl:format cl:nil "string test~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <test>))
  (cl:+ 0
     4 (cl:length (cl:slot-value msg 'test))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <test>))
  "Converts a ROS message object to a list"
  (cl:list 'test
    (cl:cons ':test (test msg))
))
