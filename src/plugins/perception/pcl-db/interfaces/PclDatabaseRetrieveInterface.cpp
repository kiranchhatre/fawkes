
/***************************************************************************
 *  PclDatabaseRetrieveInterface.cpp - Fawkes BlackBoard Interface - PclDatabaseRetrieveInterface
 *
 *  Templated created:   Thu Oct 12 10:49:19 2006
 *  Copyright  2012-2013  Tim Niemueller
 *
 ****************************************************************************/

/*  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version. A runtime exception applies to
 *  this software (see LICENSE.GPL_WRE file mentioned below for details).
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  Read the full text in the LICENSE.GPL_WRE file in the doc directory.
 */

#include <interfaces/PclDatabaseRetrieveInterface.h>

#include <core/exceptions/software.h>

#include <cstring>
#include <cstdlib>

namespace fawkes {

/** @class PclDatabaseRetrieveInterface <interfaces/PclDatabaseRetrieveInterface.h>
 * PclDatabaseRetrieveInterface Fawkes BlackBoard Interface.
 * 
      Instruct the pcl-db-retrieve plugin and receive information.
    
 * @ingroup FawkesInterfaces
 */



/** Constructor */
PclDatabaseRetrieveInterface::PclDatabaseRetrieveInterface() : Interface()
{
  data_size = sizeof(PclDatabaseRetrieveInterface_data_t);
  data_ptr  = malloc(data_size);
  data      = (PclDatabaseRetrieveInterface_data_t *)data_ptr;
  data_ts   = (interface_data_ts_t *)data_ptr;
  memset(data_ptr, 0, data_size);
  add_fieldinfo(IFT_UINT32, "msgid", 1, &data->msgid);
  add_fieldinfo(IFT_BOOL, "final", 1, &data->final);
  add_fieldinfo(IFT_STRING, "error", 256, data->error);
  add_messageinfo("RetrieveMessage");
  unsigned char tmp_hash[] = {0x1d, 0xa9, 0x54, 0xc8, 0x23, 0x39, 0xd9, 0xc, 0x60, 0x69, 0xdb, 0xc7, 0xa0, 0x63, 0xac, 0x55};
  set_hash(tmp_hash);
}

/** Destructor */
PclDatabaseRetrieveInterface::~PclDatabaseRetrieveInterface()
{
  free(data_ptr);
}
/* Methods */
/** Get msgid value.
 * 
      The ID of the message that is currently being processed, or 0 if
      no message is being processed.
    
 * @return msgid value
 */
uint32_t
PclDatabaseRetrieveInterface::msgid() const
{
  return data->msgid;
}

/** Get maximum length of msgid value.
 * @return length of msgid value, can be length of the array or number of 
 * maximum number of characters for a string
 */
size_t
PclDatabaseRetrieveInterface::maxlenof_msgid() const
{
  return 1;
}

/** Set msgid value.
 * 
      The ID of the message that is currently being processed, or 0 if
      no message is being processed.
    
 * @param new_msgid new msgid value
 */
void
PclDatabaseRetrieveInterface::set_msgid(const uint32_t new_msgid)
{
  data->msgid = new_msgid;
  data_changed = true;
}

/** Get final value.
 * 
      True, if the last goto command has been finished, false if it is
      still running.
    
 * @return final value
 */
bool
PclDatabaseRetrieveInterface::is_final() const
{
  return data->final;
}

/** Get maximum length of final value.
 * @return length of final value, can be length of the array or number of 
 * maximum number of characters for a string
 */
size_t
PclDatabaseRetrieveInterface::maxlenof_final() const
{
  return 1;
}

/** Set final value.
 * 
      True, if the last goto command has been finished, false if it is
      still running.
    
 * @param new_final new final value
 */
void
PclDatabaseRetrieveInterface::set_final(const bool new_final)
{
  data->final = new_final;
  data_changed = true;
}

/** Get error value.
 * 
      Error description if reconstruction fails.
    
 * @return error value
 */
char *
PclDatabaseRetrieveInterface::error() const
{
  return data->error;
}

/** Get maximum length of error value.
 * @return length of error value, can be length of the array or number of 
 * maximum number of characters for a string
 */
size_t
PclDatabaseRetrieveInterface::maxlenof_error() const
{
  return 256;
}

/** Set error value.
 * 
      Error description if reconstruction fails.
    
 * @param new_error new error value
 */
void
PclDatabaseRetrieveInterface::set_error(const char * new_error)
{
  strncpy(data->error, new_error, sizeof(data->error));
  data_changed = true;
}

/* =========== message create =========== */
Message *
PclDatabaseRetrieveInterface::create_message(const char *type) const
{
  if ( strncmp("RetrieveMessage", type, __INTERFACE_MESSAGE_TYPE_SIZE) == 0 ) {
    return new RetrieveMessage();
  } else {
    throw UnknownTypeException("The given type '%s' does not match any known "
                               "message type for this interface type.", type);
  }
}


/** Copy values from other interface.
 * @param other other interface to copy values from
 */
void
PclDatabaseRetrieveInterface::copy_values(const Interface *other)
{
  const PclDatabaseRetrieveInterface *oi = dynamic_cast<const PclDatabaseRetrieveInterface *>(other);
  if (oi == NULL) {
    throw TypeMismatchException("Can only copy values from interface of same type (%s vs. %s)",
                                type(), other->type());
  }
  memcpy(data, oi->data, sizeof(PclDatabaseRetrieveInterface_data_t));
}

const char *
PclDatabaseRetrieveInterface::enum_tostring(const char *enumtype, int val) const
{
  throw UnknownTypeException("Unknown enum type %s", enumtype);
}

/* =========== messages =========== */
/** @class PclDatabaseRetrieveInterface::RetrieveMessage <interfaces/PclDatabaseRetrieveInterface.h>
 * RetrieveMessage Fawkes BlackBoard Interface Message.
 * 
    
 */


/** Constructor with initial values.
 * @param ini_timestamp initial value for timestamp
 * @param ini_collection initial value for collection
 */
PclDatabaseRetrieveInterface::RetrieveMessage::RetrieveMessage(const int64_t ini_timestamp, const char * ini_collection) : Message("RetrieveMessage")
{
  data_size = sizeof(RetrieveMessage_data_t);
  data_ptr  = malloc(data_size);
  memset(data_ptr, 0, data_size);
  data      = (RetrieveMessage_data_t *)data_ptr;
  data_ts   = (message_data_ts_t *)data_ptr;
  data->timestamp = ini_timestamp;
  strncpy(data->collection, ini_collection, 256);
  add_fieldinfo(IFT_INT64, "timestamp", 1, &data->timestamp);
  add_fieldinfo(IFT_STRING, "collection", 256, data->collection);
}
/** Constructor */
PclDatabaseRetrieveInterface::RetrieveMessage::RetrieveMessage() : Message("RetrieveMessage")
{
  data_size = sizeof(RetrieveMessage_data_t);
  data_ptr  = malloc(data_size);
  memset(data_ptr, 0, data_size);
  data      = (RetrieveMessage_data_t *)data_ptr;
  data_ts   = (message_data_ts_t *)data_ptr;
  add_fieldinfo(IFT_INT64, "timestamp", 1, &data->timestamp);
  add_fieldinfo(IFT_STRING, "collection", 256, data->collection);
}

/** Destructor */
PclDatabaseRetrieveInterface::RetrieveMessage::~RetrieveMessage()
{
  free(data_ptr);
}

/** Copy constructor.
 * @param m message to copy from
 */
PclDatabaseRetrieveInterface::RetrieveMessage::RetrieveMessage(const RetrieveMessage *m) : Message("RetrieveMessage")
{
  data_size = m->data_size;
  data_ptr  = malloc(data_size);
  memcpy(data_ptr, m->data_ptr, data_size);
  data      = (RetrieveMessage_data_t *)data_ptr;
  data_ts   = (message_data_ts_t *)data_ptr;
}

/* Methods */
/** Get timestamp value.
 * 
      Timestamp for which to retrieve the most recent point clouds.
    
 * @return timestamp value
 */
int64_t
PclDatabaseRetrieveInterface::RetrieveMessage::timestamp() const
{
  return data->timestamp;
}

/** Get maximum length of timestamp value.
 * @return length of timestamp value, can be length of the array or number of 
 * maximum number of characters for a string
 */
size_t
PclDatabaseRetrieveInterface::RetrieveMessage::maxlenof_timestamp() const
{
  return 1;
}

/** Set timestamp value.
 * 
      Timestamp for which to retrieve the most recent point clouds.
    
 * @param new_timestamp new timestamp value
 */
void
PclDatabaseRetrieveInterface::RetrieveMessage::set_timestamp(const int64_t new_timestamp)
{
  data->timestamp = new_timestamp;
}

/** Get collection value.
 * 
      Collection name from which to read the point clouds. Shall NOT
      include the database name.
    
 * @return collection value
 */
char *
PclDatabaseRetrieveInterface::RetrieveMessage::collection() const
{
  return data->collection;
}

/** Get maximum length of collection value.
 * @return length of collection value, can be length of the array or number of 
 * maximum number of characters for a string
 */
size_t
PclDatabaseRetrieveInterface::RetrieveMessage::maxlenof_collection() const
{
  return 256;
}

/** Set collection value.
 * 
      Collection name from which to read the point clouds. Shall NOT
      include the database name.
    
 * @param new_collection new collection value
 */
void
PclDatabaseRetrieveInterface::RetrieveMessage::set_collection(const char * new_collection)
{
  strncpy(data->collection, new_collection, sizeof(data->collection));
}

/** Clone this message.
 * Produces a message of the same type as this message and copies the
 * data to the new message.
 * @return clone of this message
 */
Message *
PclDatabaseRetrieveInterface::RetrieveMessage::clone() const
{
  return new PclDatabaseRetrieveInterface::RetrieveMessage(this);
}
/** Check if message is valid and can be enqueued.
 * @param message Message to check
 * @return true if the message is valid, false otherwise.
 */
bool
PclDatabaseRetrieveInterface::message_valid(const Message *message) const
{
  const RetrieveMessage *m0 = dynamic_cast<const RetrieveMessage *>(message);
  if ( m0 != NULL ) {
    return true;
  }
  return false;
}

/// @cond INTERNALS
EXPORT_INTERFACE(PclDatabaseRetrieveInterface)
/// @endcond


} // end namespace fawkes
