/// DBC file: 243.dbc    Self node: GPS
/// This file should be included by a source file, for example: #include "generated.c"
#ifndef __GENEARTED_DBC_PARSER
#define __GENERATED_DBC_PARSER
#include <stdbool.h>
#include <stdint.h>
#include <cstddef>


/// Missing in Action structure
typedef struct {
    uint32_t is_mia : 1;          ///< Missing in action flag
    uint32_t mia_counter_ms : 31; ///< Missing in action counter
} mia_info_t;

/// CAN message header structure
typedef struct { 
    uint32_t mid; ///< Message ID of the message
    uint8_t  dlc; ///< Data length of the message
} msg_hdr_t; 

static const msg_hdr_t DRIVER_TX_HEARTBEAT_HDR =              { 0xA0, 1 };
static const msg_hdr_t GPS_TX_COORDINATES_HDR =               { 0x18b, 1 };
static const msg_hdr_t SENSOR_TX_SONARS_HDR =                 {  200, 6 };
static const msg_hdr_t DRIVER_TX_MOTOR_CMD_HDR =              { 0X020, 1 };


/// Message: HEARTBEAT from 'DRIVER', DLC: 1 byte(s), MID: 0xA0
typedef struct {
    uint8_t DRIVER_HEARTBEAT_cmd;             ///< B7:0   Destination: SENSOR,MOTOR,GPS

    mia_info_t mia_info;
} DRIVER_TX_HEARTBEAT_t;


/// Message: COORDINATES from 'GPS', DLC: 1 byte(s), MID: 0x18b
typedef struct {
    uint8_t GPS_LAT1;                         ///< B7:0   Destination: DRIVER,IO,MOTOR
    uint8_t GPS_LAT2;                         ///< B15:8   Destination: DRIVER,IO,MOTOR
    uint8_t GPS_LONG1;                        ///< B23:16   Destination: DRIVER,IO,MOTOR
    uint8_t GPS_LONG2;                        ///< B31:24   Destination: DRIVER,IO,MOTOR
    uint8_t CHKPT_DIST;                       ///< B39:32   Destination: DRIVER,IO,MOTOR
    uint8_t REACHED_FLAG;                     ///< B47:40   Destination: DRIVER,IO,MOTOR
    uint8_t HEADING_CURR;                     ///< B55:48   Destination: DRIVER,IO,MOTOR
    uint8_t HEADING_DESIRED;                  ///< B63:56   Destination: DRIVER,IO,MOTOR

    mia_info_t mia_info;
} GPS_TX_COORDINATES_t;


/// Not generating 'SENSOR_TX_SONARS_t' since we are not the sender or a recipient of any of its signals

/// Not generating 'DRIVER_TX_MOTOR_CMD_t' since we are not the sender or a recipient of any of its signals

/// These 'externs' need to be defined in a source file of your project
extern const uint32_t                                         HEARTBEAT__MIA_MS;
extern const DRIVER_TX_HEARTBEAT_t                            HEARTBEAT__MIA_MSG;

/// Not generating code for DRIVER_TX_HEARTBEAT_encode() since the sender is DRIVER and we are GPS

/// Encode GPS's 'COORDINATES' message
/// @returns the message header of this message
static msg_hdr_t GPS_TX_COORDINATES_encode(uint64_t *to, GPS_TX_COORDINATES_t *from)
{
    *to = 0; ///< Default the entire destination data with zeroes
    uint8_t *bytes = (uint8_t*) to;
    uint64_t raw_signal;

    raw_signal = ((uint64_t)(((from->GPS_LAT1 - (0)) / 1.0) + 0.5)) & 0xff;
    bytes[0] |= (((uint8_t)(raw_signal >> 0) & 0xff) << 0); ///< 8 bit(s) to B0

    raw_signal = ((uint64_t)(((from->GPS_LAT2 - (0)) / 1.0) + 0.5)) & 0xff;
    bytes[1] |= (((uint8_t)(raw_signal >> 0) & 0xff) << 0); ///< 8 bit(s) to B8

    raw_signal = ((uint64_t)(((from->GPS_LONG1 - (0)) / 1.0) + 0.5)) & 0xff;
    bytes[2] |= (((uint8_t)(raw_signal >> 0) & 0xff) << 0); ///< 8 bit(s) to B16

    raw_signal = ((uint64_t)(((from->GPS_LONG2 - (0)) / 1.0) + 0.5)) & 0xff;
    bytes[3] |= (((uint8_t)(raw_signal >> 0) & 0xff) << 0); ///< 8 bit(s) to B24

    raw_signal = ((uint64_t)(((from->CHKPT_DIST - (0)) / 1.0) + 0.5)) & 0xff;
    bytes[4] |= (((uint8_t)(raw_signal >> 0) & 0xff) << 0); ///< 8 bit(s) to B32

    raw_signal = ((uint64_t)(((from->REACHED_FLAG - (0)) / 1.0) + 0.5)) & 0xff;
    bytes[5] |= (((uint8_t)(raw_signal >> 0) & 0xff) << 0); ///< 8 bit(s) to B40

    raw_signal = ((uint64_t)(((from->HEADING_CURR - (0)) / 1.0) + 0.5)) & 0xff;
    bytes[6] |= (((uint8_t)(raw_signal >> 0) & 0xff) << 0); ///< 8 bit(s) to B48

    raw_signal = ((uint64_t)(((from->HEADING_DESIRED - (0)) / 1.0) + 0.5)) & 0xff;
    bytes[7] |= (((uint8_t)(raw_signal >> 0) & 0xff) << 0); ///< 8 bit(s) to B56

    return GPS_TX_COORDINATES_HDR;
}


/// Not generating code for SENSOR_TX_SONARS_encode() since the sender is SENSOR and we are GPS

/// Not generating code for DRIVER_TX_MOTOR_CMD_encode() since the sender is DRIVER and we are GPS

/// Decode DRIVER's 'HEARTBEAT' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool DRIVER_TX_HEARTBEAT_decode(DRIVER_TX_HEARTBEAT_t *to, const uint64_t *from, const msg_hdr_t *hdr)
{
    const bool success = true;
    if (NULL != hdr && (hdr->dlc != DRIVER_TX_HEARTBEAT_HDR.dlc || hdr->mid != DRIVER_TX_HEARTBEAT_HDR.mid)) {
        return !success;
    }
    uint64_t raw_signal;
    const uint8_t *bytes = (const uint8_t*) from;

    raw_signal = 0;
    raw_signal |= ((uint64_t)((bytes[0] >> 0) & 0xff)) << 0; ///< 8 bit(s) from B0
    to->DRIVER_HEARTBEAT_cmd = (raw_signal * 1.0) + (0);

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter
    return success;
}


/// Not generating code for GPS_TX_COORDINATES_decode() since we are not the recipient of any of its signals

/// Not generating code for SENSOR_TX_SONARS_decode() since we are not the recipient of any of its signals

/// Not generating code for DRIVER_TX_MOTOR_CMD_decode() since we are not the recipient of any of its signals

/// Handle the MIA for DRIVER's 'HEARTBEAT' message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter is not reset, and goes beyond the MIA value, the MIA flag is set
static inline bool DRIVER_TX_HEARTBEAT_handle_mia(DRIVER_TX_HEARTBEAT_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= HEARTBEAT__MIA_MS);

    if (!msg->mia_info.is_mia) { 
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { 
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = HEARTBEAT__MIA_MSG;
        msg->mia_info.mia_counter_ms = HEARTBEAT__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}
#endif
