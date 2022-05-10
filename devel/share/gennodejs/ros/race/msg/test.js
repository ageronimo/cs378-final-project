// Auto-generated. Do not edit!

// (in-package race.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;

//-----------------------------------------------------------

class test {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.test = null;
    }
    else {
      if (initObj.hasOwnProperty('test')) {
        this.test = initObj.test
      }
      else {
        this.test = '';
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type test
    // Serialize message field [test]
    bufferOffset = _serializer.string(obj.test, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type test
    let len;
    let data = new test(null);
    // Deserialize message field [test]
    data.test = _deserializer.string(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += object.test.length;
    return length + 4;
  }

  static datatype() {
    // Returns string type for a message object
    return 'race/test';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '8d4693bd9512b94755470aae7830e048';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    string test
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new test(null);
    if (msg.test !== undefined) {
      resolved.test = msg.test;
    }
    else {
      resolved.test = ''
    }

    return resolved;
    }
};

module.exports = test;
