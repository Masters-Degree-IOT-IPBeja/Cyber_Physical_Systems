#ifndef __PUBLISH_DATA__
#define __PUBLISH_DATA__

#include "sensors.h"
#include "wifi_and_mqtt.h"
#include <PubSubClient.h>

extern void publish_sensors_data(PubSubClient *in_PubSubClient, st_all_sensors_t *data);
extern void publish_configs_data(PubSubClient *in_PubSubClient, st_configs_saved_memory_update_via_mqtt_t *in_configs); 

#endif // __PUBLISH_DATA__