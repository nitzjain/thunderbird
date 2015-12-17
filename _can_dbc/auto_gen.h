/// DBC file: _can_dbc/243.dbc    Self node: GPS
/// This file should be included by a source file, for example: #include "generated.c"
#ifndef __GENEARTED_DBC_PARSER
#define __GENERATED_DBC_PARSER
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>


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
static const msg_hdr_t SENSOR_TX_sensorback_HDR =             { 0xA1, 1 };
static const msg_hdr_t GPS_TX_COMPASS_HDR =                   { 0xB0, 3 };
static const msg_hdr_t GPS_TX_GPS_longitude_HDR =             { 0xB1, 8 };
static const msg_hdr_t GPS_TX_GPS_latitude_HDR =              { 0xB2, 8 };
static const msg_hdr_t GPS_TX_GPS_heading_HDR =               { 0xB3, 8 };
static const msg_hdr_t GPS_TX_GPS_dest_reached_HDR =          { 0xB4, 4 };
static const msg_hdr_t BRIDGE_TX_Check_points_HDR =           { 0xC1, 8 };
static const msg_hdr_t SENSOR_TX_SONARS_HDR =                 {  200, 6 };
static const msg_hdr_t DRIVER_TX_MOTOR_CMD_HDR =              { 0X020, 2 };


/// Message: HEARTBEAT from 'DRIVER', DLC: 1 byte(s), MID: 0xA0
typedef struct {
    uint8_t DRIVER_HEARTBEAT_cmd;             ///< B7:0   Destination: SENSOR,MOTOR,GPS

    mia_info_t mia_info;
} DRIVER_TX_HEARTBEAT_t;


/// Message: sensorback from 'SENSOR', DLC: 1 byte(s), MID: 0xA1
typedef struct {
    uint8_t SENSOR_BACK_cmd;                  ///< B7:0   Destination: DRIVER,MOTOR,GPS,IO

    mia_info_t mia_info;
} SENSOR_TX_sensorback_t;


/// Message: COMPASS from 'GPS', DLC: 3 byte(s), MID: 0xB0
typedef struct {
    uint8_t COMPASS_direction;                ///< B7:0   Destination: DRIVER,GPS,MOTOR
    int32_t COMPASS_angle;                    ///< B23:8   Destination: DRIVER,GPS,MOTOR

    mia_info_t mia_info;
} GPS_TX_COMPASS_t;


/// Message: GPS_longitude from 'GPS', DLC: 8 byte(s), MID: 0xB1
typedef struct {
    uint32_t GPS_longitude_cur;               ///< B31:0   Destination: DRIVER,GPS,MOTOR
    uint32_t GPS_longitude_dst;               ///< B63:32   Destination: DRIVER,GPS,MOTOR

    mia_info_t mia_info;
} GPS_TX_GPS_longitude_t;


/// Message: GPS_latitude from 'GPS', DLC: 8 byte(s), MID: 0xB2
typedef struct {
    uint32_t GPS_latitude_cur;                ///< B31:0   Destination: DRIVER,GPS,MOTOR
    uint32_t GPS_latitude_dst;                ///< B63:32   Destination: DRIVER,GPS,MOTOR

    mia_info_t mia_info;
} GPS_TX_GPS_latitude_t;


/// Message: GPS_heading from 'GPS', DLC: 8 byte(s), MID: 0xB3
typedef struct {
    uint32_t GPS_heading_cur;                 ///< B31:0   Destination: DRIVER,GPS,MOTOR
    uint32_t GPS_heading_dst;                 ///< B63:32   Destination: DRIVER,GPS,MOTOR

    mia_info_t mia_info;
} GPS_TX_GPS_heading_t;


/// Message: GPS_dest_reached from 'GPS', DLC: 4 byte(s), MID: 0xB4
typedef struct {
    uint32_t GPS_dest_reached;                ///< B31:0   Destination: DRIVER,GPS,MOTOR

    mia_info_t mia_info;
} GPS_TX_GPS_dest_reached_t;


