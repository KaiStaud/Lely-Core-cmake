/**@file
 * Local FRAM-aware static CANopen device description declarations.
 */

#ifndef LELY_CO_SDEV_H_
#define LELY_CO_SDEV_H_

#include <lely/co/dev.h>
#include <lely/co/obj.h>
#include <lely/co/val.h>

struct co_sdev {
	co_unsigned8_t id;
	const char *name;
	const char *vendor_name;
	co_unsigned32_t vendor_id;
	const char *product_name;
	co_unsigned32_t product_code;
	co_unsigned32_t revision;
	const char *order_code;
	unsigned baud : 10;
	co_unsigned16_t rate;
	int lss;
	co_unsigned32_t dummy;
	co_unsigned16_t nobj;
	const struct co_sobj *objs;
};

struct co_sobj {
#if !LELY_NO_CO_OBJ_NAME
	const char *name;
#endif
	co_unsigned16_t idx;
	co_unsigned8_t code;
	co_unsigned8_t nsub;
	const struct co_ssub *subs;
};

struct co_ssub {
#if !LELY_NO_CO_OBJ_NAME
	const char *name;
#endif
	co_unsigned8_t subidx;
	co_unsigned32_t fram_address;
	co_unsigned16_t type;
#if !LELY_NO_CO_OBJ_LIMITS
	union co_val min;
	union co_val max;
#endif
#if !LELY_NO_CO_OBJ_DEFAULT
	union co_val def;
#endif
	union co_val val;
	uint_least32_t access : 5;
	uint_least32_t pdo_mapping : 1;
	uint_least32_t flags : 26;
};

#ifdef __cplusplus
extern "C" {
#endif

struct __co_dev *__co_dev_init_from_sdev(
		struct __co_dev *dev, const struct co_sdev *sdev);

co_dev_t *co_dev_create_from_sdev(const struct co_sdev *sdev);

int co_sdev_snprintf(char *s, size_t n, const struct co_sdev *sdev);

#ifdef __cplusplus
}
#endif

#endif
