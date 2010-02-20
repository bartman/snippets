#ifdef __KERNEL__
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/vmalloc.h>
#include <linux/zlib.h>
#include <linux/zutil.h>
#include <linux/crypto.h>
#define PRINT printk
#else
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>
#define PRINT printf
#endif

#ifndef __KERNEL__
/* user api needs allocation wrappers */
static void* my_alloc (void *o, unsigned items, unsigned size)
{
	return calloc (items, size);
}

static void my_free (void *o, void *ptr)
{
	free (ptr);
}
#define zlib_inflateInit2 inflateInit2
#define zlib_inflate      inflate
#define zlib_inflateEnd   inflateEnd
#endif


/*
 * this is the compressed input
 */
#define INPUT_LEN 315
static uint8_t input_data[INPUT_LEN] = {
	0x73, 0x65, 0x60, 0x61, 0x09, 0xac, 0x65, 0x60,
	0x70, 0x60, 0xfc, 0x1b, 0xc4, 0xc5, 0xc5, 0xc5,
	0x0e, 0xc4, 0xfc, 0x0c, 0x0c, 0x17, 0x97, 0x58,
	0xf0, 0x30, 0x30, 0x76, 0xbf, 0xbf, 0xe6, 0xca,
	0xbd, 0x94, 0x99, 0x81, 0x83, 0x93, 0x8b, 0x9b,
	0x87, 0x97, 0x8f, 0x5f, 0x40, 0x50, 0x48, 0x58,
	0x44, 0x54, 0x4c, 0x5c, 0x42, 0x52, 0x4a, 0x5a,
	0x46, 0x56, 0x4e, 0x5e, 0x41, 0x51, 0x49, 0x59,
	0x45, 0x55, 0x4d, 0x5d, 0x43, 0x53, 0x4b, 0x5b,
	0x47, 0x57, 0x4f, 0xdf, 0xc0, 0xd0, 0xc8, 0xd8,
	0xc4, 0xd4, 0xcc, 0xdc, 0xc2, 0xd2, 0xca, 0xda,
	0xc6, 0xd6, 0xce, 0xde, 0xc1, 0xd1, 0xc9, 0xd9,
	0xc5, 0xd5, 0xcd, 0xdd, 0xc3, 0xd3, 0xcb, 0xdb,
	0xc7, 0xd7, 0xcf, 0x3f, 0x20, 0x30, 0x28, 0x38,
	0x24, 0x34, 0x2c, 0x3c, 0x22, 0x32, 0x2a, 0x3a,
	0x26, 0x36, 0x2e, 0x3e, 0x21, 0x31, 0x29, 0x39,
	0x25, 0x35, 0x2d, 0x3d, 0x23, 0x33, 0x2b, 0x3b,
	0x27, 0x37, 0x2f, 0xbf, 0xa0, 0xb0, 0xa8, 0xb8,
	0xa4, 0xb4, 0xac, 0xbc, 0xa2, 0xb2, 0xaa, 0xba,
	0xa6, 0xb6, 0xae, 0xbe, 0xa1, 0xb1, 0xa9, 0xb9,
	0xa5, 0xb5, 0xad, 0xbd, 0xa3, 0xb3, 0xab, 0xbb,
	0xa7, 0xb7, 0xaf, 0x7f, 0xc2, 0xc4, 0x49, 0x93,
	0xa7, 0x4c, 0x9d, 0x36, 0x7d, 0xc6, 0xcc, 0x59,
	0xb3, 0xe7, 0xcc, 0x9d, 0x37, 0x7f, 0xc1, 0xc2,
	0x45, 0x8b, 0x97, 0x2c, 0x5d, 0xb6, 0x7c, 0xc5,
	0xca, 0x55, 0xab, 0xd7, 0xac, 0x5d, 0xb7, 0x7e,
	0xc3, 0xc6, 0x4d, 0x9b, 0xb7, 0x6c, 0xdd, 0xb6,
	0x7d, 0xc7, 0xce, 0x5d, 0xbb, 0xf7, 0xec, 0xdd,
	0xb7, 0xff, 0xc0, 0xc1, 0x43, 0x87, 0x8f, 0x1c,
	0x3d, 0x76, 0xfc, 0xc4, 0xc9, 0x53, 0xa7, 0xcf,
	0x9c, 0x3d, 0x77, 0xfe, 0xc2, 0xc5, 0x4b, 0x97,
	0xaf, 0x5c, 0xbd, 0x76, 0xfd, 0xc6, 0xcd, 0x5b,
	0xb7, 0xef, 0xdc, 0xbd, 0x77, 0xff, 0xc1, 0xc3,
	0x47, 0x8f, 0x9f, 0x3c, 0x7d, 0xf6, 0xfc, 0xc5,
	0xcb, 0x57, 0xaf, 0xdf, 0xbc, 0x7d, 0xf7, 0xfe,
	0xc3, 0xc7, 0x4f, 0x9f, 0xbf, 0x7c, 0xfd, 0xf6,
	0xfd, 0xc7, 0xcf, 0x5f, 0xbf, 0xff, 0xfc, 0xfd,
	0xf7, 0x9f, 0x81, 0x91, 0x89, 0x99, 0x85, 0x95,
	0x8d, 0x7d, 0xd4, 0xff, 0xa3, 0xfe, 0x1f, 0xce,
	0xfe, 0x07, 0x00 };

