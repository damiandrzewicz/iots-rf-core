#include "Buffer.h"

Buffer::Buffer()
{
    clear();
}

char *Buffer::data()
{
    return buffer_;
}

uint8_t Buffer::size()
{
    return strlen(buffer_);
}

void Buffer::appendText(const char *data, bool lastItem)
{
    strcat(buffer_, data);
    if(!lastItem){ appendDelimeter(); }
}

void Buffer::appendInt(long value, bool lastItem)
{
    snprintf(buffer_ + strlen(buffer_), sizeof(buffer_) - strlen(buffer_), "%ld", value);
    if(!lastItem){ appendDelimeter(); }
}

void Buffer::appendFloat(double value, uint8_t size, uint8_t prec, bool lastItem)
{
    char temp[10];
    dtostrf(value, size, prec, temp);
    strcat(buffer_, temp);
    if(!lastItem){ appendDelimeter(); }
}

void Buffer::appendDelimeter()
{
    strcat(buffer_, "|");
}

void Buffer::clear()
{
    memset(buffer_, 0, sizeof(buffer_));
}