/// Message: Check_points from 'BRIDGE', DLC: 8 byte(s), MID: 0xC1
typedef struct {
    uint32_t GPS_longitude_cur;               ///< B31:0   Destination: DRIVER,GPS,MOTOR
    uint32_t GPS_longitude_dst;               ///< B63:32   Destination: DRIVER,GPS,MOTOR

    mia_info_t mia_info;
} BRIDGE_TX_Check_points_t;


/// Not generating 'SENSOR_TX_SONARS_t' since we are not the sender or a recipient of any of its signals

/// Not generating 'DRIVER_TX_MOTOR_CMD_t' since we are not the sender or a recipient of any of its signals

/// These 'externs' need to be defined in a source file of your project
extern const uint32_t                                         HEARTBEAT__MIA_MS;
extern const DRIVER_TX_HEARTBEAT_t                            HEARTBEAT__MIA_MSG;
extern const uint32_t                                        sensorback__MIA_MS;
extern const SENSOR_TX_sensorback_t                          sensorback__MIA_MSG;
extern const uint32_t                                           COMPASS__MIA_MS;
extern const GPS_TX_COMPASS_t                                   COMPASS__MIA_MSG;
extern const uint32_t                                     GPS_longitude__MIA_MS;
extern const GPS_TX_GPS_longitude_t                       GPS_longitude__MIA_MSG;
extern const uint32_t                                      GPS_latitude__MIA_MS;
extern const GPS_TX_GPS_latitude_t                         GPS_latitude__MIA_MSG;
extern const uint32_t                                       GPS_heading__MIA_MS;
extern const GPS_TX_GPS_heading_t                           GPS_heading__MIA_MSG;
extern const uint32_t                                  GPS_dest_reached__MIA_MS;
extern const GPS_TX_GPS_dest_reached_t                 GPS_dest_reached__MIA_MSG;
extern const uint32_t                                      Check_points__MIA_MS;
extern const BRIDGE_TX_Check_points_t                      Check_points__MIA_MSG;

/// Not generating code for DRIVER_TX_HEARTBEAT_encode() since the sender is DRIVER and we are GPS

/// Not generating code for SENSOR_TX_sensorback_encode() since the sender is SENSOR and we are GPS

/// Encode GPS's 'COMPASS' message
/// @returns the message header of this message
static msg_hdr_t GPS_TX_COMPASS_encode(uint64_t *to, GPS_TX_COMPASS_t *from)
{
    *to = 0; ///< Default the entire destination data with zeroes
    uint8_t *bytes = (uint8_t*) to;
    uint64_t raw_signal;

    raw_signal = ((uint64_t)(((from->COMPASS_direction - (0)) / 1.0) + 0.5)) & 0xff;
    bytes[0] |= (((uint8_t)(raw_signal >> 0) & 0xff) << 0); ///< 8 bit(s) to B0

    raw_signal = ((uint64_t)(((from->COMPASS_angle - (0)) / 1.0) + 0.5)) & 0xffff;
    bytes[1] |= (((uint8_t)(raw_signal >> 0) & 0xff) << 0); ///< 8 bit(s) to B8
    bytes[2] |= (((uint8_t)(raw_signal >> 8) & 0xff) << 0); ///< 8 bit(s) to B16

    return GPS_TX_COMPASS_HDR;
}