/*
 * this is the expected deflated output
 */
#define GOOD_LEN 1028
static uint8_t good_data[GOOD_LEN] = {
	0x45, 0x00, 0x04, 0x04, 0x51, 0x7d, 0x00, 0x00,
	0x40, 0x01, 0xfd, 0x52, 0x0a, 0x0a, 0x0a, 0x07,
	0x0a, 0x0a, 0x0a, 0x0f, 0x00, 0x00, 0xd1, 0xa4,
	0x38, 0x0c, 0x00, 0x01, 0x8b, 0xef, 0xd6, 0x45,
	0x0b, 0xa5, 0x03, 0x00, 0x08, 0x09, 0x0a, 0x0b,
	0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13,
	0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b,
	0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23,
	0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b,
	0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33,
	0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b,
	0x3c, 0x3d, 0x3e, 0x3f, 0x40, 0x41, 0x42, 0x43,
	0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b,
	0x4c, 0x4d, 0x4e, 0x4f, 0x50, 0x51, 0x52, 0x53,
	0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x5b,
	0x5c, 0x5d, 0x5e, 0x5f, 0x60, 0x61, 0x62, 0x63,
	0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b,
	0x6c, 0x6d, 0x6e, 0x6f, 0x70, 0x71, 0x72, 0x73,
	0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x7b,
	0x7c, 0x7d, 0x7e, 0x7f, 0x80, 0x81, 0x82, 0x83,
	0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b,
	0x8c, 0x8d, 0x8e, 0x8f, 0x90, 0x91, 0x92, 0x93,
	0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9a, 0x9b,
	0x9c, 0x9d, 0x9e, 0x9f, 0xa0, 0xa1, 0xa2, 0xa3,
	0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab,
	0xac, 0xad, 0xae, 0xaf, 0xb0, 0xb1, 0xb2, 0xb3,
	0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xbb,
	0xbc, 0xbd, 0xbe, 0xbf, 0xc0, 0xc1, 0xc2, 0xc3,
	0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb,
	0xcc, 0xcd, 0xce, 0xcf, 0xd0, 0xd1, 0xd2, 0xd3,
	0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xdb,
	0xdc, 0xdd, 0xde, 0xdf, 0xe0, 0xe1, 0xe2, 0xe3,
	0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xeb,
	0xec, 0xed, 0xee, 0xef, 0xf0, 0xf1, 0xf2, 0xf3,
	0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb,
	0xfc, 0xfd, 0xfe, 0xff, 0x00, 0x01, 0x02, 0x03,
	0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b,
	0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13,
	0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b,
	0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23,
	0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b,
	0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33,
	0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b,
	0x3c, 0x3d, 0x3e, 0x3f, 0x40, 0x41, 0x42, 0x43,
	0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b,
	0x4c, 0x4d, 0x4e, 0x4f, 0x50, 0x51, 0x52, 0x53,
	0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x5b,
	0x5c, 0x5d, 0x5e, 0x5f, 0x60, 0x61, 0x62, 0x63,
	0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b,
	0x6c, 0x6d, 0x6e, 0x6f, 0x70, 0x71, 0x72, 0x73,
	0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x7b,
	0x7c, 0x7d, 0x7e, 0x7f, 0x80, 0x81, 0x82, 0x83,
	0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b,
	0x8c, 0x8d, 0x8e, 0x8f, 0x90, 0x91, 0x92, 0x93,
	0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9a, 0x9b,
	0x9c, 0x9d, 0x9e, 0x9f, 0xa0, 0xa1, 0xa2, 0xa3,
	0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab,
	0xac, 0xad, 0xae, 0xaf, 0xb0, 0xb1, 0xb2, 0xb3,
	0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xbb,
	0xbc, 0xbd, 0xbe, 0xbf, 0xc0, 0xc1, 0xc2, 0xc3,
	0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb,
	0xcc, 0xcd, 0xce, 0xcf, 0xd0, 0xd1, 0xd2, 0xd3,
	0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xdb,
	0xdc, 0xdd, 0xde, 0xdf, 0xe0, 0xe1, 0xe2, 0xe3,
	0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xeb,
	0xec, 0xed, 0xee, 0xef, 0xf0, 0xf1, 0xf2, 0xf3,
	0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb,
	0xfc, 0xfd, 0xfe, 0xff, 0x00, 0x01, 0x02, 0x03,
	0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b,
	0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13,
	0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b,
	0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23,
	0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b,
	0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33,
	0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b,
	0x3c, 0x3d, 0x3e, 0x3f, 0x40, 0x41, 0x42, 0x43,
	0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b,
	0x4c, 0x4d, 0x4e, 0x4f, 0x50, 0x51, 0x52, 0x53,
	0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x5b,
	0x5c, 0x5d, 0x5e, 0x5f, 0x60, 0x61, 0x62, 0x63,
	0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b,
	0x6c, 0x6d, 0x6e, 0x6f, 0x70, 0x71, 0x72, 0x73,
	0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x7b,
	0x7c, 0x7d, 0x7e, 0x7f, 0x80, 0x81, 0x82, 0x83,
	0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b,
	0x8c, 0x8d, 0x8e, 0x8f, 0x90, 0x91, 0x92, 0x93,
	0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9a, 0x9b,
	0x9c, 0x9d, 0x9e, 0x9f, 0xa0, 0xa1, 0xa2, 0xa3,
	0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab,
	0xac, 0xad, 0xae, 0xaf, 0xb0, 0xb1, 0xb2, 0xb3,
	0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xbb,
	0xbc, 0xbd, 0xbe, 0xbf, 0xc0, 0xc1, 0xc2, 0xc3,
	0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb,
	0xcc, 0xcd, 0xce, 0xcf, 0xd0, 0xd1, 0xd2, 0xd3,
	0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xdb,
	0xdc, 0xdd, 0xde, 0xdf, 0xe0, 0xe1, 0xe2, 0xe3,
	0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xeb,
	0xec, 0xed, 0xee, 0xef, 0xf0, 0xf1, 0xf2, 0xf3,
	0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb,
	0xfc, 0xfd, 0xfe, 0xff, 0x00, 0x01, 0x02, 0x03,
	0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b,
	0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13,
	0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b,
	0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23,
	0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b,
	0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33,
	0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b,
	0x3c, 0x3d, 0x3e, 0x3f, 0x40, 0x41, 0x42, 0x43,
	0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b,
	0x4c, 0x4d, 0x4e, 0x4f, 0x50, 0x51, 0x52, 0x53,
	0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x5b,
	0x5c, 0x5d, 0x5e, 0x5f, 0x60, 0x61, 0x62, 0x63,
	0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b,
	0x6c, 0x6d, 0x6e, 0x6f, 0x70, 0x71, 0x72, 0x73,
	0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x7b,
	0x7c, 0x7d, 0x7e, 0x7f, 0x80, 0x81, 0x82, 0x83,
	0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b,
	0x8c, 0x8d, 0x8e, 0x8f, 0x90, 0x91, 0x92, 0x93,
	0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9a, 0x9b,
	0x9c, 0x9d, 0x9e, 0x9f, 0xa0, 0xa1, 0xa2, 0xa3,
	0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab,
	0xac, 0xad, 0xae, 0xaf, 0xb0, 0xb1, 0xb2, 0xb3,
	0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xbb,
	0xbc, 0xbd, 0xbe, 0xbf, 0xc0, 0xc1, 0xc2, 0xc3,
	0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb,
	0xcc, 0xcd, 0xce, 0xcf, 0xd0, 0xd1, 0xd2, 0xd3,
	0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xdb,
	0xdc, 0xdd, 0xde, 0xdf, 0xe0, 0xe1, 0xe2, 0xe3,
	0xe4, 0xe5, 0xe6, 0xe7 };

