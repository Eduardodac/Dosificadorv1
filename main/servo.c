#include "servo.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>

#define SERVO_PULSE_GPIO 4
#define SERVO_TIMEBASE_RESOLUTION_HZ 1300000
#define SERVO_TIMEBASE_PERIOD 20000

static const char *TAG = "SERVO";

static uint32_t example_angle_to_compare(int angle)
{
    return (angle + 90) * (2500 - 500) / 180 + 500;
}

void servo_control_task(void *arg)
{
    mcpwm_timer_handle_t timer = NULL;
    mcpwm_timer_config_t timer_config = {
        .clk_src = MCPWM_TIMER_CLK_SRC_DEFAULT,
        .resolution_hz = SERVO_TIMEBASE_RESOLUTION_HZ,
        .period_ticks = SERVO_TIMEBASE_PERIOD,
        .count_mode = MCPWM_TIMER_COUNT_MODE_UP,
    };
    mcpwm_new_timer(&timer_config, &timer);

    mcpwm_oper_handle_t oper = NULL;
    mcpwm_operator_config_t operator_config = {0};
    mcpwm_new_operator(&operator_config, &oper);
    mcpwm_operator_connect_timer(oper, timer);

    mcpwm_cmpr_handle_t comparator = NULL;
    mcpwm_comparator_config_t comparator_config = {.flags.update_cmp_on_tez = true};
    mcpwm_new_comparator(oper, &comparator_config, &comparator);

    mcpwm_gen_handle_t generator = NULL;
    mcpwm_generator_config_t generator_config = {.gen_gpio_num = SERVO_PULSE_GPIO};
    mcpwm_new_generator(oper, &generator_config, &generator);

    mcpwm_comparator_set_compare_value(comparator, example_angle_to_compare(0));
    mcpwm_generator_set_action_on_timer_event(generator, MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, MCPWM_TIMER_EVENT_EMPTY, MCPWM_GEN_ACTION_HIGH));
    mcpwm_generator_set_action_on_compare_event(generator, MCPWM_GEN_COMPARE_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, comparator, MCPWM_GEN_ACTION_LOW));

    mcpwm_timer_enable(timer);
    mcpwm_timer_start_stop(timer, MCPWM_TIMER_START_NO_STOP);

    while (1)
    {
        int angle = 0;
        ESP_LOGI(TAG, "Angle: %d", angle);
        mcpwm_comparator_set_compare_value(comparator, example_angle_to_compare(angle));
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
