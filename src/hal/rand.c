/**
 * @file hal/rand.c
 * Random Number Generator using the Mersenne Twister.
 * @author Conlan Wesson
 */

#include "rand.h"

#include <errno.h>
#include <stdbool.h>
#include "device.h"
#include "../dev/rtc.h"

#define POOL_SIZE 624

static int pool[POOL_SIZE];    //!< Random data pool.
static int index = 0;          //!< Last used index from the pool.
static bool init = false;      //!< Has the RNG been initialized?

/**
 * Regenerates the pool after using all indexes.
 */
static void rand_regen(){
	for(int i = 0; i < POOL_SIZE; ++i){
		int y = (pool[i] >> 31) + ((pool[(i+1) % POOL_SIZE]) & 0x7FFFFFFF);
		pool[i] = pool[(i + (POOL_SIZE*6362)/10000) % POOL_SIZE] ^ (y >> 1);
		if(y & 0x01){
			pool[i] ^= 0x9908B0DF;
		}
	}
}

/**
 * Seeds the RNG.
 * @param seed The seed value.
 * @return Error code or EOK.
 */
int rand_write(char seed){
	pool[0] = (seed*685956421) ^ (seed*567498787) ^ (seed*135768463) ^ (seed*954167527);    // Random prime numbers.
	for(int i = 1; i < POOL_SIZE; ++i){
		pool[i] = 0x6C078965 * (pool[i-1] ^ ((pool[i-1]) >> 30)) + i;
	}
	init = true;
	return EOK;
}

/**
 * Initialize the random number generator.
 */
void rand_init(){
	datetime dt;
	rtc_time(&dt);
	rand_write(dt.sec + dt.min*60 + dt.hour*3600 + dt.day*86400);
}

/**
 * Generates a random integer.
 * @return A random number.
 */
char rand_read(){
	if(!init){
		rand_init();
	}
	if(!index){
		rand_regen();
	}
	
	int y = pool[index];
	y ^= y >> 11;
	y ^= (y << 7) & 0x9D2C5680;
	y ^= (y << 15) & 0xEFC60000;
	y ^= y >> 18;
	index = (index + 1) % POOL_SIZE;
	return (y >> 20) & 0x000000FF;
}

/**
 * Always returns EOK.
 * @return EOK
 */
int rand_flush(){
	return EOK;
}

/**
 * RNG device descriptor.
 */
device_descriptor rand_desc = {
	"Random Number Generator.",
	DEVICE_FLAG_INOUT | DEVICE_FLAG_STREAM | DEVICE_FLAG_VIRTUAL,
	0, 0,
	0, 0,
	0, 0,
	rand_read, rand_write, rand_flush
};

