#include <gtest/gtest.h>
#include "Serializer.hpp"
#include <queue>
#include <cstdint>
#include <string.h>

// Setup a minimal reproducible example of buffers that can be used to test the Serializer
//     Broadcaster -> Broadcaster::buffer -> Broadcaster::receive() -> Receiver::receive()

class Broadcaster{
  std::queue<uint8_t> buffer;
  public:
  void send(uint8_t data){
    buffer.push(data);
  }
  int16_t receive(){
    if(buffer.empty()) return -1;
    uint8_t data = buffer.front();
    buffer.pop();
    return data;
  }
};

class Receiver{
  private:
  Broadcaster &broadcaster;
  public:
  Receiver(Broadcaster &broadcaster): broadcaster(broadcaster){}
  int16_t receive(){
    return broadcaster.receive();
  }
};





// Serialize and deserialize one integer
TEST(SerializerTests, Serialize_Int) {

  // Create the test object
  int a = 0xf005ba11;

  // Make a print stream
  Broadcaster broadcaster;

  // Serialize the object and transmit the message over the print stream
  serializer::Serializer<int> serializer(0);
  serializer.serialize<Broadcaster, void>(a, broadcaster, &Broadcaster::send);

  // Receive the message
  Receiver receiver(broadcaster);

  // Deserialize the message
  serializer::Deserializer<int> deserializer(0);
  int a_deserialized = -1;
  while(true){
    int16_t read_result = receiver.receive();
    if(read_result<0) break;
    deserializer.deserialize((uint8_t) read_result, &a_deserialized);
  }
} // end Serialize_Int






// Serialize and deserialize one hundred integers
TEST(SerializerTests, Serialize_Int_100x) {


  // Make a print stream...
  Broadcaster broadcaster;
  // ... and the print stream reader
  Receiver receiver(broadcaster);

  // Create the serializer...
  serializer::Serializer<int> serializer(0);
  // ... and the deserializer
  serializer::Deserializer<int> deserializer(0);

  // Create the test object
  for(int i = 0; i < 100; i++){
      // Serialize the object i and transmit the message over the print stream
      serializer.serialize<Broadcaster, void>(i, broadcaster, &Broadcaster::send);

      int i_deserialized = 0xDEADBEEF;
      while(true){
        int16_t read_result = receiver.receive();
        if(read_result<0) break;
        deserializer.deserialize((uint8_t) read_result, &i_deserialized);
      }

      // Check that the serialized/deserialized object is the same as the original
      ASSERT_EQ(i, i_deserialized);
  }
} // end Serialize_Int_100x











// Serialize and deserialize one integer
TEST(SerializerTests, Serialize_Struct) {

  class TestClass{
    public:
    long data[10];
    TestClass(){
      for(int i = 0; i < 10; i++) data[i] = 0;
    }
    bool operator==(const TestClass &other) const{
      for(int i = 0; i < 10; i++) if (data[i]!=other.data[i]) return false;
      return true;
    }
  };

  // Create the test object
  // Make a test object that can be serialized
  struct Foo{
    int         a;
    float       b;
    long long   c;
    uint8_t     d;
    TestClass   e;
  };

  struct Foo foo;
  foo.a = 0xC001BEA7;
  foo.b = 3.14159265359;
  foo.c = -1;
  foo.d = 42;
  foo.e.data[5] = -512;

  // Make a print stream
  Broadcaster broadcaster;

  // Serialize the object and transmit the message over the print stream
  serializer::Serializer<struct Foo> serializer(0);
  serializer.serialize<Broadcaster, void>(foo, broadcaster, &Broadcaster::send);

  // Receive the message
  Receiver receiver(broadcaster);

  // Deserialize the message
  serializer::Deserializer<struct Foo> deserializer(0);
  struct Foo foo_deserialized;
  while(true){
    int16_t read_result = receiver.receive();
    if(read_result<0) break;
    deserializer.deserialize((uint8_t) read_result, &foo_deserialized);
  }

  // Check that the serialized/deserialized object is the same as the original
  ASSERT_EQ(foo.a, foo_deserialized.a);
  ASSERT_EQ(foo.b, foo_deserialized.b);
  ASSERT_EQ(foo.c, foo_deserialized.c);
  ASSERT_EQ(foo.d, foo_deserialized.d);
  ASSERT_EQ(foo.e, foo_deserialized.e);
} // end Serialize_Int


