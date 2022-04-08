#ifndef K_DURATION_H
#define K_DURATION_H

// An duration which can be positive or negative, with the precision of seconds.
typedef int duration_t;

duration_t duration_from_seconds(int seconds);

duration_t duration_from_minutes(int minutes);

duration_t duration_from_hours(int hours);

duration_t duration_from_components(int seconds, int minutes, int hours);

duration_t duration_as_seconds(duration_t duration);

duration_t duration_as_minutes(duration_t duration);

duration_t duration_as_hours(duration_t duration);

void duration_to_components(duration_t duration, int* seconds, int* minutes, int* hours);

void duration_to_components_array(duration_t duration, int components[3]);

#endif
