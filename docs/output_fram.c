#include <lely/co/sdev.h>

#define CO_SDEV_STRING(s)	NULL

const struct co_sdev lpc17xx_sdev = {
	.id = 0x02,
	.name = NULL,
	.vendor_name = CO_SDEV_STRING("Lely Industries N.V."),
	.vendor_id = 0x00000360,
	.product_name = NULL,
	.product_code = 0x00000000,
	.revision = 0x00000000,
	.order_code = NULL,
	.baud = 0
		| CO_BAUD_1000,
	.rate = 0,
	.lss = 0,
	.dummy = 0x000000fe,
	.nobj = 22,
	.objs = (const struct co_sobj[]){{
		.name = CO_SDEV_STRING("Device type"),
		.idx = 0x1000,
		.code = CO_OBJECT_VAR,
		.nsub = 1,
		.subs = (const struct co_ssub[]){{
			.name = CO_SDEV_STRING("Device type"),
			.subidx = 0x00,
            .fram_address = 0x000004,
			.type = CO_DEFTYPE_UNSIGNED32,
			.min = { .u32 = CO_UNSIGNED32_MIN },
			.max = { .u32 = CO_UNSIGNED32_MAX },
			.def = { .u32 = CO_UNSIGNED32_MIN },
			.val = { .u32 = CO_UNSIGNED32_MIN },
			.access = CO_ACCESS_RO,
			.pdo_mapping = 0,
			.flags = 0
		}}
	}, {
		.name = CO_SDEV_STRING("Error register"),
		.idx = 0x1001,
		.code = CO_OBJECT_VAR,
		.nsub = 1,
		.subs = (const struct co_ssub[]){{
			.name = CO_SDEV_STRING("Error register"),
			.subidx = 0x00,
            .fram_address = 0x000008,
			.type = CO_DEFTYPE_UNSIGNED8,
			.min = { .u8 = CO_UNSIGNED8_MIN },
			.max = { .u8 = CO_UNSIGNED8_MAX },
			.def = { .u8 = CO_UNSIGNED8_MIN },
			.val = { .u8 = CO_UNSIGNED8_MIN },
			.access = CO_ACCESS_RO,
			.pdo_mapping = 0,
			.flags = 0
		}}
	}, {
		.name = CO_SDEV_STRING("COB-ID time stamp object"),
		.idx = 0x1012,
		.code = CO_OBJECT_VAR,
		.nsub = 1,
		.subs = (const struct co_ssub[]){{
			.name = CO_SDEV_STRING("COB-ID time stamp object"),
			.subidx = 0x00,
            .fram_address = 0x00000C,
			.type = CO_DEFTYPE_UNSIGNED32,
			.min = { .u32 = CO_UNSIGNED32_MIN },
			.max = { .u32 = CO_UNSIGNED32_MAX },
			.def = { .u32 = 0x80000100lu },
			.val = { .u32 = 0x80000100lu },
			.access = CO_ACCESS_RW,
			.pdo_mapping = 0,
			.flags = 0
		}}
	},

	{
			.name = CO_SDEV_STRING("Consumer heartbeat time"),
			.idx = 0x1016,
			.code = CO_OBJECT_ARRAY,
			.nsub = 2,
			.subs = (const struct co_ssub[]){{
				.name = CO_SDEV_STRING("NrOfObjects"),
				.subidx = 0x00,
            .fram_address = 0x000010,
				.type = CO_DEFTYPE_UNSIGNED8,
				.min = { .u8 = CO_UNSIGNED8_MIN },
				.max = { .u8 = CO_UNSIGNED8_MAX },
				.def = { .u8 = 0x01 },
				.val = { .u8 = 0x01 },
				.access = CO_ACCESS_RO,
				.pdo_mapping = 0,
				.flags = 0
			}, {
				.name = CO_SDEV_STRING("Consumer heartbeat time1"),
				.subidx = 0x01,
            .fram_address = 0x000014,
				.type = CO_DEFTYPE_UNSIGNED32,
				.min = { .u32 = CO_UNSIGNED32_MIN },
				.max = { .u32 = CO_UNSIGNED32_MAX },
				.def = { .u32 = CO_UNSIGNED32_MIN },
				.val = { .u32 = CO_UNSIGNED32_MIN },
				.access = CO_ACCESS_RW,
				.pdo_mapping = 0,
				.flags = 0
			}}
		},
	{
		.name = CO_SDEV_STRING("Producer heartbeat time"),
		.idx = 0x1017,
		.code = CO_OBJECT_VAR,
		.nsub = 1,
		.subs = (const struct co_ssub[]){{
			.name = CO_SDEV_STRING("Producer heartbeat time"),
			.subidx = 0x00,
            .fram_address = 0x000018,
			.type = CO_DEFTYPE_UNSIGNED16,
			.min = { .u16 = CO_UNSIGNED16_MIN },
			.max = { .u16 = CO_UNSIGNED16_MAX },
			.def = { .u16 = 0x3E8 },
			.val = { .u16 = 0x3E8 },
			.access = CO_ACCESS_RW,
			.pdo_mapping = 0,
			.flags = 0
		}}
	}, {
		.name = CO_SDEV_STRING("Identity object"),
		.idx = 0x1018,
		.code = CO_OBJECT_RECORD,
		.nsub = 5,
		.subs = (const struct co_ssub[]){{
			.name = CO_SDEV_STRING("Highest sub-index supported"),
			.subidx = 0x00,
            .fram_address = 0x00001C,
			.type = CO_DEFTYPE_UNSIGNED8,
			.min = { .u8 = CO_UNSIGNED8_MIN },
			.max = { .u8 = CO_UNSIGNED8_MAX },
			.def = { .u8 = 0x04 },
			.val = { .u8 = 0x04 },
			.access = CO_ACCESS_CONST,
			.pdo_mapping = 0,
			.flags = 0
		}, {
			.name = CO_SDEV_STRING("Vendor-ID"),
			.subidx = 0x01,
            .fram_address = 0x000020,
			.type = CO_DEFTYPE_UNSIGNED32,
			.min = { .u32 = CO_UNSIGNED32_MIN },
			.max = { .u32 = CO_UNSIGNED32_MAX },
			.def = { .u32 = 0x00000360lu },
			.val = { .u32 = 0x00000360lu },
			.access = CO_ACCESS_RO,
			.pdo_mapping = 0,
			.flags = 0
		}, {
			.name = CO_SDEV_STRING("Product code"),
			.subidx = 0x02,
            .fram_address = 0x000024,
			.type = CO_DEFTYPE_UNSIGNED32,
			.min = { .u32 = CO_UNSIGNED32_MIN },
			.max = { .u32 = CO_UNSIGNED32_MAX },
			.def = { .u32 = CO_UNSIGNED32_MIN },
			.val = { .u32 = CO_UNSIGNED32_MIN },
			.access = CO_ACCESS_RO,
			.pdo_mapping = 0,
			.flags = 0
		}, {
			.name = CO_SDEV_STRING("Revision number"),
			.subidx = 0x03,
            .fram_address = 0x000028,
			.type = CO_DEFTYPE_UNSIGNED32,
			.min = { .u32 = CO_UNSIGNED32_MIN },
			.max = { .u32 = CO_UNSIGNED32_MAX },
			.def = { .u32 = CO_UNSIGNED32_MIN },
			.val = { .u32 = CO_UNSIGNED32_MIN },
			.access = CO_ACCESS_RO,
			.pdo_mapping = 0,
			.flags = 0
		}, {
			.name = CO_SDEV_STRING("Serial number"),
			.subidx = 0x04,
            .fram_address = 0x00002C,
			.type = CO_DEFTYPE_UNSIGNED32,
			.min = { .u32 = CO_UNSIGNED32_MIN },
			.max = { .u32 = CO_UNSIGNED32_MAX },
			.def = { .u32 = CO_UNSIGNED32_MIN },
			.val = { .u32 = CO_UNSIGNED32_MIN },
			.access = CO_ACCESS_RO,
			.pdo_mapping = 0,
			.flags = 0
		}}
	}, {
		.name = CO_SDEV_STRING("NMT startup"),
		.idx = 0x1f80,
		.code = CO_OBJECT_VAR,
		.nsub = 1,
		.subs = (const struct co_ssub[]){{
			.name = CO_SDEV_STRING("NMT startup"),
			.subidx = 0x00,
            .fram_address = 0x000030,
			.type = CO_DEFTYPE_UNSIGNED32,
			.min = { .u32 = CO_UNSIGNED32_MIN },
			.max = { .u32 = CO_UNSIGNED32_MAX },
			.def = { .u32 = CO_UNSIGNED32_MIN },
			.val = { .u32 = 0x00000004lu },
			.access = CO_ACCESS_RW,
			.pdo_mapping = 0,
			.flags = 0
		}}
	},
	/* PDOs */
	{
		.name = CO_SDEV_STRING("RPDO communication parameter"),
		.idx = 0x1400,
		.code = CO_OBJECT_RECORD,
		.nsub = 6,
		.subs = (const struct co_ssub[]){{

			.name = CO_SDEV_STRING("highest sub-index supported"),

			.subidx = 0x00,
            .fram_address = 0x000034,
			.type = CO_DEFTYPE_UNSIGNED8,

			.min = { .u8 = CO_UNSIGNED8_MIN },
			.max = { .u8 = CO_UNSIGNED8_MAX },

			.def = { .u8 = 0x05 },

			.val = { .u8 = 0x05 },
			.access = CO_ACCESS_CONST,
			.pdo_mapping = 0,
			.flags = 0
		}, {

			.name = CO_SDEV_STRING("COB-ID used by RPDO"),

			.subidx = 0x01,
            .fram_address = 0x000038,
			.type = CO_DEFTYPE_UNSIGNED32,

			.min = { .u32 = CO_UNSIGNED32_MIN },
			.max = { .u32 = CO_UNSIGNED32_MAX },

			.def = { .u32 = 0x000002fflu },

			.val = { .u32 = 0x000002fflu },
			.access = CO_ACCESS_RW,
			.pdo_mapping = 0,
			.flags = 0
				| CO_OBJ_FLAGS_DEF_NODEID
				| CO_OBJ_FLAGS_VAL_NODEID
		}, {

			.name = CO_SDEV_STRING("transmission type"),

			.subidx = 0x02,
            .fram_address = 0x00003C,
			.type = CO_DEFTYPE_UNSIGNED8,

			.min = { .u8 = CO_UNSIGNED8_MIN },
			.max = { .u8 = CO_UNSIGNED8_MAX },

			.def = { .u8 = 0x01 },

			.val = { .u8 = 0x01 },
			.access = CO_ACCESS_RW,
			.pdo_mapping = 0,
			.flags = 0
		}, {

			.name = CO_SDEV_STRING("inhibit time"),

			.subidx = 0x03,
            .fram_address = 0x000040,
			.type = CO_DEFTYPE_UNSIGNED16,

			.min = { .u16 = CO_UNSIGNED16_MIN },
			.max = { .u16 = CO_UNSIGNED16_MAX },

			.def = { .u16 = CO_UNSIGNED16_MIN },

			.val = { .u16 = CO_UNSIGNED16_MIN },
			.access = CO_ACCESS_RW,
			.pdo_mapping = 0,
			.flags = 0
		}, {

			.name = CO_SDEV_STRING("compatibility entry"),

			.subidx = 0x04,
            .fram_address = 0x000044,
			.type = CO_DEFTYPE_UNSIGNED8,

			.min = { .u8 = CO_UNSIGNED8_MIN },
			.max = { .u8 = CO_UNSIGNED8_MAX },

			.def = { .u8 = CO_UNSIGNED8_MIN },

			.val = { .u8 = CO_UNSIGNED8_MIN },
			.access = CO_ACCESS_RW,
			.pdo_mapping = 0,
			.flags = 0
		}, {

			.name = CO_SDEV_STRING("event-timer"),

			.subidx = 0x05,
            .fram_address = 0x000048,
			.type = CO_DEFTYPE_UNSIGNED16,

			.min = { .u16 = CO_UNSIGNED16_MIN },
			.max = { .u16 = CO_UNSIGNED16_MAX },

			.def = { .u16 = CO_UNSIGNED16_MIN },

			.val = { .u16 = CO_UNSIGNED16_MIN },
			.access = CO_ACCESS_RW,
			.pdo_mapping = 0,
			.flags = 0
		}}
	}, {

		.name = CO_SDEV_STRING("RPDO mapping parameter"),

		.idx = 0x1600,
		.code = CO_OBJECT_ARRAY,
		.nsub = 2,
		.subs = (const struct co_ssub[]){{

			.name = CO_SDEV_STRING("NrOfObjects"),

			.subidx = 0x00,
            .fram_address = 0x00004C,
			.type = CO_DEFTYPE_UNSIGNED8,

			.min = { .u8 = CO_UNSIGNED8_MIN },
			.max = { .u8 = CO_UNSIGNED8_MAX },

			.def = { .u8 = 0x01 },

			.val = { .u8 = 0x01 },
			.access = CO_ACCESS_RO,
			.pdo_mapping = 0,
			.flags = 0
		}, {

			.name = CO_SDEV_STRING("RPDO mapping parameter1"),

			.subidx = 0x01,
            .fram_address = 0x000050,
			.type = CO_DEFTYPE_UNSIGNED32,

			.min = { .u32 = CO_UNSIGNED32_MIN },
			.max = { .u32 = CO_UNSIGNED32_MAX },

			.def = { .u32 = CO_UNSIGNED32_MIN },

			.val = { .u32 = 0x60400020lu }, // receives 0x4000
			.access = CO_ACCESS_RW,
			.pdo_mapping = 0,
			.flags = 0
		}}
	}, {

		.name = CO_SDEV_STRING("TPDO communication parameter"),
		.idx = 0x1800,
		.code = CO_OBJECT_RECORD,
		.nsub = 7,
		.subs = (const struct co_ssub[]){{
			.name = CO_SDEV_STRING("highest sub-index supported"),
			.subidx = 0x00,
            .fram_address = 0x000054,
			.type = CO_DEFTYPE_UNSIGNED8,
			.min = { .u8 = CO_UNSIGNED8_MIN },
			.max = { .u8 = CO_UNSIGNED8_MAX },
			.def = { .u8 = 0x06 },
			.val = { .u8 = 0x06 },
			.access = CO_ACCESS_CONST,
			.pdo_mapping = 0,
			.flags = 0
		}, {

			.name = CO_SDEV_STRING("COB-ID used by TPDO"),
			.subidx = 0x01,
            .fram_address = 0x000058,
			.type = CO_DEFTYPE_UNSIGNED32,
			.min = { .u32 = CO_UNSIGNED32_MIN },
			.max = { .u32 = CO_UNSIGNED32_MAX },
			.def = { .u32 = 0x00000182lu },
			.val = { .u32 = 0x00000182lu },
			.access = CO_ACCESS_RW,
			.pdo_mapping = 0,
			.flags = 0
				| CO_OBJ_FLAGS_DEF_NODEID
				| CO_OBJ_FLAGS_VAL_NODEID
		}, {
			.name = CO_SDEV_STRING("transmission type"),
			.subidx = 0x02,
            .fram_address = 0x00005C,
			.type = CO_DEFTYPE_UNSIGNED8,
			.min = { .u8 = CO_UNSIGNED8_MIN },
			.max = { .u8 = CO_UNSIGNED8_MAX },
			.def = { .u8 = 0x01 },
			.val = { .u8 = 0x01 },
			.access = CO_ACCESS_RW,
			.pdo_mapping = 0,
			.flags = 0
		}, {
			.name = CO_SDEV_STRING("inhibit time"),
			.subidx = 0x03,
            .fram_address = 0x000060,
			.type = CO_DEFTYPE_UNSIGNED16,
			.min = { .u16 = CO_UNSIGNED16_MIN },
			.max = { .u16 = CO_UNSIGNED16_MAX },
			.def = { .u16 = CO_UNSIGNED16_MIN },
			.val = { .u16 = CO_UNSIGNED16_MIN },
			.access = CO_ACCESS_RW,
			.pdo_mapping = 0,
			.flags = 0
		}, {
			.name = CO_SDEV_STRING("reserved"),
			.subidx = 0x04,
            .fram_address = 0x000064,
			.type = CO_DEFTYPE_UNSIGNED8,
			.min = { .u8 = CO_UNSIGNED8_MIN },
			.max = { .u8 = CO_UNSIGNED8_MAX },
			.def = { .u8 = CO_UNSIGNED8_MIN },
			.val = { .u8 = CO_UNSIGNED8_MIN },
			.access = CO_ACCESS_RW,
			.pdo_mapping = 0,
			.flags = 0
		}, {
			.name = CO_SDEV_STRING("event timer"),
			.subidx = 0x05,
            .fram_address = 0x000068,
			.type = CO_DEFTYPE_UNSIGNED16,
			.min = { .u16 = CO_UNSIGNED16_MIN },
			.max = { .u16 = CO_UNSIGNED16_MAX },
			.def = { .u16 = CO_UNSIGNED16_MIN },
			.val = { .u16 = CO_UNSIGNED16_MIN },
			.access = CO_ACCESS_RW,
			.pdo_mapping = 0,
			.flags = 0
		}, {
			.name = CO_SDEV_STRING("SYNC start value"),
			.subidx = 0x06,
            .fram_address = 0x00006C,
			.type = CO_DEFTYPE_UNSIGNED8,
			.min = { .u8 = CO_UNSIGNED8_MIN },
			.max = { .u8 = CO_UNSIGNED8_MAX },
			.def = { .u8 = CO_UNSIGNED8_MIN },

			.val = { .u8 = CO_UNSIGNED8_MIN },
			.access = CO_ACCESS_RW,
			.pdo_mapping = 0,
			.flags = 0
		}}
	}, {
		.name = CO_SDEV_STRING("TPDO mapping parameter"),
		.idx = 0x1a00,
		.code = CO_OBJECT_ARRAY,
		.nsub = 2,
		.subs = (const struct co_ssub[]){{
			.name = CO_SDEV_STRING("NrOfObjects"),
			.subidx = 0x00,
            .fram_address = 0x000070,
			.type = CO_DEFTYPE_UNSIGNED8,
			.min = { .u8 = CO_UNSIGNED8_MIN },
			.max = { .u8 = CO_UNSIGNED8_MAX },
			.def = { .u8 = 0x01 },
			.val = { .u8 = 0x01 },
			.access = CO_ACCESS_RO,
			.pdo_mapping = 0,
			.flags = 0
		}, {
			.name = CO_SDEV_STRING("TPDO mapping parameter1"),
			.subidx = 0x01,
            .fram_address = 0x000074,
			.type = CO_DEFTYPE_UNSIGNED32,
			.min = { .u32 = CO_UNSIGNED32_MIN },
			.max = { .u32 = CO_UNSIGNED32_MAX },
			.def = { .u32 = CO_UNSIGNED32_MIN },
			.val = { .u32 = 0x60410020lu }, // sends 0x4001:00 32 bit lenght
			.access = CO_ACCESS_RW,
			.pdo_mapping = 0,
			.flags = 0
		}}
	},
	/* Objects */
	{
		.name = CO_SDEV_STRING("Object with custom SDO download callback"),
		.idx = 0x2000,
		.code = CO_OBJECT_VAR,
		.nsub = 1,
		.subs = (const struct co_ssub[]){{
			.name = CO_SDEV_STRING("Object with custom SDO download callback"),
			.subidx = 0x00,
            .fram_address = 0x000078,
			.type = CO_DEFTYPE_UNSIGNED32,
			.min = { .u32 = CO_UNSIGNED32_MIN },
			.max = { .u32 = CO_UNSIGNED32_MAX },
			.def = { .u32 = CO_UNSIGNED32_MIN },
			.val = { .u32 = CO_UNSIGNED32_MIN },
			.access = CO_ACCESS_RW,
			.pdo_mapping = 0,
			.flags = 0
		}}
	}, {
		.name = CO_SDEV_STRING("Object with custom SDO upload callback"),
		.idx = 0x2001,
		.code = CO_OBJECT_VAR,
		.nsub = 1,
		.subs = (const struct co_ssub[]){{
			.name = CO_SDEV_STRING("Object with custom SDO upload callback"),
			.subidx = 0x00,
            .fram_address = 0x00007C,
			.type = CO_DEFTYPE_UNSIGNED32,
			.min = { .u32 = CO_UNSIGNED32_MIN },
			.max = { .u32 = CO_UNSIGNED32_MAX },
			.def = { .u32 = CO_UNSIGNED32_MIN },
			.val = { .u32 = CO_UNSIGNED32_MIN },
			.access = CO_ACCESS_RO,
			.pdo_mapping = 0,
			.flags = 0
		}}
	},
	/* Custom Objecs */

	{
		.name = CO_SDEV_STRING("Controlword"),
		.idx = 0x6040,
		.code = CO_OBJECT_VAR,
		.nsub = 1,
		.subs = (const struct co_ssub[]){{
			.name = CO_SDEV_STRING("Controlword"),
			.subidx = 0x00,
            .fram_address = 0x000080,
			.type = CO_DEFTYPE_UNSIGNED32,
			.min = { .u32 = CO_UNSIGNED32_MIN },
			.max = { .u32 = CO_UNSIGNED32_MAX },
			.def = { .u32 = CO_UNSIGNED32_MIN },
			.val = { .u32 = 0x6 },
			.access = CO_ACCESS_RW,
			.pdo_mapping = 1,
			.flags = 0
		}}
	},
	{
		.name = CO_SDEV_STRING("Statusword"),
		.idx = 0x6041,
		.code = CO_OBJECT_VAR,
		.nsub = 1,
		.subs = (const struct co_ssub[]){{
			.name = CO_SDEV_STRING("Statusword"),
			.subidx = 0x00,
            .fram_address = 0x000084,
			.type = CO_DEFTYPE_UNSIGNED32,
			.min = { .u32 = CO_UNSIGNED32_MIN },
			.max = { .u32 = CO_UNSIGNED32_MAX },
			.def = { .u32 = CO_UNSIGNED32_MIN },
			.val = { .u32 = CO_UNSIGNED32_MIN },
			.access = CO_ACCESS_RW,
			.pdo_mapping = 1,
			.flags = 0
		}}
	},
	{
		.name = CO_SDEV_STRING("Digital Inputs"),
		.idx = 0x60FD,
		.code = CO_OBJECT_VAR,
		.nsub = 1,
		.subs = (const struct co_ssub[]){{
			.name = CO_SDEV_STRING("Digital Inputs"),
			.subidx = 0x00,
            .fram_address = 0x000088,
			.type = CO_DEFTYPE_UNSIGNED32,
			.min = { .u32 = CO_UNSIGNED32_MIN },
			.max = { .u32 = CO_UNSIGNED32_MAX },
			.def = { .u32 = CO_UNSIGNED32_MIN },
			.val = { .u32 = CO_UNSIGNED32_MIN },
			.access = CO_ACCESS_RW,
			.pdo_mapping = 0,
			.flags = 0
		}}
	},
	{
		.name = CO_SDEV_STRING("Homing Method"),
		.idx = 0x6098,
		.code = CO_OBJECT_VAR,
		.nsub = 1,
		.subs = (const struct co_ssub[]){{
			.name = CO_SDEV_STRING("Homing Method"),
			.subidx = 0x00,
            .fram_address = 0x00008C,
			.type = CO_DEFTYPE_UNSIGNED32,
			.min = { .u32 = CO_UNSIGNED32_MIN },
			.max = { .u32 = CO_UNSIGNED32_MAX },
			.def = { .u32 = CO_UNSIGNED32_MIN },
			.val = { .u32 = CO_UNSIGNED32_MIN },
			.access = CO_ACCESS_RW,
			.pdo_mapping = 0,
			.flags = 0
		}}
	},
	{
		.name = CO_SDEV_STRING("Homing Speed"),
		.idx = 0x6099,
		.code = CO_OBJECT_VAR,
		.nsub = 1,
		.subs = (const struct co_ssub[]){{
			.name = CO_SDEV_STRING("Homing Speed"),
			.subidx = 0x00,
            .fram_address = 0x000090,
			.type = CO_DEFTYPE_UNSIGNED32,
			.min = { .u32 = CO_UNSIGNED32_MIN },
			.max = { .u32 = CO_UNSIGNED32_MAX },
			.def = { .u32 = CO_UNSIGNED32_MIN },
			.val = { .u32 = CO_UNSIGNED32_MIN },
			.access = CO_ACCESS_RW,
			.pdo_mapping = 0,
			.flags = 0
		}}
	},
	{
		.name = CO_SDEV_STRING("Homing Acceleration"),
		.idx = 0x609A,
		.code = CO_OBJECT_VAR,
		.nsub = 1,
		.subs = (const struct co_ssub[]){{
			.name = CO_SDEV_STRING("Homing Acceleration"),
			.subidx = 0x00,
            .fram_address = 0x000094,
			.type = CO_DEFTYPE_UNSIGNED32,
			.min = { .u32 = CO_UNSIGNED32_MIN },
			.max = { .u32 = CO_UNSIGNED32_MAX },
			.def = { .u32 = CO_UNSIGNED32_MIN },
			.val = { .u32 = CO_UNSIGNED32_MIN },
			.access = CO_ACCESS_RW,
			.pdo_mapping = 0,
			.flags = 0
		}}
	},
	{
		.name = CO_SDEV_STRING("Modes of Operation"),
		.idx = 0x6060,
		.code = CO_OBJECT_VAR,
		.nsub = 1,
		.subs = (const struct co_ssub[]){{
			.name = CO_SDEV_STRING("Modes of Operation"),
			.subidx = 0x00,
            .fram_address = 0x000098,
			.type = CO_DEFTYPE_UNSIGNED32,
			.min = { .u32 = CO_UNSIGNED32_MIN },
			.max = { .u32 = CO_UNSIGNED32_MAX },
			.def = { .u32 = CO_UNSIGNED32_MIN },
			.val = { .u32 = CO_UNSIGNED32_MIN },
			.access = CO_ACCESS_RW,
			.pdo_mapping = 0,
			.flags = 0
		}},
		},
		{
		.name = CO_SDEV_STRING("Target Position"),
		.idx = 0x607A,
		.code = CO_OBJECT_VAR,
		.nsub = 1,
		.subs = (const struct co_ssub[]){{
			.name = CO_SDEV_STRING("Target Position"),
			.subidx = 0x00,
            .fram_address = 0x00009C,
			.type = CO_DEFTYPE_UNSIGNED32,
			.min = { .u32 = CO_UNSIGNED32_MIN },
			.max = { .u32 = CO_UNSIGNED32_MAX },
			.def = { .u32 = CO_UNSIGNED32_MIN },
			.val = { .u32 = CO_UNSIGNED32_MIN },
			.access = CO_ACCESS_RW,
			.pdo_mapping = 0,
			.flags = 0
		}}
	},
			{
		.name = CO_SDEV_STRING("Target Velocity"),
		.idx = 0x60FF,
		.code = CO_OBJECT_VAR,
		.nsub = 1,
		.subs = (const struct co_ssub[]){{
			.name = CO_SDEV_STRING("Target Velocity"),
			.subidx = 0x00,
			.type = CO_DEFTYPE_UNSIGNED32,
			.min = { .u32 = CO_UNSIGNED32_MIN },
			.max = { .u32 = CO_UNSIGNED32_MAX },
			.def = { .u32 = CO_UNSIGNED32_MIN },
			.val = { .u32 = CO_UNSIGNED32_MIN },
			.access = CO_ACCESS_RW,
			.pdo_mapping = 0,
			.flags = 0
		}}
	}
	
}
};
