#include "DistanceDataCollection.h"

DistanceDataCollection::DistanceDataCollection() { }
DistanceDataCollection::~DistanceDataCollection() {
    delete this->dataArray;
}

void DistanceDataCollection::insertData(DistanceData data) {
    int duplicateIndex = this->findDuplicate(data);

    if (duplicateIndex < 0) {
        if (this->dataArrayLength < DISTANCE_DATA_ARRAY_MAX_SIZE)
            this->pushToEnd(data);
        else
            this->insertAtBufferPointer(data);
    } else {
        // use old direction to prevent point moving. When slowly rotating the sensor the point should not move but update until rotated more than the threshold
        float duplicateDirection = this->dataArray[duplicateIndex].direction;

        if (this->dataArrayLength < DISTANCE_DATA_ARRAY_MAX_SIZE)
            this->updateDuplicateAndShiftToEnd({duplicateDirection, data.distance}, duplicateIndex);
        else
            this->updateDuplicateAndShiftToBufferPointer({duplicateDirection, data.distance}, duplicateIndex);
    }
}

DynamicJsonDocument* DistanceDataCollection::toJsonDocument() const {
    DynamicJsonDocument* doc = new DynamicJsonDocument(DISTANCE_DATA_JSON_DOC_BUFFER_SIZE);

    for (unsigned int i = 0; i < this->dataArrayLength; i++) {
        JsonObject item = doc->createNestedObject();
        item[DISTANCE_DATA_ARRAY_DIRECTION_KEY] = this->dataArray[i].direction;
        item[DISTANCE_DATA_ARRAY_DISTANCE_KEY] = this->dataArray[i].distance;
    }

    return doc;
}

int DistanceDataCollection::findDuplicate(DistanceData data) {
    for (unsigned int i = 0; i < this->dataArrayLength; i++)
        if (abs(data.direction - this->dataArray[i].direction) < DIRECTION_THRESHOLD)
            return i;

    return -1;
}

void DistanceDataCollection::pushToEnd(DistanceData data) {
    this->dataArray[this->dataArrayLength] = data;
    this->dataArrayLength++;
}

void DistanceDataCollection::insertAtBufferPointer(DistanceData data) {
    this->dataArray[this->circularBufferPointer] = data;
    this->incrementCircularBufferPointer();
}

void DistanceDataCollection::updateDuplicateAndShiftToEnd(DistanceData data, int duplicateIndex) {
    for (unsigned int i = duplicateIndex + 1; i < this->dataArrayLength; i++)
        this->dataArray[i - 1] = this->dataArray[i];

    this->dataArray[duplicateIndex] = data;
}

void DistanceDataCollection::updateDuplicateAndShiftToBufferPointer(DistanceData data, int duplicateIndex) {
    if (duplicateIndex == this->circularBufferPointer) {
        this->dataArray[duplicateIndex] = data;
        this->incrementCircularBufferPointer();
        return;
    }

    if (duplicateIndex < this->circularBufferPointer) {
        for (unsigned int i = duplicateIndex + 1; i < this->circularBufferPointer; i++)
            this->dataArray[i - 1] = this->dataArray[i];

        this->dataArray[this->circularBufferPointer - 1] = data;
        return;
    }

    // if duplicateIndex > circularBufferPointer
    for (unsigned int i = duplicateIndex - 1; i > this->circularBufferPointer; i--)
            this->dataArray[i + 1] = this->dataArray[i];

    this->dataArray[this->circularBufferPointer] = data;
    this->incrementCircularBufferPointer();
}

void DistanceDataCollection::incrementCircularBufferPointer() {
    this->circularBufferPointer++;

    if (this->circularBufferPointer >= DISTANCE_DATA_ARRAY_MAX_SIZE)
        this->circularBufferPointer = 0;
}
