#ifndef DISTANCE_DATA_COLLECTION_H
#define DISTANCE_DATA_COLLECTION_H

#include <ArduinoJson.h>

#define DIRECTION_THRESHOLD 2.0f
#define DISTANCE_DATA_ARRAY_MAX_SIZE 50
#define DISTANCE_DATA_JSON_DOC_BUFFER_SIZE 3072
#define DISTANCE_DATA_ARRAY_DIRECTION_KEY "direction"
#define DISTANCE_DATA_ARRAY_DISTANCE_KEY "distance"

struct DistanceData {
    float direction, distance;
};

class DistanceDataCollection {
public:
    DistanceDataCollection();
    ~DistanceDataCollection();

    void insertData(DistanceData data);
    DynamicJsonDocument* toJsonDocument() const;

private:
    int findDuplicate(DistanceData data);
    void pushToEnd(DistanceData data);
    void insertAtBufferPointer(DistanceData data);
    void updateDuplicateAndShiftToEnd(DistanceData data, int duplicateIndex);
    void updateDuplicateAndShiftToBufferPointer(DistanceData data, int duplicateIndex);

    void incrementCircularBufferPointer();

    DistanceData* dataArray = new DistanceData[DISTANCE_DATA_ARRAY_MAX_SIZE];
    unsigned int dataArrayLength = 0;
    unsigned int circularBufferPointer = 0;
};

#endif