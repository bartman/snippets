#include <iostream>
#include <ostream>
#include <vector>

#include "flatbuffers/flatbuffers.h"
#include "message_generated.h"

std::ostream& operator<<(std::ostream& o, const Flat::Message& m) {
	return o << "(x=" << m.x()
		<< ", y=" << m.y()
		<< ", z=" << m.z()
		<< ")";
}

struct Message {
	int x;
	int y;
	int z;
	Message(int _x, int _y, int _z)
		: x(_x), y(_y), z(_z) {}
	~Message() {}
};

std::ostream& operator<<(std::ostream& o, const Message& m) {
	return o << "(x=" << m.x
		<< ", y=" << m.y
		<< ", z=" << m.z
		<< ")";
}

int main(void)
{
	/* this is our flat structure */

	Message m(0x11111111, 0x22222222, 0x33333333);

	std::cout << m << std::endl;
	std::cout << "message size = " << sizeof(m) << std::endl;

	/* serialize the message */

	flatbuffers::FlatBufferBuilder builder(1024);
	auto offset = Flat::CreateMessage(builder, m.x, m.y, m.z);
	builder.Finish(offset);

	std::vector<uint8_t> serialized(builder.GetBufferPointer(),
					builder.GetBufferPointer() + builder.GetSize());

	/* dump serialized stream */

	std::cout << "serialized.size() = " << serialized.size() << std::endl;

	std::ios_base::fmtflags f( std::cout.flags() );
	for (auto it : serialized) {
		std::cout << " "
			<< std::hex << std::setw(2) << std::setfill('0')
			<< (int)it;
	}
	std::cout.flags( f );
	std::cout << std::endl;

	/* deserialize */

	auto fm = Flat::GetMessage(serialized.data());

	std::cout << "deserialized " << fm << std::endl;


	return 0;
}
