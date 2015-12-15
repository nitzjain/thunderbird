/// DBC file: _can_dbc/243.dbc    Self node: DRIVER
/// This file should be included by a source file, for example: #include "generated.c"
#ifndef __GENEARTED_DBC_PARSER
#define __GENERATED_DBC_PARSER
#include <stdbool.h>
#include <stdint.h>



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
static const msg_hdr_t SENSOR_TX_SONARS_HDR =                 {  200, 6 };
static const msg_hdr_t DRIVER_TX_MOTOR_CMD_HDR =              { 0X020, 1 };


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

/// @{ MUX'd message: SONARS

/// Struct for MUX: m0
typedef struct {
    float SENSOR_SONARS_left;                 ///< B19:8  Min: 0 Max: 400   Destination: DRIVER,IO,MOTOR
    float SENSOR_SONARS_middle;               ///< B31:20   Destination: DRIVER,IO,MOTOR
    float SENSOR_SONARS_right;                ///< B43:32   Destination: DRIVER,IO,MOTOR
    float SENSOR_SONARS_rear;                 ///< B55:44   Destination: DRIVER,IO,MOTOR

    mia_info_t mia_info;
} SENSOR_TX_SONARS_m0_t;

/// Struct for MUX: m1
typedef struct {
    float SENSOR_SONARS_no_filt_left;         ///< B19:8  Min: 0 Max: 400   Destination: NOONE
    float SENSOR_SONARS_no_filt_middle;       ///< B31:20  Min: 0 Max: 400   Destination: NOONE
    float SENSOR_SONARS_no_filt_right;        ///< B43:32  Min: 0 Max: 400   Destination: NOONE
    float SENSOR_SONARS_no_filt_rear;         ///< B55:44  Min: 0 Max: 400   Destination: NOONE

    mia_info_t mia_info;
} SENSOR_TX_SONARS_m1_t;

/// Struct with all the child MUX'd signals
typedef struct {
    uint8_t SENSOR_SONARS_mux : 4;            ///< B3:0   Destination: DRIVER,IO,MOTOR
    uint8_t SENSOR_SONARS_s1_fault : 1;       ///< B4:4   Destination: DRIVER,IO,MOTOR
    uint8_t SENSOR_SONARS_s2_fault : 1;       ///< B5:5   Destination: DRIVER,IO,MOTOR
    uint8_t SENSOR_SONARS_s3_fault : 1;       ///< B6:6   Destination: DRIVER,IO,MOTOR
    uint8_t SENSOR_SONARS_s4_fault : 1;       ///< B7:7   Destination: DRIVER,IO,MOTOR

    SENSOR_TX_SONARS_m0_t m0; ///< MUX'd structure
    SENSOR_TX_SONARS_m1_t m1; ///< MUX'd structure
} SENSOR_TX_SONARS_t;
/// @} MUX'd message


/// Message: MOTOR_CMD from 'DRIVER', DLC: 1 byte(s), MID: 0X020
typedef struct {
    int8_t MOTOR_CMD_steer : 4;               ///< B3:0  Min: -5 Max: 5   Destination: MOTOR
    uint8_t MOTOR_CMD_drive : 4;              ///< B7:4  Min: 0 Max: 9   Destination: MOTOR

    mia_info_t mia_info;
} DRIVER_TX_MOTOR_CMD_t;


/// These 'externs' need to be defined in a source file of your project
extern const uint32_t                                        sensorback__MIA_MS;
extern const SENSOR_TX_sensorback_t                          sensorback__MIA_MSG;
extern const uint32_t                                            SONARS__MIA_MS;
extern const SENSOR_TX_SONARS_t                                  SONARS__MIA_MSG;

/// Encode DRIVER's 'HEARTBEAT' message
/// @returns the message header of this message
static msg_hdr_t DRIVER_TX_HEARTBEAT_encode(uint64_t *to, DRIVER_TX_HEARTBEAT_t *from)
{
    *to = 0; ///< Default the entire destination data with zeroes
    uint8_t *bytes = (uint8_t*) to;
    uint64_t raw_signal;

    raw_signal = ((uint64_t)(((from->DRIVER_HEARTBEAT_cmd - (0)) / 1.0) + 0.5)) & 0xff;
    bytes[0] |= (((uint8_t)(raw_signal >> 0) & 0xff) << 0); ///< 8 bit(s) to B0

    return DRIVER_TX_HEARTBEAT_HDR;
}


