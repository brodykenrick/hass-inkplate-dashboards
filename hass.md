# HomePlate Home Assistant Setup

The HomePlate makes use of [MQTT Discovery](https://www.home-assistant.io/docs/mqtt/discovery/) so it should automatically add its sensors to your Home Assistant instance if MQTT is already setup.

For example dashboard yaml see [dashboard.md](dashboard.md).

## MQTT Commands

You can change the activity running on the HomePlate by publishing the following MQTT message to the topic: `homeplate/<mqtt_node_id>/activity/run` which defaults to `homeplate/homeplate/activity/run`

T

You can add additional data to the action as well. To display an image:

```json
{
    "action": "img",
    "message": "http://homeassistant.local:5006/1.png"
}
```

```yaml
    - service: mqtt.publish
      data:
        topic: homeplate/homeplate/activity/run
        qos: '1'
        payload: '{ "action": "img", "refresh": "300" , "message":"http://homeassistant.local:5006/2.png"}'
        retain: true
```
