#include <iostream>
#include <ostream>
#include <vector>

#include "flatbuffers/flatbuffers.h"
#include "message_generated.h"

std::ostream& operator<<(std::ostream& o, const Flat::Message& m) {
	return o << "(x=" << m.x() << ", y=" << m.y() << ")";
}

struct Message {
	int x;
	int y;
	Message(int _x, int _y) : x(_x), y(_y) {}
	~Message() {}
};

std::ostream& operator<<(std::ostream& o, const Message& m) {
	return o << "(x=" << m.x << ", y=" << m.y << ")";
}

int main(void)
{
	/* this is our flat structure */

	Message m(0x1111, 0x2222);

	std::cout << m << std::endl;
	std::cout << "message size = " << sizeof(m) << std::endl;

	/* serialize the message */

	flatbuffers::FlatBufferBuilder builder(1024);
	auto offset = Flat::CreateMessage(builder, m.x, m.y);
	builder.Finish(offset);

	std::vector<uint8_t> serialized(builder.GetBufferPointer(),
					builder.GetBufferPointer() + builder.GetSize());

	/* dump serialized stream */

	std::cout << "serialized.size() = " << serialized.size() << std::endl;

	for (auto it : serialized) {
		std::cout << " "
			<< std::hex << std::setw(2) << std::setfill('0')
			<< (int)it;
	}
	std::cout << std::endl;

	return 0;
}
