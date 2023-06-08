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


    /// @brief A general purpose basic serializer and deserializer for any instantiable data type.
    /// @tparam T the data type to serialize and deserialize
    template<typename T>
    class Serializer{

        public:

        /// @brief a constant that is used to identify the beginning of a serialized object
        uint8_t  prefix = 'a';
        /// @brief a constant that is specific to the type of object being serialized
        uint8_t  id;
        T        object;
        uint8_t  suffix = 'z';
        uint16_t checksum;

        /// @brief Creates a Serializer that can deserialize and serialize 
        /// objects and immediately serialize "object" with a call to "serialize"
        /// @param object an object to serialize
        /// @param id the unique ID of this Serializer. Make sure it equals the ID of the receieving deserializer.
        Serializer(const T &object, uint8_t id) : id(id), object(object){
            checksum = compute_checksum();
        }

        /// @brief Creates a Serializer that can deserialize and serialize objects
        /// @param id 
        Serializer(uint8_t id) : id(id), object(T()){
            checksum = compute_checksum();
        }
        
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
        void serialize(Return_Type (*send_byte)(uint8_t)){
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
            send_byte(checksum);    // lsb
            send_byte(checksum>>8); // msb
        }


        /// @brief Sends the object over a byte stream by repeatedly calling the "send_byte" function
        /// @tparam Return_Type the return type of the send_byte function
        /// @param send_byte a function that sends a byte over a stream of some sort
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
        void serialize(Object_type &send_object, Return_Type (Object_type::*send_byte)(uint8_t)){
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
            (send_object.*send_byte)(checksum);    // lsb
            (send_object.*send_byte)(checksum>>8); // msb
        }

        /// @brief Repeatedly call this function to receive the object over a byte stream.
        /// @param incoming_byte the byte to receive
        /// @param write_object a pointer to the object to write the received object to (optional)
        /// @return a pointer to the object if the object has been received, or a null 
        /// pointer if the object has not been received or was corrupted in transit.
        bool deserialize(uint8_t incoming_byte, T *destination_object = nullptr){
            enum ParseStage { stage_prefix, stage_id, stage_object, stage_suffix, stage_checksum, stage_checksum_2 };
            static ParseStage stage = stage_prefix;
            static uint8_t * ptr = (uint8_t *) &object;

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
                    }               
                    else{                                       // If it doesn't match
                        stage = stage_prefix;                   //    Go back to the first stage. This isn't the packet we're looking for.
                    }               
                    break;               
                case stage_object:                              // We expect the object data itself
                    *(ptr++) = incoming_byte;                      // Store the byte in the object and increment the pointer
                    if(ptr >= (uint8_t *) &object+sizeof(T)){   // If we've received all the bytes
                        ptr = (uint8_t *) & object;             //    Reset the pointer
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
                    if(checksum == compute_checksum()){         //    If the checksum matches           
                        if(destination_object){                 //        If we have a valid pointer to an object
                            *destination_object = object;       //            Write the object to the given address
                        }
                        return true;                            //        Return true
                    }
                    break;
            } // end of switch
            return false;                                       // If we get here, we haven't received a proper object yet
        } // end of deserialize





    #ifdef ARDUINO
    /// @brief (TODO test) Prints the object to the given serial port (or other Print object)
    /// @param serial the Print object use to print the seralizedobject
    bool
    printTo(Print &serial) const
    {
        if(!serial) return false;
        serialize_instance.serialize<Stream, void>(Serial, &Stream::write);
    } // end printTo

    /// @brief (TODO test) Reads the object from the given serial port (or other Stream object), blocking until the object is received.
    /// @param serial the Stream object to read the serialized object from
    /// @return a pointer to the object if the object, or a null pointer if the Stream is not available for some reason.
    T*
    readFrom(Stream &serial)
    {
        T *t = nullptr;
        while(serial.available() && !t){
            t = deserialize(serial.read);
        }
    } // end readFrom


    /// @brief (TODO test) Attempts to read the object from the given serial port (or other Stream object)
    /// @param serial the Stream object to read the serialized object from
    /// @return a pointer to the object if the object; or a null pointer if the Stream is not 
    /// available for some reason, or if the object is corrupted or otherwise not yet available.
    T*
    readFromNonBlocking(Stream &serial)
    {
        T *t = nullptr;
        if(!serial.available()){
            return nullptr;
        }
        t = deserialize(serial.read);
    } // end readFrom


    #endif



        /// @brief Sets the object to the given value and recomputes the checksum
        /// @param object 
        void operator =(const T &new_value){
            object = new_value;
            checksum = compute_checksum();
        }

        



        private:

        uint16_t compute_checksum(){
            uint16_t checksum = 0;
            uint8_t * ptr = (uint8_t *) &object;
            for(int i = 0; i < sizeof(T); i++){
                checksum += ptr[i];
            }
            return checksum;
        } // end of compute_checksum
    }; // end of Serializer class

    uint8_t create_unique_id(){
        static uint8_t id = 1;
        return id++;
        #ifdef USING_STANDARD_LIBRARY
        if(id == 0){
            throw std::runtime_error("Too many objects have been serialized. The maximum number of unique objects is 255.");
        }
        #endif
    }


}