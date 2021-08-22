#include "data_filter.h"

DataFilter::DataFilter()
{
	for(uint8_t i = 0; i < DATA_BUFFER_SIZE; i++) data[i];
	pos = 0;
}

void DataFilter::push(float value)
{
	pos++;
	if(pos >= DATA_BUFFER_SIZE) pos = 0;
	data[pos] = value;
}

float DataFilter::mean()
{
	float acc = 0;
	for(uint8_t i = 0; i < DATA_BUFFER_SIZE; i++) acc += data[i];
	return acc / (float)DATA_BUFFER_SIZE;
}

float & DataFilter::top()
{
	return data[pos];
}

float & DataFilter::prev()
{
	if(pos == 0) return data[DATA_BUFFER_SIZE - 1];
	return data[pos];
}
