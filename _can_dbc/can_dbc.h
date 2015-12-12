/// DBC file: ../_can_dbc/243.dbc    Self node: SENSOR
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
static const msg_hdr_t SENSOR_TX_SONARS_HDR =                 {  200, 6 };
static const msg_hdr_t DRIVER_TX_MOTOR_CMD_HDR =              { 0X020, 1 };


/// Message: HEARTBEAT from 'DRIVER', DLC: 1 byte(s), MID: 0xA0
typedef struct {
    uint8_t DRIVER_HEARTBEAT_cmd;             ///< B7:0   Destination: SENSOR,MOTOR,GPS

    mia_info_t mia_info;
} DRIVER_TX_HEARTBEAT_t;

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


/// Not generating 'DRIVER_TX_MOTOR_CMD_t' since we are not the sender or a recipient of any of its signals

/// These 'externs' need to be defined in a source file of your project
extern const uint32_t                                         HEARTBEAT__MIA_MS;
extern const DRIVER_TX_HEARTBEAT_t                            HEARTBEAT__MIA_MSG;

/// Not generating code for DRIVER_TX_HEARTBEAT_encode() since the sender is DRIVER and we are SENSOR

/// Encode SENSOR's 'SONARS' message
/// @returns the message header of this message
static msg_hdr_t SENSOR_TX_SONARS_encode(uint64_t *to, SENSOR_TX_SONARS_t *from)
{
    *to = 0; ///< Default the entire destination data with zeroes
    uint8_t *bytes = (uint8_t*) to;
    uint64_t raw_signal;

    raw_signal = ((uint64_t)(((from->SENSOR_SONARS_mux - (0)) / 1.0) + 0.5)) & 0x0f;
    bytes[0] |= (((uint8_t)(raw_signal >> 0) & 0x0f) << 0); ///< 4 bit(s) to B0

    raw_signal = ((uint64_t)(((from->SENSOR_SONARS_s1_fault - (0)) / 1.0) + 0.5)) & 0x01;
    bytes[0] |= (((uint8_t)(raw_signal >> 0) & 0x01) << 4); ///< 1 bit(s) to B4

    raw_signal = ((uint64_t)(((from->SENSOR_SONARS_s2_fault - (0)) / 1.0) + 0.5)) & 0x01;
    bytes[0] |= (((uint8_t)(raw_signal >> 0) & 0x01) << 5); ///< 1 bit(s) to B5

    raw_signal = ((uint64_t)(((from->SENSOR_SONARS_s3_fault - (0)) / 1.0) + 0.5)) & 0x01;
    bytes[0] |= (((uint8_t)(raw_signal >> 0) & 0x01) << 6); ///< 1 bit(s) to B6

    raw_signal = ((uint64_t)(((from->SENSOR_SONARS_s4_fault - (0)) / 1.0) + 0.5)) & 0x01;
    bytes[0] |= (((uint8_t)(raw_signal >> 0) & 0x01) << 7); ///< 1 bit(s) to B7

    if(from->m0.SENSOR_SONARS_left < 0) { from->m0.SENSOR_SONARS_left = 0; }
    if(from->m0.SENSOR_SONARS_left > 400) { from->m0.SENSOR_SONARS_left = 400; }
    raw_signal = ((uint64_t)(((from->m0.SENSOR_SONARS_left - (0)) / 0.1) + 0.5)) & 0xfff;
    bytes[1] |= (((uint8_t)(raw_signal >> 0) & 0xff) << 0); ///< 8 bit(s) to B8
    bytes[2] |= (((uint8_t)(raw_signal >> 8) & 0x0f) << 0); ///< 4 bit(s) to B16

    raw_signal = ((uint64_t)(((from->m0.SENSOR_SONARS_middle - (0)) / 0.1) + 0.5)) & 0xfff;
    bytes[2] |= (((uint8_t)(raw_signal >> 0) & 0x0f) << 4); ///< 4 bit(s) to B20
    bytes[3] |= (((uint8_t)(raw_signal >> 4) & 0xff) << 0); ///< 8 bit(s) to B24

    raw_signal = ((uint64_t)(((from->m0.SENSOR_SONARS_right - (0)) / 0.1) + 0.5)) & 0xfff;
    bytes[4] |= (((uint8_t)(raw_signal >> 0) & 0xff) << 0); ///< 8 bit(s) to B32
    bytes[5] |= (((uint8_t)(raw_signal >> 8) & 0x0f) << 0); ///< 4 bit(s) to B40

    raw_signal = ((uint64_t)(((from->m0.SENSOR_SONARS_rear - (0)) / 0.1) + 0.5)) & 0xfff;
    bytes[5] |= (((uint8_t)(raw_signal >> 0) & 0x0f) << 4); ///< 4 bit(s) to B44
    bytes[6] |= (((uint8_t)(raw_signal >> 4) & 0xff) << 0); ///< 8 bit(s) to B48

    if(from->m1.SENSOR_SONARS_no_filt_left < 0) { from->m1.SENSOR_SONARS_no_filt_left = 0; }
    if(from->m1.SENSOR_SONARS_no_filt_left > 400) { from->m1.SENSOR_SONARS_no_filt_left = 400; }
    raw_signal = ((uint64_t)(((from->m1.SENSOR_SONARS_no_filt_left - (0)) / 0.1) + 0.5)) & 0xfff;
    bytes[1] |= (((uint8_t)(raw_signal >> 0) & 0xff) << 0); ///< 8 bit(s) to B8
    bytes[2] |= (((uint8_t)(raw_signal >> 8) & 0x0f) << 0); ///< 4 bit(s) to B16

    if(from->m1.SENSOR_SONARS_no_filt_middle < 0) { from->m1.SENSOR_SONARS_no_filt_middle = 0; }
    if(from->m1.SENSOR_SONARS_no_filt_middle > 400) { from->m1.SENSOR_SONARS_no_filt_middle = 400; }
    raw_signal = ((uint64_t)(((from->m1.SENSOR_SONARS_no_filt_middle - (0)) / 0.1) + 0.5)) & 0xfff;
    bytes[2] |= (((uint8_t)(raw_signal >> 0) & 0x0f) << 4); ///< 4 bit(s) to B20
    bytes[3] |= (((uint8_t)(raw_signal >> 4) & 0xff) << 0); ///< 8 bit(s) to B24

    if(from->m1.SENSOR_SONARS_no_filt_right < 0) { from->m1.SENSOR_SONARS_no_filt_right = 0; }
    if(from->m1.SENSOR_SONARS_no_filt_right > 400) { from->m1.SENSOR_SONARS_no_filt_right = 400; }
    raw_signal = ((uint64_t)(((from->m1.SENSOR_SONARS_no_filt_right - (0)) / 0.1) + 0.5)) & 0xfff;
    bytes[4] |= (((uint8_t)(raw_signal >> 0) & 0xff) << 0); ///< 8 bit(s) to B32
    bytes[5] |= (((uint8_t)(raw_signal >> 8) & 0x0f) << 0); ///< 4 bit(s) to B40

    if(from->m1.SENSOR_SONARS_no_filt_rear < 0) { from->m1.SENSOR_SONARS_no_filt_rear = 0; }
    if(from->m1.SENSOR_SONARS_no_filt_rear > 400) { from->m1.SENSOR_SONARS_no_filt_rear = 400; }
    raw_signal = ((uint64_t)(((from->m1.SENSOR_SONARS_no_filt_rear - (0)) / 0.1) + 0.5)) & 0xfff;
    bytes[5] |= (((uint8_t)(raw_signal >> 0) & 0x0f) << 4); ///< 4 bit(s) to B44
    bytes[6] |= (((uint8_t)(raw_signal >> 4) & 0xff) << 0); ///< 8 bit(s) to B48

    return SENSOR_TX_SONARS_HDR;
}


/// Not generating code for DRIVER_TX_MOTOR_CMD_encode() since the sender is DRIVER and we are SENSOR

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
