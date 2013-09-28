/**
 * @file sys/event.h
 * Functions for distributing events.
 * @author Conlan Wesson
 */

#ifndef __SYS_EVENT_H_
#define __SYS_EVENT_H_

/**
 * Enum to define the payload type of an event.
 */
enum event_type{
	EVENT_TYPE_INT,      //!< Integer data.
	EVENT_TYPE_FLOAT,    //!< Floating-point data.
	EVENT_TYPE_STR       //!< String data.
};

/**
 * Event payload structure.
 */
typedef struct event_payload{
	enum event_type type;    //!< Type of data to expect.
	union{
		int i;      //!< Integer data.
		float f;    //!< Floating-point data.
		char *s;    //!< String data.
	};
} event_payload;

/**
 * Callback function to notify of events.
 * @param payload Event payload structure.
 */
typedef void *(*event_notify)(event_payload);

/**
 * Register to throw an event.
 * @param name Name of the event to register.
 * @return Unique key to allow throwing of the event, or 0 if the event has already been registered.
 */
int event_register(char* name);

/**
 * Register an event callback.
 * @param name Name of the event to listen for.
 * @param notify Callback function for the event.
 */
void event_listen(char* name, event_notify notify);

/**
 * Throw an event.
 * @param key The key of the event to throw (from event_register()).
 * @param payload Event payload to distribute.
 */
void event_throw(int key, event_payload payload);

#endif /* __SYS_EVENT_H_ */
