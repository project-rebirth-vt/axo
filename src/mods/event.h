#ifndef EVENT_H
#define EVENT_H

#include <stdbool.h>

typedef enum {
  EVENT_QUIT,
} EventType;

typedef struct {
  EventType type;
  // union {
  // extra data for specific event types
  // } data;
} Event;

bool event_open(void);
void event_close(void);
void event_pump(void);
bool event_poll(Event* event);

#endif