/// Encode GPS's 'GPS_longitude' message
/// @returns the message header of this message
static msg_hdr_t GPS_TX_GPS_longitude_encode(uint64_t *to, GPS_TX_GPS_longitude_t *from)
{
    *to = 0; ///< Default the entire destination data with zeroes
    uint8_t *bytes = (uint8_t*) to;
    uint64_t raw_signal;

    raw_signal = ((uint64_t)(((from->GPS_longitude_cur - (0)) / 1.0) + 0.5)) & 0xffffffff;
    bytes[0] |= (((uint8_t)(raw_signal >> 0) & 0xff) << 0); ///< 8 bit(s) to B0
    bytes[1] |= (((uint8_t)(raw_signal >> 8) & 0xff) << 0); ///< 8 bit(s) to B8
    bytes[2] |= (((uint8_t)(raw_signal >> 16) & 0xff) << 0); ///< 8 bit(s) to B16
    bytes[3] |= (((uint8_t)(raw_signal >> 24) & 0xff) << 0); ///< 8 bit(s) to B24

    raw_signal = ((uint64_t)(((from->GPS_longitude_dst - (0)) / 1.0) + 0.5)) & 0xffffffff;
    bytes[4] |= (((uint8_t)(raw_signal >> 0) & 0xff) << 0); ///< 8 bit(s) to B32
    bytes[5] |= (((uint8_t)(raw_signal >> 8) & 0xff) << 0); ///< 8 bit(s) to B40
    bytes[6] |= (((uint8_t)(raw_signal >> 16) & 0xff) << 0); ///< 8 bit(s) to B48
    bytes[7] |= (((uint8_t)(raw_signal >> 24) & 0xff) << 0); ///< 8 bit(s) to B56

    return GPS_TX_GPS_longitude_HDR;
}


/// Encode GPS's 'GPS_latitude' message
/// @returns the message header of this message
static msg_hdr_t GPS_TX_GPS_latitude_encode(uint64_t *to, GPS_TX_GPS_latitude_t *from)
{
    *to = 0; ///< Default the entire destination data with zeroes
    uint8_t *bytes = (uint8_t*) to;
    uint64_t raw_signal;

    raw_signal = ((uint64_t)(((from->GPS_latitude_cur - (0)) / 1.0) + 0.5)) & 0xffffffff;
    bytes[0] |= (((uint8_t)(raw_signal >> 0) & 0xff) << 0); ///< 8 bit(s) to B0
    bytes[1] |= (((uint8_t)(raw_signal >> 8) & 0xff) << 0); ///< 8 bit(s) to B8
    bytes[2] |= (((uint8_t)(raw_signal >> 16) & 0xff) << 0); ///< 8 bit(s) to B16
    bytes[3] |= (((uint8_t)(raw_signal >> 24) & 0xff) << 0); ///< 8 bit(s) to B24

    raw_signal = ((uint64_t)(((from->GPS_latitude_dst - (0)) / 1.0) + 0.5)) & 0xffffffff;
    bytes[4] |= (((uint8_t)(raw_signal >> 0) & 0xff) << 0); ///< 8 bit(s) to B32
    bytes[5] |= (((uint8_t)(raw_signal >> 8) & 0xff) << 0); ///< 8 bit(s) to B40
    bytes[6] |= (((uint8_t)(raw_signal >> 16) & 0xff) << 0); ///< 8 bit(s) to B48
    bytes[7] |= (((uint8_t)(raw_signal >> 24) & 0xff) << 0); ///< 8 bit(s) to B56

    return GPS_TX_GPS_latitude_HDR;
}


/// Encode GPS's 'GPS_heading' message
/// @returns the message header of this message
static msg_hdr_t GPS_TX_GPS_heading_encode(uint64_t *to, GPS_TX_GPS_heading_t *from)
{
    *to = 0; ///< Default the entire destination data with zeroes
    uint8_t *bytes = (uint8_t*) to;
    uint64_t raw_signal;

    raw_signal = ((uint64_t)(((from->GPS_heading_cur - (0)) / 1.0) + 0.5)) & 0xffffffff;
    bytes[0] |= (((uint8_t)(raw_signal >> 0) & 0xff) << 0); ///< 8 bit(s) to B0
    bytes[1] |= (((uint8_t)(raw_signal >> 8) & 0xff) << 0); ///< 8 bit(s) to B8
    bytes[2] |= (((uint8_t)(raw_signal >> 16) & 0xff) << 0); ///< 8 bit(s) to B16
    bytes[3] |= (((uint8_t)(raw_signal >> 24) & 0xff) << 0); ///< 8 bit(s) to B24

    raw_signal = ((uint64_t)(((from->GPS_heading_dst - (0)) / 1.0) + 0.5)) & 0xffffffff;
    bytes[4] |= (((uint8_t)(raw_signal >> 0) & 0xff) << 0); ///< 8 bit(s) to B32
    bytes[5] |= (((uint8_t)(raw_signal >> 8) & 0xff) << 0); ///< 8 bit(s) to B40
    bytes[6] |= (((uint8_t)(raw_signal >> 16) & 0xff) << 0); ///< 8 bit(s) to B48
    bytes[7] |= (((uint8_t)(raw_signal >> 24) & 0xff) << 0); ///< 8 bit(s) to B56

    return GPS_TX_GPS_heading_HDR;
}


