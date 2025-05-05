
# ADS1015 Comparator Library

## Initialization

``` cpp

// construct a new adc instance at the given address
ads1015 adc0 = ads1015(i2c_conn /* ptr to i2c_inst_t */, addr /* address of ads */);

// construct a new adc instance with the alert pin attached 
ads1015 adc1 = ads1015(i2c_conn, addr, pin /* the pin alert is tied to */);

```

## Usage

### ADC

``` cpp

// start a single conversion, poll for it to end, and read contents

adc0.start_oneshot_conversion();

while (adc0.is_converting());

uint16_t res = adc0.read_conversion();

```

``` cpp

// start continous conversion and poll on alert for finishing

adc1.set_continous_conversion();

adc1.set_alert_on();

uint8_t pos;
uint16_t buf[256];

for ( ; ; ) {

    while (!adc1.alert());

    buf[pos++] = adc1.read_conversion();

    /* other logic ... */

}

```

``` cpp

// set gain to 4.096 two different ways  

adc0.set_gain(ADS1015_PGA_4_096);

adc0.set_gain_best_effort(4.096);

```

``` cpp

// set data rate to 1600 sps two different ways 

adc0.set_data_rate(ADS1015_DR_1600SPS);

adc0.set_data_rate_best_effort(1600);

```

``` cpp

// set the mutiplexer to AINP = AIN2 and AINN = AIN3

adc0.set_multiplexer(ADS1015_MUX_AIN2_AIN3);

```

### Alert Usage

``` cpp

// enable ready alert if the adc has the pin
// may interfere with comparator if it is on

// does nothing...
adc0.set_alert_on();

// turns on alert
adc1.set_alert_on();
```

``` cpp

// disable ready alert functionality
// also implicitly disable comparator

adc1.set_alarm_off();

```

``` cpp

// check if alert pin is high
adc1.alert();

```

### Comparator

``` cpp

// set comparator to check if analog value falls above 1200 two times

adc1.enable_comparator(ADS1015_COMP_QUE_TWO, 1200, 0, false);

while (!adc1.alert());

// triggered!
adc1.disable_comparator();


```

``` cpp

// set comparator to latch if analog value falls below 1200 four times

adc1.enable_comparator(ADS1015_COMP_QUE_FOUR, 0xfff, 1200, true);

while (!adc1.alert());

// triggered!
adc1.disable_comparator();

```

``` cpp

// set comparator to window mode 

adc1.set_comparator_window(true);

```

``` cpp

// set comparator polarity to high

adc1.set_comparator_polarity(true);

```

### Primitive Operations

The actions above can all be achieved by writing to the config 
struct in the ads class instance and then calling set_config to
write it. Additionally, the values of the registers on the device
can be read and written using the read_* and write_* functions.

``` cpp

// set adc to one-shot mode and start a conversion

adc0.config.operating_mode = ADS1015_MODE_ONESHOT;
adc0.config.operational_status = ADS1015_OS_SINGLE;

adc0.set_config();

```

``` cpp

// turn on the ready alert pin and enable continous conversion 

adc1.write_hithresh(0x8000);
adc1.write_lothresh(0x0000);

adc1.config.comparator_assert = ADS1015_COMP_QUE_ONE;
adc1.config.operating_mode = ADS1015_MODE_CONTINUOUS;

adc1.set_config();

```

``` cpp

// read the current values of all registers 

uint16_t conversion = adc0.read_conversion();
uint16_t config = adc0.read_config();
uint16_t lothresh = adc0.read_lothresh();
uint16_t hithresh = adc0.read_hithresh();

```
