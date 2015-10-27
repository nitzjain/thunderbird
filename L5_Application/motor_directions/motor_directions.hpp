//need to update this value based on sensor controller data
#define  SENSOR_THRESHOLD 1000
#define MOVE_STRAIGHT 0
#define MOVE_RIGHT 1
#define MOVE_LEFT 2
#define MOVE_REVERSE 3
#define STOP -1


typedef enum direction {

    straight = 0X020,
    left = 0x021,
    right = 0X022,
    stop = 0X023,
    reverse = 0X024,

}direction_t;

typedef enum sensor {

    sensor_straight = 0X030,
    sensor_right = 0X031,
    sensor_left = 0X032,
    sensor_rear = 0X033,

}sensor_t;

//to compute sensor values and decide the direction
//bool obstacle_detection(int *);

char direction_computation(int, int, int, int);