/// Encode GPS's 'GPS_dest_reached' message
/// @returns the message header of this message
static msg_hdr_t GPS_TX_GPS_dest_reached_encode(uint64_t *to, GPS_TX_GPS_dest_reached_t *from)
{
    *to = 0; ///< Default the entire destination data with zeroes
    uint8_t *bytes = (uint8_t*) to;
    uint64_t raw_signal;

    raw_signal = ((uint64_t)(((from->GPS_dest_reached - (0)) / 1.0) + 0.5)) & 0xffffffff;
    bytes[0] |= (((uint8_t)(raw_signal >> 0) & 0xff) << 0); ///< 8 bit(s) to B0
    bytes[1] |= (((uint8_t)(raw_signal >> 8) & 0xff) << 0); ///< 8 bit(s) to B8
    bytes[2] |= (((uint8_t)(raw_signal >> 16) & 0xff) << 0); ///< 8 bit(s) to B16
    bytes[3] |= (((uint8_t)(raw_signal >> 24) & 0xff) << 0); ///< 8 bit(s) to B24

    return GPS_TX_GPS_dest_reached_HDR;
}


/// Not generating code for BRIDGE_TX_Check_points_encode() since the sender is BRIDGE and we are GPS

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


/// Decode SENSOR's 'sensorback' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool SENSOR_TX_sensorback_decode(SENSOR_TX_sensorback_t *to, const uint64_t *from, const msg_hdr_t *hdr)
{
    const bool success = true;
    if (NULL != hdr && (hdr->dlc != SENSOR_TX_sensorback_HDR.dlc || hdr->mid != SENSOR_TX_sensorback_HDR.mid)) {
        return !success;
    }
    uint64_t raw_signal;
    const uint8_t *bytes = (const uint8_t*) from;

    raw_signal = 0;
    raw_signal |= ((uint64_t)((bytes[0] >> 0) & 0xff)) << 0; ///< 8 bit(s) from B0
    to->SENSOR_BACK_cmd = (raw_signal * 1.0) + (0);

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter
    return success;
}


/// Decode GPS's 'COMPASS' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool GPS_TX_COMPASS_decode(GPS_TX_COMPASS_t *to, const uint64_t *from, const msg_hdr_t *hdr)
{
    const bool success = true;
    if (NULL != hdr && (hdr->dlc != GPS_TX_COMPASS_HDR.dlc || hdr->mid != GPS_TX_COMPASS_HDR.mid)) {
        return !success;
    }
    uint64_t raw_signal;
    const uint8_t *bytes = (const uint8_t*) from;

    raw_signal = 0;
    raw_signal |= ((uint64_t)((bytes[0] >> 0) & 0xff)) << 0; ///< 8 bit(s) from B0
    to->COMPASS_direction = (raw_signal * 1.0) + (0);

    raw_signal = 0;
    raw_signal |= ((uint64_t)((bytes[1] >> 0) & 0xff)) << 0; ///< 8 bit(s) from B8
    raw_signal |= ((uint64_t)((bytes[2] >> 0) & 0xff)) << 8; ///< 8 bit(s) from B16
    to->COMPASS_angle = (raw_signal * 1.0) + (0);

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter
    return success;
}


