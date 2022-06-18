#ifndef MPU_6050_GYRO_DATA_H
#define MPU_6050_GYRO_DATA_H

struct Mpu6050GyroData {
    float x, y, z;

    Mpu6050GyroData operator+(Mpu6050GyroData other) {
        return {
            this->x + other.x,
            this->y + other.y,
            this->z + other.z,
        };
    }

    Mpu6050GyroData operator-(Mpu6050GyroData other) {
        return {
            this->x - other.x,
            this->y - other.y,
            this->z - other.z,
        };
    }

    Mpu6050GyroData operator*(float scale) {
        return {
            this->x * scale,
            this->y * scale,
            this->z * scale,
        };
    }

    Mpu6050GyroData operator/(float divisor) {
        return {
            this->x / divisor,
            this->y / divisor,
            this->z / divisor,
        };
    }

// Serial.print("Rotation X: ");
  // Serial.print(orientation.x);
  // Serial.print(", Y: ");
  // Serial.print(orientation.y);
  // Serial.print(", Z: ");
  // Serial.print(orientation.z);
  // Serial.println(" deg");

    String toString(unsigned int decimalPlaces = 2U) {
        return "Rotation X: " + String(this->x, decimalPlaces) + ", Y: " + String(this->y, decimalPlaces) + ", Z: " + String(this->z, decimalPlaces);
    }
};

#endif