#include "event.h"

#include <SDL3/SDL.h>

typedef struct EventNode {
  Event event;
  struct EventNode* next;
} EventNode;

typedef struct {
  EventNode* head;
  EventNode* tail;
} EventQueue;

static struct {
  EventQueue event_queue;
} state;

static void enqueue_event(EventQueue* queue, SDL_Event* sdl_event) {
  Event event;

  switch (sdl_event->type) {
    case SDL_EVENT_QUIT:
      event.type = EVENT_QUIT;
      break;
    default:
      return;
  }

  EventNode* node = SDL_malloc(sizeof(EventNode));
  if (!node)
    return;

  node->event = event;
  node->next = NULL;

  if (queue->tail) {
    queue->tail->next = node;
  } else {
    queue->head = node;
  }

  queue->tail = node;
}

bool event_open(void) {
  SDL_Log("event module initialized.");
  state.event_queue.head = NULL;
  state.event_queue.tail = NULL;
  return true;
}

void event_close(void) {
  SDL_Log("event module closed.");
}

void event_pump(void) {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    enqueue_event(&state.event_queue, &e);
  }
}

bool event_poll(Event* event) {
  if (!state.event_queue.head) {
    return false;
  }

  EventNode* node = state.event_queue.head;
  *event = node->event;

  state.event_queue.head = node->next;
  if (!state.event_queue.head) {
    state.event_queue.tail = NULL;
  }

  SDL_free(node);
  return true;
}
