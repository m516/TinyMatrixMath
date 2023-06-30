#pragma once

#ifdef USING_STANDARD_LIBRARY
#include <stdexcept>
#include <cstdint>
using uint8_t = std::uint8_t;
#endif

#ifdef ARDUINO
#include <Arduino.h>
#endif






namespace serializer{



    template<typename T>
    uint16_t compute_checksum(const T &object){
        uint16_t checksum = 0;
        uint8_t * ptr = (uint8_t *) &object;
        for(int i = 0; i < sizeof(T); i++){
            checksum += ptr[i];
        }
        return checksum;
    } // end of compute_checksum


    uint8_t create_unique_id();




    /// @brief A general purpose basic serializer for any instantiable data type 
    /// (compatible with the Deserializer class).
    /// @tparam T the data type to serialize
    template<typename T>
    class Serializer{

        public:


        const uint8_t  prefix = 'a';
        const uint8_t  id;
        const uint8_t  suffix = 'z';

        /// @brief Creates a Serializer that serialize objects
        /// @param id the unique ID to give to this Serializer object.
        Serializer(uint8_t id) : id(id){
        };
        
        
        /// @brief Sends the object over a byte stream by repeatedly calling the "send_byte" function
        /// @tparam Return_Type the return type of the send_byte function
        /// @param send_byte a function that sends a byte over a stream of some sort. 
        /// @note 
        /// If using a method of a class (i.e. Class instance; instance.method) invoke it like this:     <br>
        /// ~~~~~~~~~~~~~~~{.cpp}
        /// serialize_instance.serialize<Class, void>(instance, &Class::method);
        /// ~~~~~~~~~~~~~~~
        /// For example, to use Serial.write, serialize like this:                                       <br>
        /// ~~~~~~~~~~~~~~~{.cpp}
        /// serialize_instance.serialize<Stream, void>(Serial, &Stream::write);
        /// ~~~~~~~~~~~~~~~
        template<typename Return_Type>
        void serialize(const T &object, Return_Type (*send_byte)(uint8_t)){
            // Serialization is done in five stages:
            // 1. The prefix
            send_byte(prefix);
            // 2. The id
            send_byte(id);
            // 3. The object
            uint8_t * ptr = (uint8_t *) &object;
            for(int i = 0; i < sizeof(T); i++){
                send_byte(ptr[i]);
            }
            // 4. The suffix
            send_byte(suffix);
            // 5. The checksum
            uint16_t checksum = compute_checksum(object);
            send_byte(checksum);    // lsb
            send_byte(checksum>>8); // msb
        }


        /// @brief Sends the object over a byte stream by repeatedly calling the "send_byte" method 
        /// from an instance of the `Object_type` class
        /// @tparam Object_type a class with a method that accepts one uint8_t parameter
        /// @tparam Return_Type the return type of the send_byte function
        /// @param send_object an instance of Object_type
        /// @param send_byte a method of  that sends a byte over a stream of some sort
        /// @note 
        /// If using a method of a class (i.e. Class instance; instance.method) invoke it like this:     <br>
        /// ~~~~~~~~~~~~~~~{.cpp}
        /// serialize_instance.serialize<Class, void>(instance, &Class::method);
        /// ~~~~~~~~~~~~~~~
        /// For example, to use Serial.write, serialize like this:                                       <br>
        /// ~~~~~~~~~~~~~~~{.cpp}
        /// serialize_instance.serialize<Stream, void>(Serial, &Stream::write);
        /// ~~~~~~~~~~~~~~~
        template<typename Object_type, typename Return_Type>
        void serialize(const T &object, Object_type &send_object, Return_Type (Object_type::*send_byte)(uint8_t)){
            // Serialization is done in five stages:
            // 1. The prefix
            (send_object.*send_byte)(prefix);
            // 2. The id
            (send_object.*send_byte)(id);
            // 3. The object
            uint8_t * ptr = (uint8_t *) &object;
            for(int i = 0; i < sizeof(T); i++){
                (send_object.*send_byte)(ptr[i]);
            }
            // 4. The suffix
            (send_object.*send_byte)(suffix);
            // 5. The checksum
            uint16_t checksum = compute_checksum(object);
            (send_object.*send_byte)(checksum);    // lsb
            (send_object.*send_byte)(checksum>>8); // msb
        }

    }; // end of Serializer class














