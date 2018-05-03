package io.silverspoon.bulldog.raspberrypi.bcm;

/*
  Common clock divider frequencies used to set the PWM frequency. It is highly
  recommended to use one of the following values to set the frequency of the
  bus. Each enum has written the belonging resulting frequency in its javadoc.

  https://raspberrypi.stackexchange.com/questions/53854/driving-pwm-output-frequency
 */
public enum PwmClockDivider {
    /*
     4.6875kHz
     */
    BCM2835_PWM_CLOCK_DIVIDER_4096(4096),
    /*
     9.375kHz
     */
    BCM2835_PWM_CLOCK_DIVIDER_2048(2048),
    /*
     18.75kHz
     */
    BCM2835_PWM_CLOCK_DIVIDER_1024(1024),
    /*
     37.5kHz
     */
    BCM2835_PWM_CLOCK_DIVIDER_512(512),
    /*
     75kHz
     */
    BCM2835_PWM_CLOCK_DIVIDER_256(256),
    /*
     150kHz
     */
    BCM2835_PWM_CLOCK_DIVIDER_128(128),
    /*
     300kHz
     */
    BCM2835_PWM_CLOCK_DIVIDER_64(64),
    /*
     600kHz
     */
    BCM2835_PWM_CLOCK_DIVIDER_32 (32),
    /*
     1.2MHz
     */
    BCM2835_PWM_CLOCK_DIVIDER_16(16),
    /*
     2.4Mhz
     */
    BCM2835_PWM_CLOCK_DIVIDER_8 (8),
    /*
     4.8Mhz
     */
    BCM2835_PWM_CLOCK_DIVIDER_4(4),
    /*
     9.6MHz, the fastest possible frequency
     */
    BCM2835_PWM_CLOCK_DIVIDER_2(2),
    /*
     4.6875kHz
     */
    BCM2835_PWM_CLOCK_DIVIDER_1(1),
    DEFAULT(PwmClockDivider.BCM2835_PWM_CLOCK_DIVIDER_16);
    private final int divider;
    PwmClockDivider(int divider) {
        this.divider = divider;
    }
    PwmClockDivider(PwmClockDivider div) { this.divider = div.divider; }

    public int getDivider() {
        return divider;
    }
 }