/*
 * buffer for output
 */
#define OUTPUT_LEN 2048
static uint8_t output_data[OUTPUT_LEN];

static void dump_zs(z_stream *zs)
{
	PRINT ("    zs.next_in %p (+%ld)\n",
			zs->next_in, 
			(long)(zs->next_in - input_data));
	PRINT ("    zs.avail_in %d\n",
			zs->avail_in);
	PRINT ("    zs.next_out %p (+%ld)\n",
			zs->next_out, 
			(long)(zs->next_out - output_data));
	PRINT ("    zs.avail_out %d\n",
			zs->avail_out);
}

#ifdef __KERNEL__
static int __init ztry_init (void)
#else
int main (void)
#endif
{
	int rc, rc2, error;
	unsigned output_used;
	z_stream zs;
#ifdef __KERNEL__
	struct crypto_tfm *tfm;
#endif

	memset (&zs, 0, sizeof(zs));
	memset (output_data, -1, OUTPUT_LEN);

#ifdef __KERNEL__
	printk ("--------------- zlib_inflate test ---------------\n");

	error = -ENOMEM;
	zs.workspace = vmalloc (zlib_inflate_workspacesize());
	if (!zs.workspace)
		goto done_zlib;

	error = -EINVAL;
#else
	zs.zalloc = my_alloc;
	zs.zfree = my_free;
	zs.opaque = 0;

	error = -1;
#endif

	zs.next_in = input_data;
	zs.avail_in = INPUT_LEN;

	zs.next_out = output_data;
	zs.avail_out = OUTPUT_LEN;

	rc = zlib_inflateInit2 (&zs, -11);
	PRINT ("inflateInit2 returns %d\n", rc);
	if (rc != Z_OK)
		goto cleanup_zlib;

	PRINT ("ready to go\n");
	dump_zs(&zs);

	rc = zlib_inflate(&zs, Z_SYNC_FLUSH);
	PRINT ("inflate(Z_SYNC_FLUSH) returns %d\n", rc);
	dump_zs(&zs);

	if (rc == Z_OK && !zs.avail_in && zs.avail_out) {
		uint8_t zerostuff = 0;
		zs.next_in = &zerostuff;
		zs.avail_in = 1;
		rc = zlib_inflate(&zs, Z_FINISH);
		PRINT ("inflate(Z_FINISH) returns %d\n", rc);
		dump_zs(&zs);
	}

	if (rc != Z_STREAM_END) {
		if (rc != Z_OK)
			goto cleanup_zlib;
		PRINT ("!!! rc is not Z_STREAM_END as expected, it's Z_OK\n");
	}

	output_used = OUTPUT_LEN - zs.avail_out;

	PRINT ("output of %d bytes\n", output_used);
	if (output_used != GOOD_LEN) {
		int min = output_used < GOOD_LEN ? output_used : GOOD_LEN;
		PRINT ("!!! expected %d bytes\n", GOOD_LEN);
		PRINT ("first %d bytes %smatch\n", min,
				memcmp (output_data, good_data, min)
				? "don't " : "");
		goto cleanup_zlib;
	}

	if (memcmp (output_data, good_data, GOOD_LEN)) {
		PRINT ("buffers differ\n");
		goto cleanup_zlib;
	}

	PRINT ("zlib success\n");

	error = 0;
cleanup_zlib:
	rc2 = zlib_inflateEnd (&zs);
	PRINT ("inflateEnd returns %d\n", rc2);
	dump_zs(&zs);
#ifdef __KERNEL__
	vfree (zs.workspace);
done_zlib:
	printk ("--------------- cryptoapi test ---------------\n");

	rc = -ENOENT;
	tfm = crypto_alloc_tfm ("deflate", 0);
	if (!tfm) {
		PRINT ("could not get tfm for 'deflate'\n");
		goto done_capi;
	}

	PRINT ("running crypto_comp_decompress...\n");
	output_used = OUTPUT_LEN;
	rc = crypto_comp_decompress (tfm, input_data, INPUT_LEN, 
			output_data, &output_used);

	PRINT ("crypto_comp_decompress returned %d, output_used=%d/%d\n", 
			rc, output_used, OUTPUT_LEN);
	if (output_used != GOOD_LEN) {
		int min = output_used < GOOD_LEN ? output_used : GOOD_LEN;
		PRINT ("!!! expected %d bytes\n", GOOD_LEN);
		PRINT ("first %d bytes %smatch\n", min,
				memcmp (output_data, good_data, min)
				? "don't " : "");
		goto cleanup_capi;
	}

	if (memcmp (output_data, good_data, GOOD_LEN)) {
		PRINT ("buffers differ\n");
		goto cleanup_capi;
	}

	PRINT ("capi success\n");

cleanup_capi:
	crypto_free_tfm (tfm);
done_capi:
#endif

	return error;
}

#ifdef __KERNEL__
static void __exit ztry_exit (void)
{
}
module_init (ztry_init);
module_exit (ztry_exit);
MODULE_LICENSE("GPL");
#endif
