#pragma once

#ifdef USING_STANDARD_LIBRARY
#include <cstdint>
using uint8_t = std::uint8_t;
#endif

#ifdef ARDUINO
#include <Arduino.h>
#endif






namespace serializer{


    template<typename T>
    class SerialPacket{
        public:




    };

    template<typename T>
    class Serializer{

        public:

        /// @brief Creates a 
        /// @param object 
        /// @param id 
        Serializer(const T &object, uint8_t id) : _id(id), _object(object){
            _checksum = compute_checksum();
        }

        /// @brief Sends the object over a byte stream by repeatedly calling the "send_byte" function
        /// @tparam Return_Type the return type of the send_byte function
        /// @param send_byte a function that sends a byte over a stream of some sort
        template<typename Return_Type>
        void serialize(Return_Type (*send_byte)(uint8_t)){
            // Serialization is done in five stages:
            // 1. The prefix
            send_byte(_prefix);
            // 2. The id
            send_byte(_id);
            // 3. The object
            uint8_t * ptr = (uint8_t *) &_object;
            for(int i = 0; i < sizeof(T); i++){
                send_byte(ptr[i]);
            }
            // 4. The suffix
            send_byte(_suffix);
            // 5. The checksum
            send_byte(_checksum);    // lsb
            send_byte(_checksum>>8); // msb
        }

        /// @brief Repeatedly call this function to receive the object over a byte stream.
        /// @param byte the byte to receive
        /// @return a pointer to the object if the object has been received, or a null 
        /// pointer if the object has not been received or was corrupted in transit.
        T* deserialize(uint8_t byte){
            enum ParseStage { stage_prefix, stage_id, stage_object, stage_suffix, stage_checksum };
            static ParseStage stage = prefix;
            static uint8_t * ptr = (uint8_t *) &_object;

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
                    if(byte == _prefix){                        // If it matches
                        stage = stage_id;                       //     Go to the next stage
                    }               
                    break;               
                case stage_id:                                  // We expect the id
                    if(byte == _prefix){                        // If it matches
                        stage = stage_object;                   //    Go to the next stage
                    }               
                    else{                                       // If it doesn't match
                        stage = stage_prefix;                   //    Go back to the first stage. This isn't the packet we're looking for.
                    }               
                    break;               
                case stage_object:                              // We expect the object data itself
                    ptr++ = byte;                               // Store the byte in the object and increment the pointer
                    if(ptr >= object+sizeof(T)){                // If we've received all the bytes
                        ptr = (uint8_t *) &_object;             //    Reset the pointer
                        stage = stage_suffix;                   //    Go to the next stage
                    }
                    break;
                case stage_suffix:                              // We expect the suffix
                    if(byte == suffix){                         // If it matches
                        stage = stage_checksum;                 //    Go to the next stage
                        checksum = 0;                           //    Reset the checksum. 
                                                                //        It takes a couple of bytes to get the checksum, 
                                                                //        so we'll use the fact that it's zero to determine 
                                                                //        when we've received the first byte of the checksum.
                    }
                    break;
                case checksum:                                  // We expect the checksum
                    if(!checksum){                              // If this is the first byte of the checksum
                        checksum = byte;                        //    Store it
                    }
                    else{                                       // If this is the second byte of the checksum
                        checksum += byte<<8;                    //    Store it
                        stage = prefix;                         //    Go back to the first stage
                        if(checksum == compute_checksum()){     //    If the checksum matches           
                            return &object;                     //        Return a pointer to the object
                        }
                        else{                                   //    If the checksum doesn't match
                            return nullptr;                     //        Return a null pointer
                        }
                    }
                    break;
            } // end of switch
        } // end of deserialize





    #ifdef ARDUINO
    /// @brief (TODO test) Prints the object to the given serial port (or other Print object)
    /// @param serial the Print object use to print the seralizedobject
    bool
    printTo(Print &serial) const
    {
        if(serial.)
        serialize(serial.write);
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
        enable_if_t< operator =(const T &object){
            _object = object;
            _checksum = compute_checksum();
        }


        uint8_t  prefix = 'a';
        uint8_t  id;
        T        object;
        uint8_t  suffix = 'z';
        uint16_t checksum;



        private:

        uint16_t compute_checksum(){
            uint8_t checksum = 0;
            uint8_t * ptr = (uint8_t *) &_object;
            for(int i = 0; i < sizeof(T); i++){
                checksum += ptr[i];
            }
            return checksum;
        } // end of compute_checksum
    }; // end of Serializer class


}