/// Decode GPS's 'GPS_longitude' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool GPS_TX_GPS_longitude_decode(GPS_TX_GPS_longitude_t *to, const uint64_t *from, const msg_hdr_t *hdr)
{
    const bool success = true;
    if (NULL != hdr && (hdr->dlc != GPS_TX_GPS_longitude_HDR.dlc || hdr->mid != GPS_TX_GPS_longitude_HDR.mid)) {
        return !success;
    }
    uint64_t raw_signal;
    const uint8_t *bytes = (const uint8_t*) from;

    raw_signal = 0;
    raw_signal |= ((uint64_t)((bytes[0] >> 0) & 0xff)) << 0; ///< 8 bit(s) from B0
    raw_signal |= ((uint64_t)((bytes[1] >> 0) & 0xff)) << 8; ///< 8 bit(s) from B8
    raw_signal |= ((uint64_t)((bytes[2] >> 0) & 0xff)) << 16; ///< 8 bit(s) from B16
    raw_signal |= ((uint64_t)((bytes[3] >> 0) & 0xff)) << 24; ///< 8 bit(s) from B24
    to->GPS_longitude_cur = (raw_signal * 1.0) + (0);

    raw_signal = 0;
    raw_signal |= ((uint64_t)((bytes[4] >> 0) & 0xff)) << 0; ///< 8 bit(s) from B32
    raw_signal |= ((uint64_t)((bytes[5] >> 0) & 0xff)) << 8; ///< 8 bit(s) from B40
    raw_signal |= ((uint64_t)((bytes[6] >> 0) & 0xff)) << 16; ///< 8 bit(s) from B48
    raw_signal |= ((uint64_t)((bytes[7] >> 0) & 0xff)) << 24; ///< 8 bit(s) from B56
    to->GPS_longitude_dst = (raw_signal * 1.0) + (0);

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter
    return success;
}


/// Decode GPS's 'GPS_latitude' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool GPS_TX_GPS_latitude_decode(GPS_TX_GPS_latitude_t *to, const uint64_t *from, const msg_hdr_t *hdr)
{
    const bool success = true;
    if (NULL != hdr && (hdr->dlc != GPS_TX_GPS_latitude_HDR.dlc || hdr->mid != GPS_TX_GPS_latitude_HDR.mid)) {
        return !success;
    }
    uint64_t raw_signal;
    const uint8_t *bytes = (const uint8_t*) from;

    raw_signal = 0;
    raw_signal |= ((uint64_t)((bytes[0] >> 0) & 0xff)) << 0; ///< 8 bit(s) from B0
    raw_signal |= ((uint64_t)((bytes[1] >> 0) & 0xff)) << 8; ///< 8 bit(s) from B8
    raw_signal |= ((uint64_t)((bytes[2] >> 0) & 0xff)) << 16; ///< 8 bit(s) from B16
    raw_signal |= ((uint64_t)((bytes[3] >> 0) & 0xff)) << 24; ///< 8 bit(s) from B24
    to->GPS_latitude_cur = (raw_signal * 1.0) + (0);

    raw_signal = 0;
    raw_signal |= ((uint64_t)((bytes[4] >> 0) & 0xff)) << 0; ///< 8 bit(s) from B32
    raw_signal |= ((uint64_t)((bytes[5] >> 0) & 0xff)) << 8; ///< 8 bit(s) from B40
    raw_signal |= ((uint64_t)((bytes[6] >> 0) & 0xff)) << 16; ///< 8 bit(s) from B48
    raw_signal |= ((uint64_t)((bytes[7] >> 0) & 0xff)) << 24; ///< 8 bit(s) from B56
    to->GPS_latitude_dst = (raw_signal * 1.0) + (0);

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter
    return success;
}


