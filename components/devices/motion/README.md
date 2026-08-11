# Motion Sensors

Future chip-specific components: accelerometer, gyroscope, IMU, magnetometer, optical flow, and vibration sensor. Typical devices include ICM-42688, BMI270, LIS3DH, and BMM150.

Use `II2c` or `ISpi`; chip register protocol and calibration stay here, while filter/estimation policy belongs in services or libraries.
