#ifndef DATA_FILTER_H
#define DATA_FILTER_H

#include <stdint.h>

#define DATA_BUFFER_SIZE 14

class DataFilter
{
	float data[DATA_BUFFER_SIZE];
	uint8_t pos;

public:
	DataFilter();

	float & top();
	float & prev();

	void push(float value);
	float mean();
};

#endif // DATA_FILTER_H