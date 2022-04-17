/**
 * @file message_types.h
 *
 * Project:	message processor
 *
 * enum for message types. It is not used in current version.
 */


#pragma once
namespace message_processor
{
  enum class MessageType
  {
    AnyType = 1,
    MsgType,
    SeqNumber,
    Side,
    Size,
    Ticker,
    Price
  };
}