/// Decode GPS's 'GPS_heading' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool GPS_TX_GPS_heading_decode(GPS_TX_GPS_heading_t *to, const uint64_t *from, const msg_hdr_t *hdr)
{
    const bool success = true;
    if (NULL != hdr && (hdr->dlc != GPS_TX_GPS_heading_HDR.dlc || hdr->mid != GPS_TX_GPS_heading_HDR.mid)) {
        return !success;
    }
    uint64_t raw_signal;
    const uint8_t *bytes = (const uint8_t*) from;

    raw_signal = 0;
    raw_signal |= ((uint64_t)((bytes[0] >> 0) & 0xff)) << 0; ///< 8 bit(s) from B0
    raw_signal |= ((uint64_t)((bytes[1] >> 0) & 0xff)) << 8; ///< 8 bit(s) from B8
    raw_signal |= ((uint64_t)((bytes[2] >> 0) & 0xff)) << 16; ///< 8 bit(s) from B16
    raw_signal |= ((uint64_t)((bytes[3] >> 0) & 0xff)) << 24; ///< 8 bit(s) from B24
    to->GPS_heading_cur = (raw_signal * 1.0) + (0);

    raw_signal = 0;
    raw_signal |= ((uint64_t)((bytes[4] >> 0) & 0xff)) << 0; ///< 8 bit(s) from B32
    raw_signal |= ((uint64_t)((bytes[5] >> 0) & 0xff)) << 8; ///< 8 bit(s) from B40
    raw_signal |= ((uint64_t)((bytes[6] >> 0) & 0xff)) << 16; ///< 8 bit(s) from B48
    raw_signal |= ((uint64_t)((bytes[7] >> 0) & 0xff)) << 24; ///< 8 bit(s) from B56
    to->GPS_heading_dst = (raw_signal * 1.0) + (0);

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter
    return success;
}


/// Decode GPS's 'GPS_dest_reached' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool GPS_TX_GPS_dest_reached_decode(GPS_TX_GPS_dest_reached_t *to, const uint64_t *from, const msg_hdr_t *hdr)
{
    const bool success = true;
    if (NULL != hdr && (hdr->dlc != GPS_TX_GPS_dest_reached_HDR.dlc || hdr->mid != GPS_TX_GPS_dest_reached_HDR.mid)) {
        return !success;
    }
    uint64_t raw_signal;
    const uint8_t *bytes = (const uint8_t*) from;

    raw_signal = 0;
    raw_signal |= ((uint64_t)((bytes[0] >> 0) & 0xff)) << 0; ///< 8 bit(s) from B0
    raw_signal |= ((uint64_t)((bytes[1] >> 0) & 0xff)) << 8; ///< 8 bit(s) from B8
    raw_signal |= ((uint64_t)((bytes[2] >> 0) & 0xff)) << 16; ///< 8 bit(s) from B16
    raw_signal |= ((uint64_t)((bytes[3] >> 0) & 0xff)) << 24; ///< 8 bit(s) from B24
    to->GPS_dest_reached = (raw_signal * 1.0) + (0);

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter
    return success;
}


/// Decode BRIDGE's 'Check_points' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool BRIDGE_TX_Check_points_decode(BRIDGE_TX_Check_points_t *to, const uint64_t *from, const msg_hdr_t *hdr)
{
    const bool success = true;
    if (NULL != hdr && (hdr->dlc != BRIDGE_TX_Check_points_HDR.dlc || hdr->mid != BRIDGE_TX_Check_points_HDR.mid)) {
        return !success;
    }
    uint64_t raw_signal;
    const uint8_t *bytes = (const uint8_t*) from;

    raw_signal = 0;
    raw_signal |= ((uint64_t)((bytes[0] >> 0) & 0xff)) << 0; ///< 8 bit(s) from B0
    raw_signal |= ((uint64_t)((bytes[1] >> 0) & 0xff)) << 8; ///< 8 bit(s) from B8
    raw_signal |= ((uint64_t)((bytes[2] >> 0) & 0xff)) << 16; ///< 8 bit(s) from B16
    raw_signal |= ((uint64_t)((bytes[3] >> 0) & 0xff)) << 24; ///< 8 bit(s) from B24
    to->GPS_longitude_cur = (raw_signal * 1.0) + (0);

    raw_signal = 0;
    raw_signal |= ((uint64_t)((bytes[4] >> 0) & 0xff)) << 0; ///< 8 bit(s) from B32
    raw_signal |= ((uint64_t)((bytes[5] >> 0) & 0xff)) << 8; ///< 8 bit(s) from B40
    raw_signal |= ((uint64_t)((bytes[6] >> 0) & 0xff)) << 16; ///< 8 bit(s) from B48
    raw_signal |= ((uint64_t)((bytes[7] >> 0) & 0xff)) << 24; ///< 8 bit(s) from B56
    to->GPS_longitude_dst = (raw_signal * 1.0) + (0);

    to->mia_info.mia_counter_ms = 0; ///< Reset the MIA counter
    return success;
}


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