    /// @brief A general purpose basic serializer and deserializer for any instantiable data type.
    /// @tparam T the data type to serialize and deserialize
    template<typename T>
    class Deserializer{

        public:

        /// @brief a constant that is used to identify the beginning of a serialized object
        const uint8_t  prefix = 'a';
        /// @brief a constant that is specific to the type of object being serialized
        const uint8_t  id;
        T        object;
        const uint8_t  suffix = 'z';
        uint16_t checksum;

        /// @brief Creates a Serializer that can deserialize and serialize objects
        /// @param id 
        Deserializer(uint8_t id) : id(id){
        }

        /// @brief Repeatedly call this function to receive the object over a byte stream.
        /// @param incoming_byte the byte to receive
        /// @param write_object a pointer to the object to write the received object to (optional)
        /// @return a pointer to the object if the object has been received, or a null 
        /// pointer if the object has not been received or was corrupted in transit.
        bool deserialize(uint8_t incoming_byte, T *destination_object = nullptr){
            enum ParseStage { stage_prefix, stage_id, stage_object, stage_suffix, stage_checksum, stage_checksum_2 };
            static ParseStage stage = stage_prefix;
            static uint8_t * ptr; // for writing to the object

            // Serialization is done in five stages:
            // 1. The prefix
            // 2. The id
            // 3. The object
            // 4. The suffix
            // 5. The checksum

            // Therefore, deserialization is implemented in a state machine with five states.
            // If parsing during any stage fails, the state machine resets to the first stage.
            // A valid pointer to the object is returned only if the checksum matches at the 
            // end of the last stage.

            switch(stage){
                case stage_prefix:                              // We expect the prefix
                    if(incoming_byte == prefix){                // If it matches
                        stage = stage_id;                       //     Go to the next stage
                    }               
                    break;               
                case stage_id:                                  // We expect the id
                    if(incoming_byte == id){                    // If it matches
                        stage = stage_object;                   //    Go to the next stage
                        ptr = (uint8_t *) &object;              //    Reset the pointer
                    }               
                    else{                                       // If it doesn't match
                        stage = stage_prefix;                   //    Go back to the first stage. This isn't the packet we're looking for.
                    }               
                    break;               
                case stage_object:                              // We expect the object data itself
                    *(ptr++) = incoming_byte;                      // Store the byte in the object and increment the pointer
                    if(ptr >= (uint8_t *) &object+sizeof(T)){   // If we've received all the bytes
                        stage = stage_suffix;                   //    Go to the next stage
                    }
                    break;
                case stage_suffix:                              // We expect the suffix
                    if(incoming_byte == suffix){                // If it matches
                        stage = stage_checksum;                 //    Go to the next stage
                        checksum = 0;                           //    Reset the checksum. 
                                                                //        It takes a couple of bytes to get the checksum, 
                                                                //        so we'll use the fact that it's zero to determine 
                                                                //        when we've received the first byte of the checksum.
                    }
                    break;
                case stage_checksum:                            // We expect the checksum
                    checksum = incoming_byte;                   //    Store it
                    stage = stage_checksum_2;                   //    Advance to stage_checksum_2
                    break;
                case stage_checksum_2:                          // We expect the most significant byte of the checksum
                    checksum += incoming_byte<<8;               //    Store it
                    stage = stage_prefix;                       //    Go back to the first stage
                    if(checksum == compute_checksum(object)){   //    If the checksum matches           
                        if(destination_object){                 //        If we have a valid pointer to an object
                            *destination_object = object;       //            Write the object to the given address
                        }
                        return true;                            //        Return true
                    }
                    break;
            } // end of switch
            return false;                                       // If we get here, we haven't received a proper object yet
        } // end of deserialize


    }; // end of Deserializer class

}