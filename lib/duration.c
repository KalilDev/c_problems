#include "duration.h"

const int segundos_por_minuto = 60;
const int minutos_por_hora = 60;
const int segundos_por_hora = segundos_por_minuto*minutos_por_hora;

duration_t duration_from_seconds(int seconds) {
    return seconds;
}

duration_t duration_from_minutes(int minutes) {
    return minutes*segundos_por_minuto;
}

duration_t duration_from_hours(int hours) {
    return hours*segundos_por_hora;
}

duration_t duration_from_components(int seconds, int minutes, int hours) {
    return duration_from_seconds(seconds) + duration_from_minutes(minutes) + duration_from_hours(hours);
}

duration_t duration_as_seconds(duration_t duration) {
    return duration;
}

duration_t duration_as_minutes(duration_t duration) {
    return duration / segundos_por_minuto;
}

duration_t duration_as_hours(duration_t duration) {
    return duration / segundos_por_hora;
}

void duration_to_components(duration_t duration, int* seconds, int* minutes, int* hours) {
    *seconds = duration_as_seconds(duration) % segundos_por_minuto;
    *minutes = duration_as_minutes(duration) % minutos_por_hora;
    *hours = duration_as_hours(duration);
}

void duration_to_components_array(duration_t duration, int components[3]) {
    duration_to_components(duration, components, components + 1, components + 2);
}