/// Handle the MIA for SENSOR's 'sensorback' message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter is not reset, and goes beyond the MIA value, the MIA flag is set
static inline bool SENSOR_TX_sensorback_handle_mia(SENSOR_TX_sensorback_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= sensorback__MIA_MS);

    if (!msg->mia_info.is_mia) { 
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { 
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = sensorback__MIA_MSG;
        msg->mia_info.mia_counter_ms = sensorback__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

/// Handle the MIA for GPS's 'COMPASS' message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter is not reset, and goes beyond the MIA value, the MIA flag is set
static inline bool GPS_TX_COMPASS_handle_mia(GPS_TX_COMPASS_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= COMPASS__MIA_MS);

    if (!msg->mia_info.is_mia) { 
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { 
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = COMPASS__MIA_MSG;
        msg->mia_info.mia_counter_ms = COMPASS__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

/// Handle the MIA for GPS's 'GPS_longitude' message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter is not reset, and goes beyond the MIA value, the MIA flag is set
static inline bool GPS_TX_GPS_longitude_handle_mia(GPS_TX_GPS_longitude_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= GPS_longitude__MIA_MS);

    if (!msg->mia_info.is_mia) { 
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { 
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = GPS_longitude__MIA_MSG;
        msg->mia_info.mia_counter_ms = GPS_longitude__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

/// Handle the MIA for GPS's 'GPS_latitude' message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter is not reset, and goes beyond the MIA value, the MIA flag is set
static inline bool GPS_TX_GPS_latitude_handle_mia(GPS_TX_GPS_latitude_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= GPS_latitude__MIA_MS);

    if (!msg->mia_info.is_mia) { 
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { 
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = GPS_latitude__MIA_MSG;
        msg->mia_info.mia_counter_ms = GPS_latitude__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

/// Handle the MIA for GPS's 'GPS_heading' message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter is not reset, and goes beyond the MIA value, the MIA flag is set
static inline bool GPS_TX_GPS_heading_handle_mia(GPS_TX_GPS_heading_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= GPS_heading__MIA_MS);

    if (!msg->mia_info.is_mia) { 
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { 
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = GPS_heading__MIA_MSG;
        msg->mia_info.mia_counter_ms = GPS_heading__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

/// Handle the MIA for GPS's 'GPS_dest_reached' message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter is not reset, and goes beyond the MIA value, the MIA flag is set
static inline bool GPS_TX_GPS_dest_reached_handle_mia(GPS_TX_GPS_dest_reached_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= GPS_dest_reached__MIA_MS);

    if (!msg->mia_info.is_mia) { 
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { 
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = GPS_dest_reached__MIA_MSG;
        msg->mia_info.mia_counter_ms = GPS_dest_reached__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}

/// Handle the MIA for BRIDGE's 'Check_points' message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter is not reset, and goes beyond the MIA value, the MIA flag is set
static inline bool BRIDGE_TX_Check_points_handle_mia(BRIDGE_TX_Check_points_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const mia_info_t old_mia = msg->mia_info;
    msg->mia_info.is_mia = (msg->mia_info.mia_counter_ms >= Check_points__MIA_MS);

    if (!msg->mia_info.is_mia) { 
        msg->mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { 
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = Check_points__MIA_MSG;
        msg->mia_info.mia_counter_ms = Check_points__MIA_MS;
        msg->mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}
#endif