/// Not generating code for SENSOR_TX_sensorback_encode() since the sender is SENSOR and we are DRIVER

/// Not generating code for SENSOR_TX_SONARS_encode() since the sender is SENSOR and we are DRIVER

/// Encode DRIVER's 'MOTOR_CMD' message
/// @returns the message header of this message
static msg_hdr_t DRIVER_TX_MOTOR_CMD_encode(uint64_t *to, DRIVER_TX_MOTOR_CMD_t *from)
{
    *to = 0; ///< Default the entire destination data with zeroes
    uint8_t *bytes = (uint8_t*) to;
    uint64_t raw_signal;

    if(from->MOTOR_CMD_steer < -5) { from->MOTOR_CMD_steer = -5; }
    if(from->MOTOR_CMD_steer > 5) { from->MOTOR_CMD_steer = 5; }
    raw_signal = ((uint64_t)(((from->MOTOR_CMD_steer - (-5)) / 1.0) + 0.5)) & 0x0f;
    bytes[0] |= (((uint8_t)(raw_signal >> 0) & 0x0f) << 0); ///< 4 bit(s) to B0

    if(from->MOTOR_CMD_drive < 0) { from->MOTOR_CMD_drive = 0; }
    if(from->MOTOR_CMD_drive > 9) { from->MOTOR_CMD_drive = 9; }
    raw_signal = ((uint64_t)(((from->MOTOR_CMD_drive - (0)) / 1.0) + 0.5)) & 0x0f;
    bytes[0] |= (((uint8_t)(raw_signal >> 0) & 0x0f) << 4); ///< 4 bit(s) to B4

    return DRIVER_TX_MOTOR_CMD_HDR;
}


/// Not generating code for DRIVER_TX_HEARTBEAT_decode() since we are not the recipient of any of its signals

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


