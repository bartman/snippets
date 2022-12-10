#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "message_generated.h"

struct Message {
	int x;
	int y;
	int z;
};

static void print_message(struct Message *m)
{
	printf("{ x=%d, y=%d, z=%d }\n", m->x, m->y, m->z);
}

int main(void)
{
	/* this is our flat structure */

	struct Message m = {0x11111111, 0x22222222, 0x33333333};

	print_message(&m);
	printf("sizeof(Message) = %zu\n", sizeof(m));

	/* serialize the message */

	flatcc_builder_t builder;
	flatcc_builder_init(&builder);

	Flat_Message_start_as_root(&builder);
	Flat_Message_x_add(&builder, m.x);
	Flat_Message_y_add(&builder, m.y);
	Flat_Message_z_add(&builder, m.z);
	Flat_Message_end_as_root(&builder);

	size_t ser_size;
	void *ser_ptr = flatcc_builder_get_direct_buffer(&builder, &ser_size);

	/* dump serialized stream */

	printf("serialized size = %zu\n", ser_size);

	for (size_t i=0; i<ser_size; i++) {
		printf("%s%02hhx", i?" ":"", ((uint8_t*)ser_ptr)[i]);
	}
	puts("");

	/* deserialize */

#if 0
	auto fm = Flat::GetMessage(serialized.data());

	std::cout << "deserialized " << fm << std::endl;
#endif

	return 0;
}
