#ifndef PROTOCOL_H
#define PROTOCOL_H

typedef enum {
  PROTO_HELLO,
} ProtocolType;

// TLV - Type Length Value system
typedef struct {
  ProtocolType type;
  unsigned short length;
} ProtocolHeader;

#endif