/// Decode SENSOR's 'SONARS' message
/// @param hdr  The header of the message to validate its DLC and MID; this can be NULL to skip this check
static inline bool SENSOR_TX_SONARS_decode(SENSOR_TX_SONARS_t *to, const uint64_t *from, const msg_hdr_t *hdr)
{
    const bool success = true;
    if (NULL != hdr && (hdr->dlc != SENSOR_TX_SONARS_HDR.dlc || hdr->mid != SENSOR_TX_SONARS_HDR.mid)) {
        return !success;
    }
    uint64_t raw_signal;
    const uint8_t *bytes = (const uint8_t*) from;

    // Decode the MUX
    raw_signal = 0;
    raw_signal |= ((uint64_t)((bytes[0] >> 0) & 0x0f)) << 0; ///< 4 bit(s) from B0
    to->SENSOR_SONARS_mux = (raw_signal * 1.0) + (0);

    // Decode the NON MUX'd signals
    {
        raw_signal = 0;
        raw_signal |= ((uint64_t)((bytes[0] >> 4) & 0x01)) << 0; ///< 1 bit(s) from B4
        to->SENSOR_SONARS_s1_fault = (raw_signal * 1.0) + (0);
        raw_signal = 0;
        raw_signal |= ((uint64_t)((bytes[0] >> 5) & 0x01)) << 0; ///< 1 bit(s) from B5
        to->SENSOR_SONARS_s2_fault = (raw_signal * 1.0) + (0);
        raw_signal = 0;
        raw_signal |= ((uint64_t)((bytes[0] >> 6) & 0x01)) << 0; ///< 1 bit(s) from B6
        to->SENSOR_SONARS_s3_fault = (raw_signal * 1.0) + (0);
        raw_signal = 0;
        raw_signal |= ((uint64_t)((bytes[0] >> 7) & 0x01)) << 0; ///< 1 bit(s) from B7
        to->SENSOR_SONARS_s4_fault = (raw_signal * 1.0) + (0);
    }

    if (0 == to->SENSOR_SONARS_mux) {
        raw_signal = 0;
        raw_signal |= ((uint64_t)((bytes[1] >> 0) & 0xff)) << 0; ///< 8 bit(s) from B8
        raw_signal |= ((uint64_t)((bytes[2] >> 0) & 0x0f)) << 8; ///< 4 bit(s) from B16
        to->m0.SENSOR_SONARS_left = (raw_signal * 0.1) + (0);
        raw_signal = 0;
        raw_signal |= ((uint64_t)((bytes[2] >> 4) & 0x0f)) << 0; ///< 4 bit(s) from B20
        raw_signal |= ((uint64_t)((bytes[3] >> 0) & 0xff)) << 4; ///< 8 bit(s) from B24
        to->m0.SENSOR_SONARS_middle = (raw_signal * 0.1) + (0);
        raw_signal = 0;
        raw_signal |= ((uint64_t)((bytes[4] >> 0) & 0xff)) << 0; ///< 8 bit(s) from B32
        raw_signal |= ((uint64_t)((bytes[5] >> 0) & 0x0f)) << 8; ///< 4 bit(s) from B40
        to->m0.SENSOR_SONARS_right = (raw_signal * 0.1) + (0);
        raw_signal = 0;
        raw_signal |= ((uint64_t)((bytes[5] >> 4) & 0x0f)) << 0; ///< 4 bit(s) from B44
        raw_signal |= ((uint64_t)((bytes[6] >> 0) & 0xff)) << 4; ///< 8 bit(s) from B48
        to->m0.SENSOR_SONARS_rear = (raw_signal * 0.1) + (0);
        to->m0.mia_info.mia_counter_ms = 0; ///< Reset the MIA counter
    }

    if (1 == to->SENSOR_SONARS_mux) {
        raw_signal = 0;
        raw_signal |= ((uint64_t)((bytes[1] >> 0) & 0xff)) << 0; ///< 8 bit(s) from B8
        raw_signal |= ((uint64_t)((bytes[2] >> 0) & 0x0f)) << 8; ///< 4 bit(s) from B16
        to->m1.SENSOR_SONARS_no_filt_left = (raw_signal * 0.1) + (0);
        raw_signal = 0;
        raw_signal |= ((uint64_t)((bytes[2] >> 4) & 0x0f)) << 0; ///< 4 bit(s) from B20
        raw_signal |= ((uint64_t)((bytes[3] >> 0) & 0xff)) << 4; ///< 8 bit(s) from B24
        to->m1.SENSOR_SONARS_no_filt_middle = (raw_signal * 0.1) + (0);
        raw_signal = 0;
        raw_signal |= ((uint64_t)((bytes[4] >> 0) & 0xff)) << 0; ///< 8 bit(s) from B32
        raw_signal |= ((uint64_t)((bytes[5] >> 0) & 0x0f)) << 8; ///< 4 bit(s) from B40
        to->m1.SENSOR_SONARS_no_filt_right = (raw_signal * 0.1) + (0);
        raw_signal = 0;
        raw_signal |= ((uint64_t)((bytes[5] >> 4) & 0x0f)) << 0; ///< 4 bit(s) from B44
        raw_signal |= ((uint64_t)((bytes[6] >> 0) & 0xff)) << 4; ///< 8 bit(s) from B48
        to->m1.SENSOR_SONARS_no_filt_rear = (raw_signal * 0.1) + (0);
        to->m1.mia_info.mia_counter_ms = 0; ///< Reset the MIA counter
    }

    return success;
}


/// Not generating code for DRIVER_TX_MOTOR_CMD_decode() since we are not the recipient of any of its signals

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

/// Handle the MIA for SENSOR's 'SONARS' message
/// @param   time_incr_ms  The time to increment the MIA counter with
/// @returns true if the MIA just occurred
/// @post    If the MIA counter is not reset, and goes beyond the MIA value, the MIA flag is set
static inline bool SENSOR_TX_SONARS_handle_mia(SENSOR_TX_SONARS_t *msg, uint32_t time_incr_ms)
{
    bool mia_occurred = false;
    const mia_info_t old_mia = msg->m1.mia_info;
    msg->m1.mia_info.is_mia = (msg->m1.mia_info.mia_counter_ms >= SONARS__MIA_MS);

    if (!msg->m1.mia_info.is_mia) {
        msg->m1.mia_info.mia_counter_ms += time_incr_ms;
    }
    else if(!old_mia.is_mia)   { 
        // Copy MIA struct, then re-write the MIA counter and is_mia that is overwriten
        *msg = SONARS__MIA_MSG;
        msg->m1.mia_info.mia_counter_ms = SONARS__MIA_MS;
        msg->m1.mia_info.is_mia = true;
        mia_occurred = true;
    }

    return mia_occurred;
}
#endif
