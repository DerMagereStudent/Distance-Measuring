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
};

#endif